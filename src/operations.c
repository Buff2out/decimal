// src/operations.c
#include "../src/operations.h"

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

  s21_big_decimal big1 = {0}, big2 = {0};
  to_big(&value_1, &big1);
  to_big(&value_2, &big2);

  // int scale = get_scale(&value_1) + get_scale(&value_2);

  s21_big_decimal big_result = {0};
  int flag = mul(&big1, &big2, &big_result);
  flag = to_dec_with_bank_round(&big_result, result);

  return flag;
}

/*
Деление: 
у нас есть 
a/b = c + rem/b

алгоритм.
заводим переменные 
s21_big_decimal a_copy = {0};
copy_big(a, a_copy);
s21_big_decimal b_copy = {0};
copy_big(b, b_copy);
int a_copy_scale = get_big_scale(a);

unsigned long long mult_b = 1; // возможно не нужно

s21_big_decimal two = new_big_from_int(2);
unsigned two_scale = 0; // степень двойки которую мы прибавляем каждый раз как домножаем
s21_big_decimal b_res = new_big_from_int(1) // результат умножения делителя на два в степени two_scale

a * 10 и каждый раз как в цикле умножаем на 10, Прибавляем scale (как только overflow, stop)
после этого

b_res = b_copy * b_res;


while (res >= 0) {
  res = a_copy - b_res;
  shift_left_to(&b_res, 1); // сдвиг - то же самое что и умножение на 2
  ++mult_b // инкрементируем степень
}
итак после всего этого мы определили границы того в каких пределах домножаемого находится число
например это будет между 2^35 и 2^36
теперь нам нужно уточнять множитель. Будем уточнять его через ту же степень двойки. 
Мы знаем гарантированно что b_res гарантированно будет делиться на два. 
Всё что нам надо делать это прибавлять к результату примерно так
s21_big_decimal added = {0};

здесь мы должны понять насколько меньшее число в диапазоне от 2^35 и 2^36 мы должны прибавить, 
чтобы получить
while (res >= 0) {
  copy_big(b_res, added);
  shift_right_to(&added);

}

*/
