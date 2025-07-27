// src/utils/utils.c
#include "utils.h"
#include "../../src/operations.h"

// Вспомогательная функция для создания big_decimal из массива
s21_big_decimal from_bits(unsigned b0, unsigned b1, unsigned b2,
                          unsigned b3, unsigned b4, unsigned b5,
                          unsigned b6, unsigned b7) {
    s21_big_decimal result = {{b0, b1, b2, b3, b4, b5, b6, b7}};
    return result;
}

// Вспомогательная функция для создания decimal из массива
s21_decimal new_dec_native(unsigned b2, unsigned b1, unsigned b0, unsigned b3) {
    s21_decimal result = {{b0, b1, b2, b3}};
    return result;
}

s21_decimal get_dec_from_int(int val) {
  s21_decimal result = {{0, 0, val, 0}};
  if (val < 0) {
    set_scale(&result, 1);
  }
  return result;
}

// Вспомогательная функция для создания big_decimal из массива
s21_big_decimal new_big_native(unsigned b6, unsigned b5, unsigned b4, unsigned b3, 
                               unsigned b2, unsigned b1, unsigned b0, unsigned b7) {
    s21_big_decimal result = {{b0, b1, b2, b3, b4, b5, b6, b7}};
    return result;
}

s21_big_decimal get_big_from_int(int val) {
  s21_big_decimal result = {{0, 0, 0, 0, 0, 0, val, 0}};
  if (val < 0) {
    set_big_scale(&result, 1);
  }
  return result;
}

// Вспомогательная функция для создания big_decimal из массива
s21_big_decimal from_bits_native(unsigned b6, unsigned b5, unsigned b4, 
                                             unsigned b3, unsigned b2, unsigned b1, unsigned b0, unsigned b7) {
    s21_big_decimal result = {{b0, b1, b2, b3, b4, b5, b6, b7}};
    return result;
}

// void copy_dec(s21_decimal* from, s21_decimal* to) {
//   to->bits[0] = from->bits[0];
//   to->bits[1] = from->bits[1];
//   to->bits[2] = from->bits[2];
//   to->bits[3] = from->bits[3];
// }

void copy_big(s21_big_decimal* from, s21_big_decimal* to) {
  to->bits[0] = from->bits[0];
  to->bits[1] = from->bits[1];
  to->bits[2] = from->bits[2];
  to->bits[3] = from->bits[3];
  to->bits[4] = from->bits[4];
  to->bits[5] = from->bits[5];
  to->bits[6] = from->bits[6];
  to->bits[7] = from->bits[7];
}

// Проверяем ноль ли число
// warning num null pointer. 
int is_zero(const s21_decimal* num) {
  int is_num_zero = FALSE;
  if (num->bits[0] == 0 && num->bits[1] == 0 && num->bits[2] == 0) {
    is_num_zero = TRUE;
  }

  return is_num_zero;
}

// warning num null pointer. 
int is_big_zero(const s21_big_decimal* big) {
  int is_num_zero = FALSE;
  if (big->bits[0] == 0 && big->bits[1] == 0 && big->bits[2] == 0 &&
      big->bits[3] == 0 && big->bits[4] == 0 && big->bits[5] == 0 &&
      big->bits[6] == 0) {
    is_num_zero = TRUE;
  }

  return is_num_zero;
}

// warning num null pointer. 
void reset_decimal(s21_decimal *num) {
  *num = (s21_decimal){0};
}

// warning num null pointer. 
void reset_big_decimal(s21_big_decimal *big) {
  *big = (s21_big_decimal){0}; 
}

// Получаем знак: 1 - минус, 0 - плюс
// warning num null pointer. 
int get_sign(const s21_decimal* num) { return ((1 << 31) & num->bits[DEC_METAINFO]) >> 31; }

// warning num null pointer. 
int get_big_sign(const s21_big_decimal* big) {
  return ((1 << 31) & big->bits[BIG_METAINFO]) >> 31;
}

// Получаем показатель степени
// warning num null pointer. 
int get_scale(const s21_decimal* num) { return ((0xFF << 16) & num->bits[DEC_METAINFO]) >> 16; }

