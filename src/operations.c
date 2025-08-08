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


// Vibe coded
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    if (!result) return 2;
    if (is_zero(&value_2)) return 3; // деление на ноль
    reset_decimal(result);

    s21_big_decimal a = {0}, b = {0};
    to_big(&value_1, &a);
    to_big(&value_2, &b);

    int a_sign = get_big_sign(&a);
    int b_sign = get_big_sign(&b);
    int result_sign = a_sign ^ b_sign;

    int a_scale = get_big_scale(&a);
    int b_scale = get_big_scale(&b);

    /*
      Деление: 
      у нас есть 
      a/b = c + rem/b
    */
    s21_big_decimal a_copy = {0};
    copy_big(&a, &a_copy);
    s21_big_decimal b_copy = {0};
    copy_big(&b, &b_copy);
    int a_copy_scale = get_big_scale(&a);

    unsigned power_b = 0; // степень двойки которую мы прибавляем каждый раз как домножаем
    s21_big_decimal right = {0};
    s21_big_decimal roof = new_big_native(4189374BC6A7EF9DB22D0E56041893, unsigned int b5, unsigned int b4, unsigned int b3, 0x74BC6A7E, 0xF9DB22D0, 0xE5604189, 0)

    while () {

    }

    

    copy_big(&a, &right); // результат умножения делителя на два в степени power_b
    shift_left_to(&right, 31);
    int compare_res = compare_big_decimal(&a_copy, &b_copy);
    while (compare_res == -1) {
      shift_left_to(&right, 1); // сдвиг - то же самое что и умножение на 2
      ++power_b; // инкрементируем степень
      compare_res = compare_big_decimal(&a, &b);
    }
    /*
      unsigned long long power_b = 1; // возможно не нужно

      a * 10 и каждый раз как в цикле умножаем на 10, Прибавляем scale (как только overflow, stop)
      после этого

      right = b_copy * right;



      итак после всего этого мы определили границы того в каких пределах домножаемого находится число
      например это будет между 2^35 и 2^36
      теперь нам нужно уточнять множитель. Будем уточнять его через ту же степень двойки. 
      Мы знаем гарантированно что right гарантированно будет делиться на два. 
      Всё что нам надо делать это прибавлять к результату примерно так
      s21_big_decimal added = {0};

      здесь мы должны понять насколько меньшее число в диапазоне от 2^35 и 2^36 мы должны прибавить, 
      чтобы получить
      while (res >= 0) {
        copy_big(right, added);
        shift_right_to(&added);

      }

    */
    if (power > 0) {
        shift_right_to(&b_power, 1); // возвращаемся к последнему, где b_power <= remainder
        power--;
    }

    // Теперь: b * 2^power <= remainder < b * 2^(power+1)
    // Вычитаем и добавляем 2^power к частному
    while (power >= 0) {
        if (compare_big_decimal(&b_power, &remainder) <= 0) {
            sub(&remainder, &b_power, &remainder);
            // Добавляем 2^power к частному
            s21_big_decimal two_power = new_big_from_int(1);
            shift_left_to(&two_power, power);
            add(&quotient, &two_power, &quotient);
        }
        shift_right_to(&b_power, 1);
        power--;
    }

    // Устанавливаем scale и знак
    set_big_scale(&quotient, result_scale);
    set_big_sign(&quotient, result_sign);

    // Если scale > 28 — округляем
    while (get_big_scale(&quotient) > 28) {
        bank_round(&quotient);
    }

    // Конвертируем обратно
    return to_dec(&quotient, result);
}


