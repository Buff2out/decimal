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

// Вспомогательная функция для создания big_decimal из массива
s21_big_decimal new_big_native(unsigned b6, unsigned b5, unsigned b4, unsigned b3, 
                               unsigned b2, unsigned b1, unsigned b0, unsigned b7) {
    s21_big_decimal result = {{b0, b1, b2, b3, b4, b5, b6, b7}};
    return result;
}

// Вспомогательная функция для создания big_decimal из массива
s21_big_decimal from_bits_native(unsigned b6, unsigned b5, unsigned b4, 
                                             unsigned b3, unsigned b2, unsigned b1, unsigned b0, unsigned b7) {
    s21_big_decimal result = {{b0, b1, b2, b3, b4, b5, b6, b7}};
    return result;
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

int to_dec(s21_big_decimal *big, s21_decimal *num) {
  const int ok = 0;
  const int null_big = 1;
  const int overflow = 2;

  int flag = ok;
  *num = (s21_decimal){0};

  if (!big) { 
    flag = null_big; 
  } else {
    // Проверяем, поместится ли число в 96 бит мантиссы
    int overflow = 0;
    for (int i = DEC_END + 1; !flag && i <= BIG_END; ++i) {
        if (big->bits[i] != 0) {
            flag = overflow;
        }
    }

    int scale = get_big_scale(big);
    int sign = get_big_sign(big);

    if (!flag) {
      // Помещается — просто копируем
      for (int i = DEC_BEGIN; i <= DEC_END; ++i) {
          num->bits[i] = big->bits[i];
      }
      set_scale(num, scale);
      set_sign(num, sign);
    } else if (scale != 0) {
      // === Банковское округление ===

      s21_big_decimal temp = *big;
      int round_up = 0;

      // Увеличиваем масштаб на 1, чтобы посмотреть на следующий разряд
      multiply_by_10(&temp); // теперь scale увеличен, можно посмотреть на "десятичный" остаток

      // Проверяем, нужно ли округлять
      // Идея: если младшие биты (после масштаба) >= 5 * 10^(scale-1), то округляем вверх
      // Но проще: нормализуем и проверим младшую цифру

      // Упрощённый подход: смотрим, что будет при делении на 10
      // Но мы можем проверить: если младшее слово (после масштабирования) >= 5, округляем

      // Альтернатива: сдвигаем вправо на scale, смотрим последнюю цифру
      // Но проще: попробуем вычесть 5 * 10^(scale-1)

      // Упрощённая версия: если младшие биты >= половины шага масштаба → округляем

      // Уменьшаем масштаб временно и смотрим, что будет при делении
      // Но давай сделаем проще: используем `compare` с "половиной"

      s21_big_decimal half = *big;
      set_big_scale(&half, scale - 1); // 0.5, 0.05 и т.д.
      half = shift_left(half, 1);      // умножаем на 2 → теперь это 1 * 10^(scale-1)
      half = shift_left(half, 2);      // умножаем на 5 → 5 * 10^(scale-1)
      // На самом деле: это сложно. Давай сделаем по-другому.

      // === Простой подход: округляем "вверх", если младшие биты >= 5 * 10^(scale-1)
      // Но мы не можем легко вычислить это. Поэтому:

      // === Практический способ: нормализуем до целого, смотрим последнюю цифру
      while (get_big_scale(big) > 0) {
          divide_by_10(big); // делаем число целым
      }

      // Теперь смотрим последнюю цифру
      unsigned last_digit = big->bits[DEC_BEGIN] % 10;

      if (last_digit > 5) {
          round_up = 1;
      } else if (last_digit == 5) {
          // Банковское правило: к ближайшему чётному
          unsigned before_last = (big->bits[DEC_BEGIN] / 10) % 10;
          if (before_last % 2 == 1) {
              round_up = 1;
          }
      } else {
          round_up = 0;
      }

      if (round_up) {
          s21_big_decimal one = {0};
          one.bits[DEC_BEGIN] = 1;
          set_big_scale(&one, scale);
          add(&temp, &one, &temp); // добавляем 1 * 10^(-scale)
      }

      // Теперь пытаемся сконвертировать округлённое число
      flag = to_dec(&temp, num);
      if (flag == ok) {
        set_sign(num, sign);
      }
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

void divide_by_10(s21_big_decimal *big) {
    if (!big) return;

    unsigned long long remainder = 0;

    for (int i = BIG_BEGIN; i <= BIG_END; ++i) {
        unsigned long long dividend = ((unsigned long long)remainder << 32) | big->bits[i];
        big->bits[i] = (unsigned int)(dividend / 10);
        remainder = dividend % 10;
    }

    // Уменьшаем масштаб
    int scale = get_big_scale(big);
    if (scale > 0) {
        set_big_scale(big, scale - 1);
    }
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