// warning num null pointer. 
int get_big_scale(const s21_big_decimal* big) {
  return ((0xFF << 16) & big->bits[BIG_METAINFO]) >> 16;
}

// Получаем любой бит числа. Валидные pos от 0 до 95. 
// warning num, pos null pointer. 
int get_bit(const s21_decimal* num, unsigned pos) {
  if (DEC_MAX_POS < pos) pos = DEC_MAX_POS; // ошибка: вне диапазона

  unsigned part = pos / 32;
  unsigned local_pos = pos % 32;

  return (num->bits[part] >> local_pos) & 1;
}

// warning num, pos null pointer. 
int get_big_bit(const s21_big_decimal *big, unsigned pos) {
  if (BIG_MAX_POS < pos) pos = BIG_MAX_POS; // Защита от неверных входных данных

  unsigned part = pos / 32;       // Определяем, в каком bits[] находится бит
  unsigned local_pos = pos % 32;    // Позиция внутри слова

  return (big->bits[part] >> local_pos) & 1;
}

// Ставим знак, либо 1 либо 0.
int set_sign(s21_decimal *num, unsigned sign_value) {
  int is_err = FALSE;
  if (1 < sign_value || !num || !sign_value) is_err = TRUE;
  else {
    num->bits[DEC_METAINFO] = (num->bits[DEC_METAINFO] & ~0x80000000) | (sign_value << 31);
  }
  return is_err;
}

int set_big_sign(s21_big_decimal *big, const unsigned sign_value) {
  int is_err = FALSE;
  if (1 < sign_value || !big || !sign_value) is_err = TRUE;
  else {
    big->bits[BIG_METAINFO] = (big->bits[BIG_METAINFO] & ~0x80000000) | (sign_value << 31);
  }
  return is_err;
}

// Ставим степень, от 0 по 28
int set_scale(s21_decimal *num, const unsigned scale_value) {
  int is_err = FALSE;
  if (28 < scale_value || !num || !scale_value) is_err = TRUE;
  else {
    num->bits[DEC_METAINFO] = (num->bits[DEC_METAINFO] & ~0x00FF0000) | (scale_value << 16);
  }
  return is_err;
}

int set_big_scale(s21_big_decimal *big, const unsigned scale_value) {
  int is_err = FALSE;
  if (28 < scale_value || !big || !scale_value) is_err = TRUE;
  else {
    big->bits[BIG_METAINFO] = (big->bits[BIG_METAINFO] & ~0x00FF0000) | (scale_value << 16);
  }
  return is_err;
}

// Ставим любой бит числа. Валидные pos от 0 до 95
int set_bit(s21_decimal *num, const unsigned val, const unsigned pos) {
  int is_err = FALSE;
  if (DEC_MAX_POS < pos || !num || !val || !pos) is_err = TRUE;
  else {
    unsigned part = pos / 32;
    unsigned local_pos = pos % 32;

    if (val) {
        num->bits[part] |= (1u << local_pos);  // установить бит
    } else {
        num->bits[part] &= ~(1u << local_pos); // сбросить бит
    }
  }
  return is_err;
}

// Ставим любой бит числа. Валидные pos от 0 до 223
int set_big_bit(s21_big_decimal *num, const unsigned val, const unsigned pos) {
  int is_err = FALSE;
  if (BIG_MAX_POS < pos || !num || !val || !pos) is_err = TRUE;
  else {
    unsigned part = pos / 32;
    unsigned local_pos = pos % 32;

    if (val) {
      num->bits[part] |= (1u << local_pos);  // Установить бит
    } else {
      num->bits[part] &= ~(1u << local_pos); // Сбросить бит
    }
  }
  return is_err; // Успех
}

s21_big_decimal shift_left(s21_big_decimal big, unsigned shift_value) {
  unsigned memory = 0;
  if (31 < shift_value) shift_value = 31;
  for (unsigned i = BIG_BEGIN; i <= BIG_END; ++i) { // (int)(sizeof(s21_big_decimal) / sizeof(unsigned) - 1
    unsigned temp = big.bits[i];
    big.bits[i] <<= shift_value;
    big.bits[i] |= memory;
    memory = temp >> (32 - shift_value);
  }

  return big;
}

