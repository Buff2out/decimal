#include "utils.h"

// Проверяем ноль ли число
// warning num null pointer. 
int is_zero(const s21_decimal* num) {
  int is_num_zero = 0;
  if (num->bits[0] == 0 && num->bits[1] == 0 && num->bits[2] == 0) {
    is_num_zero = 1;
  }

  return is_num_zero;
}

// warning num null pointer. 
int is_big_zero(const s21_big_decimal* big) {
  int is_num_zero = 0;
  if (big->bits[0] == 0 && big->bits[1] == 0 && big->bits[2] == 0 &&
      big->bits[3] == 0 && big->bits[4] == 0 && big->bits[5] == 0 &&
      big->bits[6] == 0) {
    is_num_zero = 1;
  }

  return is_num_zero;
}

// warning num null pointer. 
void reset_decimal(s21_decimal *num) {
  num->bits[0] = 0;
  num->bits[1] = 0;
  num->bits[2] = 0;
  num->bits[3] = 0;
}

// warning num null pointer. 
void reset_big_decimal(s21_big_decimal *big) {
  big->bits[0] = 0;
  big->bits[1] = 0;
  big->bits[2] = 0;
  big->bits[3] = 0;
  big->bits[4] = 0;
  big->bits[5] = 0;
  big->bits[6] = 0;
  big->bits[7] = 0;
}

// Получаем знак: 1 - минус, 0 - плюс
// warning num null pointer. 
int get_sign(const s21_decimal* num) { return ((1 << 31) & num->bits[3]) >> 31; }

// warning num null pointer. 
int get_big_sign(const s21_big_decimal* big) {
  return ((1 << 31) & big->bits[7]) >> 31;
}

// Получаем показатель степени
// warning num null pointer. 
int get_scale(const s21_decimal* num) { return ((0xFF << 16) & num->bits[3]) >> 16; }

// warning num null pointer. 
int get_big_scale(const s21_big_decimal* big) {
  return ((0xFF << 16) & big->bits[7]) >> 16;
}

// Получаем любой бит числа. Валидные pos от 0 до 95. 
// warning num, pos null pointer. 
int get_bit(const s21_decimal* num, unsigned int pos) {
  if (95 < pos) pos = 95; // ошибка: вне диапазона

  unsigned int part = dec_begin - pos / 32;
  unsigned int local_pos = pos % 32;

  return (num->bits[part] >> local_pos) & 1;
}

// warning num, pos null pointer. 
int get_big_bit(const s21_big_decimal *big, unsigned int pos) {
  if (pos > 223) pos = 223; // Защита от неверных входных данных

  unsigned int part = big_begin - pos / 32;       // Определяем, в каком bits[] находится бит
  unsigned int local_pos = pos % 32;    // Позиция внутри слова

  return (big->bits[part] >> local_pos) & 1;
}

// Ставим знак, либо 1 либо 0.
int set_sign(s21_decimal *num, unsigned int sign_value) {
  int is_err = 0;
  if (1 < sign_value || !num || !sign_value) is_err = 1;
  else {
    num->bits[dec_metainfo] = (num->bits[dec_metainfo] & ~0x80000000) | (sign_value << 31);
  }
  return is_err;
}

int set_big_sign(s21_big_decimal *big, const unsigned int sign_value) {
  int is_err = 0;
  if (1 < sign_value || !big || !sign_value) is_err = 1;
  else {
    big->bits[7] = (big->bits[7] & ~0x80000000) | (sign_value << 31);
  }
  return is_err;
}

// Ставим степень, от 0 по 28
int set_scale(s21_decimal *num, const unsigned int scale_value) {
  int is_err = 0;
  if (28 < scale_value || !num || !scale_value) is_err = 1;
  else {
    num->bits[dec_metainfo] = (num->bits[dec_metainfo] & ~0x00FF0000) | (scale_value << 16);
  }
  return is_err;
}

int set_big_scale(s21_big_decimal *big, const unsigned int scale_value) {
  int is_err = 0;
  if (28 < scale_value || !big || !scale_value) is_err = 1;
  else {
    big->bits[7] = (big->bits[7] & ~0x00FF0000) | (scale_value << 16);
  }
  return is_err;
}

