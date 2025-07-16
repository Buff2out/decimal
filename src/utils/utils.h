#ifndef UTILS_H
#define UTILS_H
#include "../s21_decimal.h"

int is_zero(const s21_decimal* num);
int is_big_zero(const s21_big_decimal* big);
void reset_decimal(s21_decimal *num);
void reset_big_decimal(s21_big_decimal *big);
int get_sign(const s21_decimal* num);
int get_big_sign(const s21_big_decimal* big);
int get_scale(const s21_decimal* num);
int get_big_scale(const s21_big_decimal* big);
int get_bit(const s21_decimal* num, unsigned int pos);
int get_big_bit(const s21_big_decimal *big, unsigned int pos);
int set_sign(s21_decimal *num, unsigned int sign_value);
int set_big_sign(s21_big_decimal *big, const unsigned int sign_value);
int set_scale(s21_decimal *num, const unsigned int scale_value);
int set_big_scale(s21_big_decimal *big, const unsigned int scale_value);
int set_bit(s21_decimal *num, const unsigned int val, const unsigned int pos);
int set_big_bit(s21_big_decimal *num, const unsigned int val, const unsigned int pos);
s21_big_decimal shift_left(s21_big_decimal big, unsigned shift_value);
void convert_to_big_decimal(const s21_decimal *num, s21_big_decimal *big);
void print_binary(int num);
void print_decimal(const s21_decimal* num);
void print_big_decimal(const s21_big_decimal* num);
void normalize_scales(s21_big_decimal *num1, s21_big_decimal *num2);
void multiply_by_10(s21_big_decimal *big);
int compare_big_decimal(s21_big_decimal num1, s21_big_decimal num2);
int compare_decimal(s21_decimal num1, s21_decimal num2);
#endif