// void shift_left_to(s21_big_decimal * big, unsigned shift_value) {
//   unsigned memory = 0;
//   if (31 < shift_value) shift_value = 31;
//   for (unsigned i = BIG_BEGIN; i <= BIG_END; ++i) { // (int)(sizeof(s21_big_decimal) / sizeof(unsigned) - 1
//     unsigned temp = big->bits[i];
//     big->bits[i] <<= shift_value;
//     big->bits[i] |= memory;
//     memory = temp >> (32 - shift_value);
//   }
// }

// warning num, big null pointer. 
// legacy function
void convert_to_big_decimal(const s21_decimal *num, s21_big_decimal *big) {
  *big = (s21_big_decimal){0};

  big->bits[BIG_METAINFO] = num->bits[DEC_METAINFO]; //- знак и степень
  
  big->bits[0] = num->bits[0];
  big->bits[1] = num->bits[1];
  big->bits[2] = num->bits[2];
}

int to_big(const s21_decimal *num, s21_big_decimal *big) {
  const int ok = 0;
  const int null_big = 1;
  int flag = ok;
  *big = (s21_big_decimal){0};
  if (!num) flag = null_big;
  else {
    big->bits[BIG_METAINFO] = num->bits[DEC_METAINFO]; //- знак и степень
    
    big->bits[0] = num->bits[0];
    big->bits[1] = num->bits[1];
    big->bits[2] = num->bits[2];
  }
  return flag;
}

int fits_in_decimal(const s21_big_decimal *const big) {
  int fits = 1;
  for (int i = DEC_END + 1; fits && i <= BIG_END; i++) {
      if (big->bits[i] != 0) fits = 0;
  }
  return fits;
}

int to_dec(s21_big_decimal *big, s21_decimal *num) {
  const int ok = 0;
  const int null_big = 1;
  const int overflow = 2;

  int flag = ok;
  *num = (s21_decimal){0};

  if (!big) flag = null_big;
  else {
    num->bits[DEC_METAINFO] = big->bits[BIG_METAINFO]; //- знак и степень
    for (int i = DEC_BEGIN; i <= DEC_END; ++i) num->bits[i] = big->bits[i];
    for (int i = DEC_END + 1; !flag && i <= BIG_END; ++i) {
      if (big->bits[i] != 0) flag = overflow;
    }
  }
  return flag;
}

int to_dec_with_bank_round(s21_big_decimal *big, s21_decimal *num) {
  const int ok = 0;
  const int null_big = 1;
  const int overflow = 2;

  int flag = ok;
  *num = (s21_decimal){0};

  if (!big) flag = null_big;
  else {
    flag = fits_in_decimal(big) ? ok : overflow;
    while (overflow == flag && get_big_scale(big)) {
      int rem = num->bits[0] % 10;
      int carry = rem > 5; // если rem > 5, то true (carry = 1)
      if (5 == rem && 0 == carry) { // если нечётное число перед last цифрой,
        carry = 1 == ((num->bits[0] % 100) / 10) % 2; // то carry = 1 (true), то есть округлять вверх
      }

      divide_by_10(big);
      s21_big_decimal temp_res = {0};
      s21_big_decimal temp_carry = get_big_from_int(carry);
      if (carry) add(big, &temp_carry, &temp_res);
      flag = fits_in_decimal(big) ? ok : overflow;
    }
    if (ok == flag) {
      for (int i = DEC_BEGIN; !flag && i <= DEC_END; ++i) { 
        num->bits[i] = big->bits[i]; 
      }
      num->bits[DEC_METAINFO] = big->bits[BIG_METAINFO]; //- знак и степень
    }
  }
  return flag;
}

// printf("HEX: %X\n", x); // printf - в шестнадцатеричном
// ниже в бинарном
void print_binary(unsigned num) {
  for (unsigned i = 31; i > 0; i--) {
    printf("%d", (num >> i) & 1);
  }
  printf("%d", (num >> 0) & 1);
  printf("\n");
}

