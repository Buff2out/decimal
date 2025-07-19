// test/test.c
#include <check.h>
#include <stdlib.h>
#include "test.h"

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
    Suite *s = make_suite();
    // Создаем runner
    SRunner *runner = srunner_create(s);
    srunner_add_suite(runner, convert_suite());
    srunner_add_suite(runner, add_suite());

    srunner_run_all(runner, CK_VERBOSE);
    int failed = srunner_ntests_failed(runner);
    srunner_free(runner);

    return (failed) ? 1 : 0;
}