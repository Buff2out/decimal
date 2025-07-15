#include <check.h>
#include <stdint.h>

#include "../src/utils/utils.h"

// Вспомогательная функция для создания big_decimal из массива
s21_big_decimal from_bits(unsigned int b0, unsigned int b1, unsigned int b2,
                          unsigned int b3, unsigned int b4, unsigned int b5,
                          unsigned int b6, unsigned int b7) {
    s21_big_decimal result = {{b0, b1, b2, b3, b4, b5, b6, b7}};
    return result;
}

// Простая проверка двух big_decimal
void assert_big_decimal_eq(s21_big_decimal a, s21_big_decimal b) {
    for (int i = 0; i < 8; i++) {
        ck_assert_uint_eq(a.bits[i], b.bits[i]);
    }
}

START_TEST(test_shift_left_one_bit) {
    s21_big_decimal input = from_bits(1, 0, 0, 0, 0, 0, 0, 0); // число 1
    s21_big_decimal expected = from_bits(2, 0, 0, 0, 0, 0, 0, 0);

    s21_big_decimal result = shift_left(input, 1);
    assert_big_decimal_eq(result, expected);
}
END_TEST

START_TEST(test_shift_left_three_bits) {
    s21_big_decimal input = from_bits(1, 0, 0, 0, 0, 0, 0, 0); // 1
    s21_big_decimal expected = from_bits(8, 0, 0, 0, 0, 0, 0, 0); // 8

    s21_big_decimal result = shift_left(input, 3);
    assert_big_decimal_eq(result, expected);
}
END_TEST

START_TEST(test_shift_left_with_carry_between_words) {
    s21_big_decimal input = from_bits(0xFFFFFFFF, 0, 0, 0, 0, 0, 0, 0); // 32 единичных бита
    s21_big_decimal expected = from_bits(0xFFFFFFFE, 0, 0, 0, 0, 0, 0, 0); // << 1

    s21_big_decimal result = shift_left(input, 1);
    assert_big_decimal_eq(result, expected);
}
END_TEST

START_TEST(test_shift_left_does_not_touch_meta) {
    s21_big_decimal input = from_bits(1, 0, 0, 0, 0, 0, 0, 0xAABBCCDD);

    s21_big_decimal result = shift_left(input, 1);
    ck_assert_uint_eq(result.bits[7], 0xAABBCCDD); // bits[7] не должен меняться
}
END_TEST

Suite *make_suite(void) {
    Suite *s = suite_create("Big Decimal Shift Left");

    TCase *tc_core = tcase_create("Core");
    tcase_add_test(tc_core, test_shift_left_one_bit);
    tcase_add_test(tc_core, test_shift_left_three_bits);
    tcase_add_test(tc_core, test_shift_left_with_carry_between_words);
    tcase_add_test(tc_core, test_shift_left_does_not_touch_meta);

    suite_add_tcase(s, tc_core);
    return s;
}

int main(void) {
    Suite *s = make_suite();
    SRunner *runner = srunner_create(s);

    srunner_run_all(runner, CK_VERBOSE);
    int failed = srunner_ntests_failed(runner);
    srunner_free(runner);

    return (failed) ? 1 : 0;
}