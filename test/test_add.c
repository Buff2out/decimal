// test/test_add.c
#include "../test/test_add.h"

START_TEST(test_add_phrases_0) {
    s21_big_decimal a = new_big_native(0, 0xA, 0xFAB, 0xACE, 0xDEB, 0xFED, 0xBEEF, 0);
    s21_big_decimal b = new_big_native(0, 0xBAD, 0xFACE, 0xFED, 0xA, 0xFAB, 0xCAFE, 0);
    s21_big_decimal expected = new_big_native(0, 0xBB7, 0x10A79, 0x1ABB, 0xDF5, 0x1F98, 0x189ED, 0);
    s21_big_decimal result = {0};
    
    int flag = add(&a, &b, &result);
    
    ck_assert_int_eq(flag, 0);
    assert_big_decimal_eq(result, expected);
}
END_TEST

START_TEST(test_add_positive_numbers) {
    s21_big_decimal a = new_big_native(0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 5, 0x00000000);  // 5
    s21_big_decimal b = new_big_native(0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 3, 0x00000000);  // 3
    s21_big_decimal expected = new_big_native(0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 8, 0x00000000); // 8
    s21_big_decimal result = {0};
    
    int flag = add(&a, &b, &result);
    
    ck_assert_int_eq(flag, 0);
    assert_big_decimal_eq(result, expected);
}
END_TEST

START_TEST(test_add_negative_numbers) {
    s21_big_decimal a = new_big_native(0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 5, 0x80000000);  // -5
    s21_big_decimal b = new_big_native(0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 3, 0x80000000);  // -3
    s21_big_decimal expected = new_big_native(0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 8, 0x80000000); // -8
    s21_big_decimal result = {0};
    
    int flag = add(&a, &b, &result);
    
    ck_assert_int_eq(flag, 0);
    assert_big_decimal_eq(result, expected);
}
END_TEST

START_TEST(test_add_huge_num_0) {
    s21_big_decimal a = new_big_native(0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000);
    s21_big_decimal b = new_big_native(0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xFFFFFFAF, 0xFFFFFFFA, 0x00000000);
    s21_big_decimal expected = new_big_native(0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, 0x00000000, 0xFFFFFFAF, 0xFFFFFFFA, 0x00000000);
    s21_big_decimal result = {0};
    
    int flag = add(&a, &b, &result);
    
    ck_assert_int_eq(flag, 0);
    assert_big_decimal_eq(result, expected);
}
END_TEST

START_TEST(test_add_huge_num_1) {
    s21_big_decimal a = new_big_native(0, 0, 0, 0, 0x00000000, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000);
    s21_big_decimal b = new_big_native(0, 0, 0, 0, 0x00000000, 0x00000000, 0x00000001, 0x00000000);  // +1
    s21_big_decimal expected = new_big_native(0, 0, 0, 0, 1, 0, 0, 0);
    s21_big_decimal result = {0};
    
    int flag = add(&a, &b, &result);
    
    ck_assert_int_eq(flag, 0);
    assert_big_decimal_eq(result, expected);
}
END_TEST

START_TEST(test_add_huge_num_2) {
    s21_big_decimal a = new_big_native(0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xFFFFFFFF, 0x00000000);
    s21_big_decimal b = new_big_native(0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xABCDEFFF, 0x00000000);
    s21_big_decimal expected = new_big_native(0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 1, 0xABCDEFFE, 0x00000000);
    s21_big_decimal result = {0};
    
    int flag = add(&a, &b, &result);
    
    ck_assert_int_eq(flag, 0);
    assert_big_decimal_eq(result, expected);
}
END_TEST

START_TEST(test_add_huge_num_3) {
    s21_big_decimal a = new_big_native(0, 0, 0, 0, 0x00000000, 0xFFFFFFFF, 0xABCDEFFF, 0x00000000);
    s21_big_decimal b = new_big_native(0, 0, 0, 0, 0x00000000, 0xFFFDEFFF, 0xABCDEFFF, 0x00000000);
    s21_big_decimal expected = new_big_native(0, 0, 0, 0, 1, 0xFFFDEFFF, 0x579BDFFE, 0x00000000);
    s21_big_decimal result = {0};
    
    int flag = add(&a, &b, &result);
    
    ck_assert_int_eq(flag, 0);
    assert_big_decimal_eq(result, expected);
}
END_TEST

START_TEST(test_add_huge_num_4) {
    s21_big_decimal a = new_big_native(0, 0, 0, 0, 0x00000000, 0xFFFFFFFF, 0xABCDEFFF, 0x00000000);
    s21_big_decimal b = new_big_native(0, 0, 0, 0, 0xFFFDEFFF, 0xABCDEFFF, 0xCCCCCCCC, 0x00000000);
    s21_big_decimal expected = new_big_native(0, 0, 0, 0, 0xFFFDF000, 0xABCDEFFF, 0x789ABCCB, 0x00000000);
    s21_big_decimal result = {0};
    
    int flag = add(&a, &b, &result);
    
    ck_assert_int_eq(flag, 0);
    assert_big_decimal_eq(result, expected);
}
END_TEST

START_TEST(test_add_huge_num_5) {
    s21_big_decimal a = new_big_native(0, 0, 0x00000000, 0x00000000, 0x00000000, 0xFFFFFFFF, 0xABCDEFFF, 0x00000000);
    s21_big_decimal b = new_big_native(0, 0, 0x00000000, 0xFFFDEFFF, 0xABCDEFFF, 0xCCCCCCCC, 0xEEEEEEEE, 0x00000000);
    s21_big_decimal expected = new_big_native(0, 0, 0x00000000, 0xFFFDEFFF, 0xABCDF000, 0xCCCCCCCC, 0x9ABCDEED, 0x00000000);
    s21_big_decimal result = {0};
    
    int flag = add(&a, &b, &result);
    
    ck_assert_int_eq(flag, 0);
    assert_big_decimal_eq(result, expected);
}
END_TEST

START_TEST(test_add_overflow) {
    s21_big_decimal a = new_big_native(0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000);  // Максимальное число
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
    tcase_add_test(tc_core, test_add_phrases_0);
    tcase_add_test(tc_core, test_add_huge_num_0);
    tcase_add_test(tc_core, test_add_huge_num_1);
    tcase_add_test(tc_core, test_add_huge_num_2);
    tcase_add_test(tc_core, test_add_huge_num_3);
    tcase_add_test(tc_core, test_add_huge_num_4);
    tcase_add_test(tc_core, test_add_huge_num_5);
    tcase_add_test(tc_core, test_add_zero);
    
    suite_add_tcase(s, tc_core);
    return s;
}