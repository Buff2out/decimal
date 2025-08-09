// src/operations.c
#include "../src/operations.h"
#include "s21_decimal.h"
#include "utils/utils.h"

int add(const s21_big_decimal *a, const s21_big_decimal *b,
        s21_big_decimal *result) {
  int flag = OK;
  if (!a || !b || !result) {
    flag = ERROR;
  } else {
    *result = (s21_big_decimal){0};
    unsigned long long carry = 0;
    for (int i = BIG_BEGIN; i <= BIG_END; ++i) {
      unsigned long long sum =
          (unsigned long long)a->bits[i] + b->bits[i] + carry;
      result->bits[i] = (unsigned)(sum & 0xFFFFFFFF);
      carry = sum >> 32;
    }
    result->bits[BIG_METAINFO] = a->bits[BIG_METAINFO];
    if (carry) {
      int sign_1 = get_big_sign(a);
      flag = (sign_1 == 0) ? PLUS_INF : MINUS_INF;
    }
  }
  return flag;
}

// Функция подразумевает, что из большего числа вычитают меньшее.
int sub(const s21_big_decimal *a, const s21_big_decimal *b,
        s21_big_decimal *result) {
  int flag = OK;
  if (!a || !b || !result) {
    flag = ERROR;
  } else {
    *result = (s21_big_decimal){0};
    long long borrow = 0;
    for (int i = BIG_BEGIN; i <= BIG_END; ++i) {
      long long diff = (long long)a->bits[i] - b->bits[i] - borrow;
      borrow = 0;
      if (diff < 0) {
        diff += 0x100000000LL;
        borrow = 1;
      }
      result->bits[i] = (unsigned int)(diff & 0xFFFFFFFF);
    }
    result->bits[BIG_METAINFO] = a->bits[BIG_METAINFO];
    if (borrow)
      flag = MINUS_INF;
  }
  return flag;
}

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int flag = OK;
  if (!result) {
    flag = ERROR;
  } else {
    *result = (s21_decimal){0};

    s21_big_decimal big_1 = {0}, big_2 = {0};
    to_big(&value_1, &big_1);
    to_big(&value_2, &big_2);

    normalize_scales(&big_1, &big_2);

    const int sign_1 = get_sign(&value_1);
    const int sign_2 = get_sign(&value_2);

    int cmp_res = compare_big_decimal(&big_1, &big_2);

    s21_big_decimal result_big = {0};

    /*
    Группировка по операциям
    +a, +b + => a + b (sign a)
    -a, -b + => a + b (sign a)
    +a, -b + (a > b) => a - b (sign a)
    -a, +b + (a > b) => a - b (sign a)
    +a, -b + (a <= b) => b - a (sign b)
    -a, +b + (a <= b) => b - a (sign b)
    */
    if (sign_1 == sign_2) {
      flag = add(&big_1, &big_2, &result_big);
      set_big_sign(&result_big, sign_1);
    } else if (sign_1 != sign_2 && 1 == cmp_res) {
      flag = sub(&big_1, &big_2, &result_big);
      set_big_sign(&result_big, sign_1);
    } else if (sign_1 != sign_2 && (0 == cmp_res || -1 == cmp_res)) {
      flag = sub(&big_2, &big_1, &result_big);
      set_big_sign(&result_big, sign_2);
    }
    to_dec_with_bank_round(&result_big, result);
  }
  return flag;
}

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int flag = OK;
  if (!result) {
    flag = ERROR;
  } else {
    *result = (s21_decimal){0};

    s21_big_decimal big_1 = {0}, big_2 = {0};
    to_big(&value_1, &big_1);
    to_big(&value_2, &big_2);

    normalize_scales(&big_1, &big_2);

    const int sign_1 = get_sign(&value_1);
    const int sign_2 = get_sign(&value_2);

    int cmp_res = compare_big_decimal(&big_1, &big_2);

    s21_big_decimal result_big = {0};

    /*
    Группировка по операциям
    +a, -b - => a + b (sign a)
    -a, +b - => a + b (sign a)
    +a, +b - (a > b) => a - b (sign a)
    -a, -b - (a > b) => a - b (sign a)
    +a, +b - (a <= b) => b - a (sign !b)
    -a, -b - (a <= b) => b - a (sign !b)
    */
    if (sign_1 != sign_2) {
      flag = add(&big_1, &big_2, &result_big);
      set_big_sign(&result_big, sign_1);
    } else if (1 == cmp_res) {
      flag = sub(&big_1, &big_2, &result_big);
      set_big_sign(&result_big, sign_1);
    } else if (0 == cmp_res || -1 == cmp_res) {
      flag = sub(&big_2, &big_1, &result_big);
      const int sign = 0 == cmp_res ? SIGN_PLUS : sign_2;
      set_big_sign(&result_big, sign);
    }
    to_dec_with_bank_round(&result_big, result);
  }
  return flag;
}

