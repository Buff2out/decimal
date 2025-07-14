#ifndef UTILS_H
#define UTILS_H
#include "../s21_decimal.h"

int is_zero(s21_decimal num);
int is_big_zero(s21_big_decimal num);
void reset_decimal(s21_decimal *num);
int get_sign(s21_decimal num);
void set_sign(s21_decimal *num, unsigned int sign_value);
int get_scale(s21_decimal num);
void set_scale(s21_decimal *num, unsigned int scale_value);
int get_bit(s21_decimal num, unsigned int shift);
void set_bit(s21_decimal *num, unsigned int bit_value, unsigned int shift);
void reset_big_decimal(s21_big_decimal *num);
int get_big_sign(s21_big_decimal num);
void set_big_sign(s21_big_decimal *num, unsigned int sign_value);
int get_big_scale(s21_big_decimal num);
void set_big_scale(s21_big_decimal *num, unsigned int scale_value);
int get_big_bit(s21_big_decimal num, unsigned int shift);
void set_big_bit(s21_big_decimal *num, unsigned int bit_value,
                 unsigned int shift);
void convert_to_big_decimal(s21_decimal num, s21_big_decimal *big);
void print_binary(int num);
void print_decimal(s21_decimal num);
void print_big_decimal(s21_big_decimal num);
void normalize_scales(s21_big_decimal *num1, s21_big_decimal *num2);
void multiply_by_10(s21_big_decimal *num1, s21_big_decimal *num2);
s21_big_decimal shift_left(s21_big_decimal num, int shift_value);
int compare_big_decimal(s21_big_decimal num1, s21_big_decimal num2);
int compare_decimal(s21_decimal num1, s21_decimal num2);
#endif