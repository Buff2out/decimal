// src/utils/utils.h
#ifndef OPERATIONS_H
#define OPERATIONS_H
#include "../src/utils/utils.h"

#define OK 0
#define PLUS_INF 1
#define MINUS_INF 2
#define DIV_BY_ZERO 3
#define ERROR 4

int add(const s21_big_decimal *a, const s21_big_decimal *b, s21_big_decimal *result);
int sub(const s21_big_decimal *a, const s21_big_decimal *b, s21_big_decimal *result);
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

#endif