// test/test.c
#include <check.h>
#include <stdlib.h>
#include "../test/test.h"


// #include "../src/utils/utils.h"
// #include "../src/operations.h"

// Простая проверка двух big_decimal
void assert_big_decimal_eq(s21_big_decimal a, s21_big_decimal b) {
    for (int i = 0; i < 8; i++) {
        ck_assert_uint_eq(a.bits[i], b.bits[i]);
    }
}

// Вспомогательная функция для сравнения decimal
void assert_decimal_eq(s21_decimal a, s21_decimal b) {
    for (int i = 0; i < 4; i++) {
        ck_assert_uint_eq(a.bits[i], b.bits[i]);
    }
}

int main(void) {
    Suite *s = suite_normalize();
    SRunner *runner = srunner_create(s);
    
    srunner_add_suite(runner, sub_suite());
    srunner_add_suite(runner, add_suite());
    srunner_add_suite(runner, suite_shift());
    srunner_add_suite(runner, convert_suite());
    srunner_add_suite(runner, compare_suite());
    srunner_add_suite(runner, s21_is_equal_suite());
    srunner_add_suite(runner, s21_is_greater_or_equal_suite());
    srunner_add_suite(runner, s21_is_greater_suite());
    srunner_add_suite(runner, s21_is_less_or_equal_suite());
    srunner_add_suite(runner, s21_is_less_suite());
    srunner_add_suite(runner, s21_is_not_equal_suite());
    srunner_add_suite(runner, divide_by_10_suite());
    srunner_add_suite(runner, mul_suite());

    srunner_run_all(runner, CK_VERBOSE);
    int failed = srunner_ntests_failed(runner);
    srunner_free(runner);

    return (failed) ? 1 : 0;
}