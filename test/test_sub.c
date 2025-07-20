// test/test_sub.h
#include "../test/test_sub.h"

START_TEST(test_sub_simple) {
    s21_big_decimal a = new_big_native(0, 0, 0, 0, 0, 0, 10, 0);  // 10
    s21_big_decimal b = new_big_native(0, 0, 0, 0, 0, 0, 3, 0);    // 3
    s21_big_decimal expected = new_big_native(0, 0, 0, 0, 0, 0, 7, 0); // 7
    s21_big_decimal result = {0};
    
    ck_assert_int_eq(sub(&a, &b, &result), OK);
    assert_big_decimal_eq(result, expected);
}
END_TEST

START_TEST(test_sub_max_minus_one) {
    s21_big_decimal max = new_big_native(0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
                                       0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
                                       0xFFFFFFFF, 0);
    s21_big_decimal one = new_big_native(0, 0, 0, 0, 0, 0, 1, 0); // 1
    s21_big_decimal expected = new_big_native(0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
                                           0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
                                           0xFFFFFFFE, 0);
    s21_big_decimal result = {0};
    
    ck_assert_int_eq(sub(&max, &one, &result), OK);
    assert_big_decimal_eq(result, expected);
}
END_TEST

START_TEST(test_sub_equal_numbers) {
    s21_big_decimal a = new_big_native(0, 0, 0x12345678, 0, 0, 0, 0xABCDEF01, 0);
    s21_big_decimal b = a;
    s21_big_decimal expected = {0};
    s21_big_decimal result = {0};
    
    ck_assert_int_eq(sub(&a, &b, &result), OK);
    assert_big_decimal_eq(result, expected);
}
END_TEST

START_TEST(test_sub_with_borrow) {
    s21_big_decimal a = new_big_native(0, 0, 0, 0, 0, 1, 0, 0);  // 0x100000000
    s21_big_decimal b = new_big_native(0, 0, 0, 0, 0, 0, 1, 0);   // 1
    s21_big_decimal expected = new_big_native(0, 0, 0, 0, 0, 0, 0xFFFFFFFF, 0);
    s21_big_decimal result = {0};
    
    ck_assert_int_eq(sub(&a, &b, &result), OK);
    assert_big_decimal_eq(result, expected);
}
END_TEST

START_TEST(test_sub_null_pointers) {
    s21_big_decimal a = new_big_native(0, 0, 0, 0, 0, 0, 5, 0);
    s21_big_decimal b = new_big_native(0, 0, 0, 0, 0, 0, 3, 0);
    s21_big_decimal result = {0};
    
    ck_assert_int_eq(sub(NULL, &b, &result), ERROR);
    ck_assert_int_eq(sub(&a, NULL, &result), ERROR);
    ck_assert_int_eq(sub(&a, &b, NULL), ERROR);
}
END_TEST

START_TEST(test_sub_a_less_than_b) {
    s21_big_decimal a = new_big_native(0, 0, 0, 0, 0, 0, 3, 0);
    s21_big_decimal b = new_big_native(0, 0, 0, 0, 0, 0, 5, 0);
    s21_big_decimal result = {0};
    
    ck_assert_int_eq(sub(&a, &b, &result), MINUS_INF);
}
END_TEST

Suite* sub_suite(void) {
    Suite *s = suite_create("Big Decimal Subtraction");
    
    TCase *tc_core = tcase_create("Core");
    tcase_add_test(tc_core, test_sub_simple);
    tcase_add_test(tc_core, test_sub_max_minus_one);
    tcase_add_test(tc_core, test_sub_equal_numbers);
    tcase_add_test(tc_core, test_sub_with_borrow);
    
    TCase *tc_errors = tcase_create("Errors");
    tcase_add_test(tc_errors, test_sub_null_pointers);
    tcase_add_test(tc_errors, test_sub_a_less_than_b);
    
    suite_add_tcase(s, tc_core);
    suite_add_tcase(s, tc_errors);
    return s;
}