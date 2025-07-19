// test/test_add.c
#include "../test/test_add.h"

START_TEST(test_add_positive_numbers) {
    s21_big_decimal a = new_big_native(0, 0, 0, 0, 0, 0, 5, 0);  // 5
    s21_big_decimal b = new_big_native(0, 0, 0, 0, 0, 0, 3, 0);  // 3
    s21_big_decimal expected = new_big_native(0, 0, 0, 0, 0, 0, 8, 0); // 8
    s21_big_decimal result = {0};
    
    int flag = add(&a, &b, &result);
    
    ck_assert_int_eq(flag, 0);
    assert_big_decimal_eq(result, expected);
}
END_TEST

START_TEST(test_add_negative_numbers) {
    s21_big_decimal a = new_big_native(0, 0, 0, 0, 0, 0, 5, 0x80000000);  // -5
    s21_big_decimal b = new_big_native(0, 0, 0, 0, 0, 0, 3, 0x80000000);  // -3
    s21_big_decimal expected = new_big_native(0, 0, 0, 0, 0, 0, 8, 0x80000000); // -8
    s21_big_decimal result = {0};
    
    int flag = add(&a, &b, &result);
    
    ck_assert_int_eq(flag, 0);
    assert_big_decimal_eq(result, expected);
}
END_TEST

START_TEST(test_add_overflow) {
    s21_big_decimal a = new_big_native(0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0, 0, 0, 0, 0);  // Максимальное число
    s21_big_decimal b = new_big_native(0, 0, 0, 0, 0, 0, 1, 0);  // +1
    s21_big_decimal result = {0};
    
    int flag = add(&a, &b, &result);
    
    ck_assert_int_eq(flag, 1);  // Ожидаем флаг переполнения
}
END_TEST

START_TEST(test_add_zero) {
    s21_big_decimal a = new_big_native(0, 0, 0, 0, 0, 0, 5, 0);  // 5
    s21_big_decimal b = new_big_native(0, 0, 0, 0, 0, 0, 0, 0);  // 0
    s21_big_decimal expected = new_big_native(0, 0, 0, 0, 0, 0, 5, 0); // 5
    s21_big_decimal result = {0};
    
    int flag = add(&a, &b, &result);
    
    ck_assert_int_eq(flag, 0);
    assert_big_decimal_eq(result, expected);
}
END_TEST

Suite* add_suite(void) {
    Suite *s = suite_create("Add Suite");
    
    TCase *tc_core = tcase_create("Core");
    tcase_add_test(tc_core, test_add_positive_numbers);
    tcase_add_test(tc_core, test_add_negative_numbers);
    tcase_add_test(tc_core, test_add_overflow);
    tcase_add_test(tc_core, test_add_zero);
    
    suite_add_tcase(s, tc_core);
    return s;
}