// Ставим любой бит числа. Валидные pos от 0 до 95
int set_bit(s21_decimal *num, const unsigned int val, const unsigned int pos) {
  int is_err = 0;
  if (pos > 95 || !num || !val || !pos) is_err = 1;
  else {
    unsigned int part = dec_begin - pos / 32;
    unsigned int local_pos = pos % 32;

    if (val) {
        num->bits[part] |= (1u << local_pos);  // установить бит
    } else {
        num->bits[part] &= ~(1u << local_pos); // сбросить бит
    }
  }
  return is_err;
}

// Ставим любой бит числа. Валидные pos от 0 до 223
int set_big_bit(s21_big_decimal *num, const unsigned int val, const unsigned int pos) {
  int is_err = 0;
  if (223 < pos || !num || !val || !pos) is_err = 1;
  else {
    unsigned int part = big_begin - pos / 32;
    unsigned int local_pos = pos % 32;

    if (val) {
      num->bits[part] |= (1u << local_pos);  // Установить бит
    } else {
      num->bits[part] &= ~(1u << local_pos); // Сбросить бит
    }
  }
  return is_err; // Успех
}

s21_big_decimal shift_left(s21_big_decimal big, int shift_value) {
  unsigned memory = 0;
  for (int i = 0; i < (int)(sizeof(s21_big_decimal) / sizeof(unsigned) - 1);
       i++) {
    unsigned temp = big.bits[i];
    big.bits[i] <<= shift_value;
    big.bits[i] |= memory;
    memory = temp >> (32 - shift_value);
  }

  return big;
}

// warning num, big null pointer. 
void convert_to_big_decimal(const s21_decimal *num, s21_big_decimal *big) {
  *big = (s21_big_decimal){0};

  big->bits[big_metainfo] = num->bits[dec_metainfo]; //- знак и степень
  
  big->bits[big_begin - 0] = num->bits[dec_begin - 0];
  big->bits[big_begin - 1] = num->bits[dec_begin - 1];
  big->bits[big_begin - 2] = num->bits[dec_begin - 2];
}

void с_binary(int num) {
  for (int i = 31; i >= 0; i--) {
    printf("%d", (num >> i) & 1);
  }
  printf("\n");
}

void print_decimal(s21_decimal num) {
  for (int i = 0; i <= dec_begin; i++) {
    print_binary(num.bits[i]);
  }
  printf("\n");
}

void print_big_decimal(s21_big_decimal num) {
  for (int i = 0; i <= big_begin; i++) {
    print_binary(num.bits[i]);
  }
  printf("\n");
}

void normalize_scales(s21_big_decimal *num1, s21_big_decimal *num2) {
  if (get_big_scale(num1) > get_big_scale(num2)) {
    multiply_by_10(num1, num2);
  } else if (get_big_scale(num1) < get_big_scale(num2)) {
    multiply_by_10(num2, num1);
  }
}

void multiply_by_10(s21_big_decimal *num1, s21_big_decimal *num2) {
  while (get_big_scale(num1) != get_big_scale(num2)) {
    s21_big_decimal temp1 = shift_left(*num2, 3);
    s21_big_decimal temp2 = shift_left(*num2, 1);
    unsigned long long carry = 0;
    for (int i = 0; i < 7; i++) {
      unsigned long long sum =
          (unsigned long long)temp1.bits[i] + temp2.bits[i] + carry;
      num2->bits[i] = (unsigned int)(sum & 0xFFFFFFFF);
      carry = sum >> 32;
    }
    set_big_scale(num2, get_big_scale(num2) + 1);
  }
}

// для сравнения
// 1: num1 > num2, 0: num1 == num2, -1: num1 < num2
int compare_big_decimal(s21_big_decimal num1, s21_big_decimal num2) {
  int stop = 0;
  int comparison_result = 0;
  for (int i = 223; i >= 0; i--) {
    if (get_big_bit(&num1, i) > get_big_bit(&num2, i) && !stop) {
      stop = 1;
      comparison_result = 1;
    } else if (get_big_bit(&num1, i) < get_big_bit(&num2, i) && !stop) {
      stop = 1;
      comparison_result = -1;
    }
  }
  return comparison_result;
}

// 1: num1 > num2, 0: num1 == num2, -1: num1 < num2
int compare_decimal(s21_decimal num1, s21_decimal num2) {
  s21_big_decimal value_1_big, value_2_big;
  convert_to_big_decimal(&num1, &value_1_big);
  convert_to_big_decimal(&num2, &value_2_big);
  normalize_scales(&value_1_big, &value_2_big);
  return compare_big_decimal(value_1_big, value_2_big);
}