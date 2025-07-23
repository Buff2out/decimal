// src/s21_decimal.h
#ifndef S21_DECIMAL_H
#define S21_DECIMAL_H

#define DEC_BEGIN 0
#define BIG_BEGIN 0
#define BIG_METAINFO 7
#define DEC_METAINFO 3

#define DEC_END 2
#define BIG_END 6
#define BIG_MAX_POS 223
#define DEC_MAX_POS 95
#define TRUE 1
#define FALSE 0

#include <stdio.h>

typedef struct {
  unsigned int bits[4];
} s21_decimal;

typedef struct {
  unsigned int bits[8];
} s21_big_decimal;

int s21_is_less(s21_decimal num1, s21_decimal num2);
int s21_is_less_or_equal(s21_decimal num1, s21_decimal num2);
int s21_is_greater(s21_decimal num1, s21_decimal num2);
int s21_is_greater_or_equal(s21_decimal num1, s21_decimal num2);
int s21_is_equal(s21_decimal num1, s21_decimal num2);
int s21_is_not_equal(s21_decimal num1, s21_decimal num2);
#endif