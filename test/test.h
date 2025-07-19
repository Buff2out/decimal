// test/test.h
#ifndef TEST_H
#define TEST_H

#include <check.h>
#include "../test/test_utils.h"
#include "../test/test_add.h"

void assert_big_decimal_eq(s21_big_decimal a, s21_big_decimal b);
void assert_decimal_eq(s21_decimal a, s21_decimal b);

// Suite *convert_suite(void);
// Suite *make_suite(void);
// Suite *add_suite(void);
// void assert_big_decimal_eq(s21_big_decimal a, s21_big_decimal b);
// void assert_decimal_eq(s21_decimal a, s21_decimal b);

#endif