// test/test.h
#ifndef TEST_H
#define TEST_H

#include <check.h>
#include "../test/test_utils.h"
#include "../test/test_add.h"
#include "../test/test_sub.h"
#include "../test/test_s21_is_equal.h"
#include "../test/test_s21_is_greater_or_equal.h"
#include "../test/test_s21_is_greater.h"
#include "../test/test_s21_is_less_or_equal.h"
#include "../test/test_s21_is_less.h"
#include "../test/test_s21_is_not_equal.h"
void assert_big_decimal_eq(s21_big_decimal a, s21_big_decimal b);
void assert_decimal_eq(s21_decimal a, s21_decimal b);

#endif