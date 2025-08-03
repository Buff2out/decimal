// test/test_mul.c
#include "../test/test_add.h"

START_TEST(test_mul_big_simple) {
    // 2 * 3 = 6
    s21_big_decimal a = new_big_native(0, 0, 0, 0, 0, 0, 2, 0);
    s21_big_decimal b = new_big_native(0, 0, 0, 0, 0, 0, 3, 0);
    s21_big_decimal result;
    s21_big_decimal expected = new_big_native(0, 0, 0, 0, 0, 0, 6, 0);
    
    int flag = mul(&a, &b, &result);
    
    ck_assert_int_eq(flag, OK);
    assert_big_decimal_eq(result, expected);
}
END_TEST

START_TEST(test_mul_big_with_carry) {
    // 0xFFFFFFFF * 2 = 0x1FFFFFFFE
    s21_big_decimal a = new_big_native(0, 0, 0, 0, 0, 0, 0xFFFFFFFF, 0);
    s21_big_decimal b = new_big_native(0, 0, 0, 0, 0, 0, 2, 0);
    s21_big_decimal result;
    s21_big_decimal expected = new_big_native(0, 0, 0, 0, 0, 1, 0xFFFFFFFE, 0);
    
    int flag = mul(&a, &b, &result);
    
    ck_assert_int_eq(flag, OK);
    assert_big_decimal_eq(result, expected);
}
END_TEST

START_TEST(test_mul_big_overflow) {
    // 0xFFFFFFFFFFFFFFFF * 2 = переполнение
    s21_big_decimal a = new_big_native(0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0);
    s21_big_decimal b = new_big_native(0, 0, 0, 0, 0, 0, 2, 0);
    s21_big_decimal result = {0};
    
    int flag = mul(&a, &b, &result);
    printf("test_mul_big_overflow: \n");
    print_big_native_hex(&result);

    ck_assert_int_eq(flag, PLUS_INF);
}
END_TEST

START_TEST(test_mul_big_negative) {
    // -5 * 3 = -15
    s21_big_decimal a = new_big_native(0, 0, 0, 0, 0, 0, 5, 0x80000000);
    s21_big_decimal b = new_big_native(0, 0, 0, 0, 0, 0, 3, 0);
    s21_big_decimal result;
    s21_big_decimal expected = new_big_native(0, 0, 0, 0, 0, 0, 15, 0x80000000);
    
    int flag = mul(&a, &b, &result);
    
    ck_assert_int_eq(flag, OK);
    assert_big_decimal_eq(result, expected);
}
END_TEST

START_TEST(test_mul_big_multiple_words) {
    // 0x100000001 * 0x100000001 = 0x10000000200000001
    s21_big_decimal a = new_big_native(0, 0, 0, 0, 0, 1, 1, 0);
    s21_big_decimal b = new_big_native(0, 0, 0, 0, 0, 1, 1, 0);
    s21_big_decimal result;
    s21_big_decimal expected = new_big_native(0, 0, 0, 0, 1, 2, 1, 0);
    
    int flag = mul(&a, &b, &result);
    
    ck_assert_int_eq(flag, OK);
    assert_big_decimal_eq(result, expected);
}
END_TEST

START_TEST(test_mul_big_max_values) {
    // MAX * 1 = MAX
    s21_big_decimal max = new_big_native(0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
                                       0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
                                       0xFFFFFFFF, 0);
    s21_big_decimal one = new_big_native(0, 0, 0, 0, 0, 0, 1, 0);
    s21_big_decimal result;
    
    int flag = mul(&max, &one, &result);
    
    ck_assert_int_eq(flag, OK);
    assert_big_decimal_eq(result, max);
}
END_TEST

Suite* mul_suite(void) {
    Suite *s = suite_create("Big Decimal Multiplication");
    
    TCase *tc_core = tcase_create("Core");
    tcase_add_test(tc_core, test_mul_big_simple);
    tcase_add_test(tc_core, test_mul_big_with_carry);
    tcase_add_test(tc_core, test_mul_big_multiple_words);
    tcase_add_test(tc_core, test_mul_big_max_values);
    
    TCase *tc_errors = tcase_create("Errors");
    tcase_add_test(tc_errors, test_mul_big_overflow);
    
    TCase *tc_sign = tcase_create("Sign");
    tcase_add_test(tc_sign, test_mul_big_negative);
    
    suite_add_tcase(s, tc_core);
    suite_add_tcase(s, tc_errors);
    suite_add_tcase(s, tc_sign);
    
    return s;
}