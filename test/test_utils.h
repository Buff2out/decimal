// test_utils.h
#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <check.h>
#include "../src/utils/utils.h"

Suite *convert_suite(void);
Suite *make_suite(void);
void assert_big_decimal_eq(s21_big_decimal a, s21_big_decimal b);
void assert_decimal_eq(s21_decimal a, s21_decimal b);

#endif