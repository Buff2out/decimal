// src/utils/utils.h
#ifndef OPERATIONS_H
#define OPERATIONS_H
#include "../src/utils/utils.h"

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
// int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int add(const s21_big_decimal *a, const s21_big_decimal *b, s21_big_decimal *result);

#endif