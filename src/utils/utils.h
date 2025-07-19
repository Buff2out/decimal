// src/utils/utils.h
#ifndef UTILS_H
#define UTILS_H
#include "../s21_decimal.h"

s21_big_decimal from_bits(unsigned b0, unsigned b1, unsigned b2,
                          unsigned b3, unsigned b4, unsigned b5,
                          unsigned b6, unsigned b7);
s21_big_decimal from_bits_native(unsigned b6, unsigned b5, unsigned b4, unsigned b3, unsigned b2, unsigned b1, unsigned b0, unsigned b7);
s21_decimal new_dec_native(unsigned b2, unsigned b1, unsigned b0, unsigned b3);
s21_big_decimal new_big_native(unsigned b6, unsigned b5, unsigned b4, unsigned b3, unsigned b2, unsigned b1, unsigned b0, unsigned b7);
int is_zero(const s21_decimal* num);
int is_big_zero(const s21_big_decimal* big);
void reset_decimal(s21_decimal *num);
void reset_big_decimal(s21_big_decimal *big);
int get_sign(const s21_decimal* num);
int get_big_sign(const s21_big_decimal* big);
int get_scale(const s21_decimal* num);
int get_big_scale(const s21_big_decimal* big);
int get_bit(const s21_decimal* num, unsigned pos);
int get_big_bit(const s21_big_decimal *big, unsigned pos);
int set_sign(s21_decimal *num, unsigned sign_value);
int set_big_sign(s21_big_decimal *big, const unsigned sign_value);
int set_scale(s21_decimal *num, const unsigned scale_value);
int set_big_scale(s21_big_decimal *big, const unsigned scale_value);
int set_bit(s21_decimal *num, const unsigned val, const unsigned pos);
int set_big_bit(s21_big_decimal *num, const unsigned val, const unsigned pos);
s21_big_decimal shift_left(s21_big_decimal big, unsigned shift_value);
void convert_to_big_decimal(const s21_decimal *num, s21_big_decimal *big);
int to_big(const s21_decimal *num, s21_big_decimal *big);
int to_dec(const s21_big_decimal *big, s21_decimal *num);
void print_binary(unsigned num);
void print_dec(const s21_decimal* num);
void print_dec_native(const s21_decimal* num);
void print_big(const s21_big_decimal* num);
void print_big_native(const s21_big_decimal* num);
void normalize_scales(s21_big_decimal *num1, s21_big_decimal *num2);
void multiply_by_10(s21_big_decimal *big);
int compare_big_decimal(const s21_big_decimal* big_1, const s21_big_decimal* big_2);
int compare_decimal(const s21_decimal* num1, const s21_decimal* num2);
#endif