void print_dec(const s21_decimal* num) {
  for (unsigned i = DEC_BEGIN; i <= DEC_END; ++i) {
    printf("%d: ", i);
    print_binary(num->bits[i]);
  }
  printf("\n");
}

void print_big(const s21_big_decimal* num) {
  for (unsigned i = BIG_BEGIN; i <= BIG_END; ++i) {
    printf("%d: ", i);
    print_binary(num->bits[i]);
  }
  printf("\n");
}

void print_dec_native(const s21_decimal* num) {
  for (unsigned i = DEC_END; i > DEC_BEGIN; --i) {
    print_binary(num->bits[i]);
  }
  printf("%d: ", 0);
  print_binary(num->bits[0]);
  printf("\n");
}

void print_dec_native_hex(const s21_decimal* num) {
  for (unsigned i = DEC_END; i > DEC_BEGIN; --i) {
    printf("%d: 0x%08X ", i, num->bits[i]);
  }
  printf("%d: 0x%08X ", 0, num->bits[0]);
  printf("%d: 0x%08X ", 3, num->bits[3]);
  printf("\n");
}

void print_big_native(const s21_big_decimal* num) {
  for (unsigned i = BIG_END; i > BIG_BEGIN; --i) {
    printf("%d: ", i);
    print_binary(num->bits[i]);
  }
  printf("%d: ", 0);
  print_binary(num->bits[0]);
  printf("\n");
}

void print_big_native_hex(const s21_big_decimal* num) {
  for (unsigned i = BIG_END; i > BIG_BEGIN; --i) {
    printf("%d: 0x%08X ", i, num->bits[i]);
  }
  printf("%d: ", 0);
  printf("%d: 0x%08X\n", 0, num->bits[0]);
  printf("\n");
}

void normalize_scales(s21_big_decimal *num1, s21_big_decimal *num2) {
  while (get_big_scale(num1) < get_big_scale(num2)) {
    multiply_by_10(num1);
  }
  while (get_big_scale(num2) < get_big_scale(num1)) {
    multiply_by_10(num2);
  }
}

void multiply_by_10(s21_big_decimal *big) {
  s21_big_decimal temp1 = shift_left(*big, 3); // умножает на 8 (2^3)
  s21_big_decimal temp2 = shift_left(*big, 1); // умножает на 2 (2^1)
  unsigned long long carry = 0;

  for (int i = BIG_BEGIN; i <= BIG_END; ++i) {
    unsigned long long sum = 
        (unsigned long long)temp1.bits[i] + temp2.bits[i] + carry;
    big->bits[i] = (unsigned)(sum & 0xFFFFFFFF);
    carry = sum >> 32;
  }

  set_big_scale(big, get_big_scale(big) + 1);
}

unsigned divide_by_10(s21_big_decimal *big) {
    unsigned long long remainder = 0;
    for (int i = BIG_END; i >= BIG_BEGIN; i--) {
        unsigned long long cur = ((unsigned long long)remainder << 32) + big->bits[i];
        big->bits[i] = (unsigned int)(cur / 10);
        remainder = cur % 10;
    }
    set_big_scale(big, get_big_scale(big) - 1);
    return (unsigned)remainder; // остаток от деления на 10
}


// для сравнения
// 1: big_1 > big_2, 0: big_1 == big_2, -1: big_1 < big_2
int compare_big_decimal(const s21_big_decimal* big_1, const s21_big_decimal* big_2) {
  int comparison_result = 0;
  for (int i = BIG_END; !comparison_result && i >= BIG_BEGIN; --i) {
    if (big_1->bits[i] > big_2->bits[i]) comparison_result = 1;
    else if (big_1->bits[i] < big_2->bits[i]) comparison_result = -1;
  }
  return comparison_result;
}

// 1: num1 > num2, 0: num1 == num2, -1: num1 < num2
int compare_decimal(const s21_decimal* num1, const s21_decimal* num2) {
  s21_big_decimal value_1_big, value_2_big;
  to_big(num1, &value_1_big);
  to_big(num2, &value_2_big);
  normalize_scales(&value_1_big, &value_2_big);
  return compare_big_decimal(&value_1_big, &value_2_big);
}