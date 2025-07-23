// test/test_s21_is_not_equal.c
#include "../test/test_s21_is_not_equal.h"

START_TEST(test_s21_is_not_equal_two_random_numbers_1) {
    s21_decimal a = new_dec_native(0, 0, 0xFACE, 0x001C0000);
    s21_decimal b = new_dec_native(0, 0, 0xBAD, 0x001A0000);
    int expected = 1;
    
    int flag = s21_is_not_equal(a, b);
    
    ck_assert_int_eq(flag, expected);
}
END_TEST

START_TEST(test_s21_is_not_equal_two_random_numbers_2) {
    s21_decimal a = new_dec_native(0xFACE, 0, 0xFACE, 0x001C0000);
    s21_decimal b = new_dec_native(0, 0xBAD, 0xBAD, 0);
    int expected = 1;
    
    int flag = s21_is_not_equal(a, b);
    
    ck_assert_int_eq(flag, expected);
}
END_TEST

START_TEST(test_s21_is_not_equal_two_random_equal_numbers_1) {
    s21_decimal a = new_dec_native(0, 0xBAD, 0xBAD, 0x001C0000);
    s21_decimal b = new_dec_native(0, 0xBAD, 0xBAD, 0x001C0000);
    int expected = 0;
    
    int flag = s21_is_not_equal(a, b);
    ck_assert_int_eq(flag, expected);
}
END_TEST

START_TEST(test_s21_is_not_equal_two_random_equal_numbers_2) {
    s21_decimal a = new_dec_native(0xFACE, 0, 0xFACE, 0);
    s21_decimal b = new_dec_native(0xFACE, 0, 0xFACE, 0);
    int expected = 0;
    
    int flag = s21_is_not_equal(a, b);
    
    ck_assert_int_eq(flag, expected);
}
END_TEST

START_TEST(test_s21_is_not_equal_two_zeros_diff_signs) {
    s21_decimal a = new_dec_native(0, 0, 0, 0x80000000);
    s21_decimal b = new_dec_native(0, 0, 0, 0);
    int expected = 0;
    
    int flag = s21_is_not_equal(a, b);
    
    ck_assert_int_eq(flag, expected);
}
END_TEST

START_TEST(test_s21_is_not_equal_two_zeros_with_positive_signs) {
    s21_decimal a = new_dec_native(0, 0, 0, 0);
    s21_decimal b = new_dec_native(0, 0, 0, 0);
    int expected = 0;
    
    int flag = s21_is_not_equal(a, b);
    
    ck_assert_int_eq(flag, expected);
}
END_TEST

START_TEST(test_s21_is_not_equal_two_zeros_with_negative_signs) {
    s21_decimal a = new_dec_native(0, 0, 0, 0x8000000);
    s21_decimal b = new_dec_native(0, 0, 0, 0x8000000);
    int expected = 0;
    
    int flag = s21_is_not_equal(a, b);
    
    ck_assert_int_eq(flag, expected);
}
END_TEST

START_TEST(test_s21_is_not_equal_two_numbers_with_equal_mantissas_diff_signs) {
    s21_decimal a = new_dec_native(0xFFFDEFFF, 0, 0xFFFDEFFF, 0x80000000);
    s21_decimal b = new_dec_native(0xFFFDEFFF, 0, 0xFFFDEFFF, 0);
    int expected = 1;
    
    int flag = s21_is_not_equal(a, b);
    
    ck_assert_int_eq(flag, expected);
}
END_TEST

START_TEST(test_s21_is_not_equal_two_numbers_with_equal_mantissas_diff_scales) {
    s21_decimal a = new_dec_native(0xFFFDEFFF, 0xFFFDEFFF, 0xFFFDEFFF, 0x001C0000);
    s21_decimal b = new_dec_native(0xFFFDEFFF, 0xFFFDEFFF, 0xFFFDEFFF, 0x001B0000);
    int expected = 1;
    
    int flag = s21_is_not_equal(a, b);
    
    ck_assert_int_eq(flag, expected);
}
END_TEST

Suite* s21_is_not_equal_suite(void) {
    Suite *s = suite_create("s21_is_not_equal Suite");
    
    TCase *tc_core = tcase_create("Core");
    tcase_add_test(tc_core, test_s21_is_not_equal_two_random_numbers_1);
    tcase_add_test(tc_core, test_s21_is_not_equal_two_random_numbers_2);
    tcase_add_test(tc_core, test_s21_is_not_equal_two_random_equal_numbers_1);
    tcase_add_test(tc_core, test_s21_is_not_equal_two_random_equal_numbers_2);
    tcase_add_test(tc_core, test_s21_is_not_equal_two_zeros_diff_signs);
    tcase_add_test(tc_core, test_s21_is_not_equal_two_zeros_with_positive_signs);
    tcase_add_test(tc_core, test_s21_is_not_equal_two_zeros_with_negative_signs);
    tcase_add_test(tc_core, test_s21_is_not_equal_two_numbers_with_equal_mantissas_diff_signs);
    tcase_add_test(tc_core, test_s21_is_not_equal_two_numbers_with_equal_mantissas_diff_scales);
    
    suite_add_tcase(s, tc_core);
    return s;
}