int mul(const s21_big_decimal *a, const s21_big_decimal *b,
        s21_big_decimal *result) {
  int sign = get_big_sign(a) ^ get_big_sign(b);
  int flag = OK;
  s21_big_decimal res = {0};
  set_big_sign(&res, sign);
  set_big_scale(&res, get_big_scale(a) + get_big_scale(b));
  for (int i = 0; !flag && i <= BIG_END; i++) {
    for (int j = 0; !flag && j <= BIG_END; j++) {
      unsigned long long product = (unsigned long long)a->bits[i] * b->bits[j];
      if (i + j > BIG_END && product != 0) {
          flag = sign ? MINUS_INF : PLUS_INF;
      } else {
        s21_big_decimal partial = {0};
        partial.bits[i + j] = (unsigned)product;
        int carry = (unsigned)(product >> 32);
        if (i + j + 1 <= BIG_END) {
          partial.bits[i + j + 1] = carry;
        }
        if (carry && i == BIG_END) {
          flag = sign ? MINUS_INF : PLUS_INF;
        } else {
          s21_big_decimal temp = {0};
          flag = add(&res, &partial, &temp);
          copy_big(&temp, &res);
        }
      }
    }
  }
  *result = res;
  return flag;
}

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  if (!result)
    return ERROR;

  *result = (s21_decimal){0};
  // int sign = get_sign(&value_1) ^ get_sign(&value_2);

  s21_big_decimal big_1 = {0}, big_2 = {0};
  to_big(&value_1, &big_1);
  to_big(&value_2, &big_2);

  normalize_scales(&big_1, &big_2);

  // int scale = get_scale(&value_1) + get_scale(&value_2);

  s21_big_decimal big_result = {0};
  int flag = mul(&big_1, &big_2, &big_result);
  flag = to_dec_with_bank_round(&big_result, result);

  return flag;
}

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    int flag = OK;
  // не забыть обработку краевого когда делимое или делитель равны нулю!!!!!
    if (!result) {
      flag = ERROR;
    } else if (is_zero(&value_2)) { 
      flag = DIV_BY_ZERO;
    } else if (is_zero(&value_1)) {
      s21_decimal res = new_dec_from_int(0);
      copy_dec(&res, result);
    } else {
      reset_decimal(result);
  
      s21_big_decimal a = {0}, b = {0};
      to_big(&value_1, &a);
      to_big(&value_2, &b);
  
      int a_sign = get_big_sign(&a);
      int b_sign = get_big_sign(&b);
      int result_sign = a_sign ^ b_sign;
  
      // int a_scale = get_big_scale(&a);
      // int b_scale = get_big_scale(&b);
  
      s21_big_decimal a_copy = {0};
      copy_big(&a, &a_copy);
      s21_big_decimal b_copy = {0};
      copy_big(&b, &b_copy);
  
      unsigned power_b = 0; // степень двойки которую мы прибавляем каждый раз как домножаем
      s21_big_decimal right = {0};
      s21_big_decimal roof = new_big_native(0x418937, 0x4BC6A7EF, 0x9DB22D0E, 0x56041893, 0x74BC6A7E, 0xF9DB22D0, 0xE5604189, 0);
  
      // умножаем делимое кучу раз на 10 пока оно не станет достаточно большим
      int scale_borrowed = get_big_scale(&a_copy);
      while (-1 == compare_big_decimal(&a_copy, &roof)) {
        multiply_by_10(&a_copy);
        ++scale_borrowed;
      }
  
      copy_big(&b, &right);
      
      shift_left_to(&right, 31);
      shift_left_to(&right, 31);
      shift_left_to(&right, 31); // чтобы сэкономить время
      power_b = 93;
      // не путать shift_left и left! Первое - умножение, а второе про интервал
      // понимаем что res это что то вроде
      // 2^191 + 2^190 + 2^187 + 2^185 + ... + 2^3 + 2^0 - это и есть то самое частное
      s21_big_decimal res = new_big_from_int(1);
      set_big_scale(&res, scale_borrowed);
      shift_left_to(&res, 31);
      shift_left_to(&res, 31);
      shift_left_to(&res, 31);
      while (-1 == compare_big_decimal(&right, &a_copy)) {  
        shift_left_to(&right, 1); // сдвиг - то же самое что и умножение на 2
        shift_left_to(&res, 1);
        ++power_b; // инкрементируем степень
      }
      s21_big_decimal second = {0};
      shift_right_to(&res, 1);
      copy_big(&res, &second);
      --power_b;
      s21_big_decimal left = {0};
      copy_big(&right, &left);
      shift_right_to(&left, 1); // уменьшаем лево 
      // итого получаем что наше частное находится в интервале между left и right

      s21_big_decimal added = {0};
      s21_big_decimal middle = {0};
      copy_big(&left, &added);
      copy_big(&left, &middle);
      // TODO
      int stop_flag = 0;
      while (!stop_flag && power_b != 0) {
        shift_right_to(&second, 1);
        shift_right_to(&added, 1);
        --power_b;
        // вооот. Ниже цикл в котором мы берём половину и замеряем. Наше частное слева или справа.
        // ниже как раз мы и будем присваивать то right то left уточняя диапазон.
        add(&left, &added, &middle);
        int cmpres = compare_big_decimal(&middle, &a_copy);
        // если правый всё ещё больше, то мы недостаточно прибавили, двигаем левый
        if (-1 == cmpres) {
          copy_big(&middle, &left);
          s21_big_decimal temp = {0};
          add(&res, &second, &temp);
          copy_big(&temp, &res);
        } else if (0 == cmpres) {
          stop_flag = 1;
        }
      }
      // if (stop_flag == 0) { // если разница оказалась равна нулю. То есть c остатком.
      //   s21_big_decimal rem = {0};
      //   sub(&a_copy, &middle, &rem);
      // }
      // TODO понять как скейл с делимого адаптировать под result.
      // наверное мы просто прибавим скейл от a_copy к middle
      set_big_sign(&res, result_sign);
      to_dec_with_bank_round(&res, result);
    }
    return flag;
}


