// test/test_sub.h
#include "../test/test_sub.h"

START_TEST(test_sub_phrases_0) {
    s21_big_decimal a = new_big_native(0, 0xBB7, 0x10A79, 0x1ABB, 0xDF5, 0x1F98, 0x189ED, 0);
    s21_big_decimal b = new_big_native(0, 0xA, 0xFAB, 0xACE, 0xDEB, 0xFED, 0xBEEF, 0);
    s21_big_decimal expected = new_big_native(0, 0xBAD, 0xFACE, 0xFED, 0xA, 0xFAB, 0xCAFE, 0);
    s21_big_decimal result = {0};
    
    int flag = sub(&a, &b, &result);
    
    ck_assert_int_eq(flag, 0);
    assert_big_decimal_eq(result, expected);
}
END_TEST

START_TEST(s21_test_sub_phrases_0) {
    s21_decimal a = new_dec_native(0xDF5, 0x1F98, 0x189ED, 0);
    s21_decimal b = new_dec_native(0xDEB, 0xFED, 0xBEEF, 0);
    s21_decimal expected = new_dec_native(0xA, 0xFAB, 0xCAFE, 0);
    s21_decimal result = {0};
    
    int flag = s21_sub(a, b, &result);
    
    ck_assert_int_eq(flag, 0);
    assert_decimal_eq(result, expected);
}
END_TEST

START_TEST(test_sub_huge_num_0) {
    s21_big_decimal a = new_big_native(0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, 0x00000000, 0xFFFFFFAF, 0xFFFFFFFA, 0x00000000);
    s21_big_decimal b = new_big_native(0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000);
    s21_big_decimal expected = new_big_native(0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xFFFFFFAF, 0xFFFFFFFA, 0x00000000);
    s21_big_decimal result = {0};
    
    int flag = sub(&a, &b, &result);
    
    ck_assert_int_eq(flag, 0);
    assert_big_decimal_eq(result, expected);
}
END_TEST

START_TEST(s21_test_sub_huge_num_0) {
    s21_decimal a = new_dec_native(0x00000000, 0xFFFFFFAF, 0xFFFFFFFA, 0x00000000);
    s21_decimal b = new_dec_native(0x00000000, 0x00000000, 0x00000000, 0x00000000);
    s21_decimal expected = new_dec_native(0x00000000, 0xFFFFFFAF, 0xFFFFFFFA, 0x00000000);
    s21_decimal result = {0};
    
    int flag = s21_sub(a, b, &result);
    
    ck_assert_int_eq(flag, 0);
    assert_decimal_eq(result, expected);
}
END_TEST

START_TEST(test_sub_huge_num_1) {
    s21_big_decimal a = new_big_native(0, 0, 0, 0, 1, 0, 0, 0);
    s21_big_decimal b = new_big_native(0, 0, 0, 0, 0x00000000, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000);
    s21_big_decimal expected = new_big_native(0, 0, 0, 0, 0x00000000, 0x00000000, 0x00000001, 0x00000000);
    s21_big_decimal result = {0};
    
    int flag = sub(&a, &b, &result);
    
    ck_assert_int_eq(flag, 0);
    assert_big_decimal_eq(result, expected);
}
END_TEST

START_TEST(s21_test_sub_huge_num_1) {
    s21_decimal a = new_dec_native(1, 0, 0, 0);
    s21_decimal b = new_dec_native(0x00000000, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000);
    s21_decimal expected = new_dec_native(0x00000000, 0x00000000, 0x00000001, 0x00000000);
    s21_decimal result = {0};
    
    int flag = s21_sub(a, b, &result);
    
    ck_assert_int_eq(flag, 0);
    assert_decimal_eq(result, expected);
}
END_TEST

START_TEST(test_sub_huge_num_2) {
    s21_big_decimal a = new_big_native(0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 1, 0xABCDEFFE, 0x00000000);
    s21_big_decimal b = new_big_native(0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xFFFFFFFF, 0x00000000);
    s21_big_decimal expected = new_big_native(0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xABCDEFFF, 0x00000000);
    s21_big_decimal result = {0};
    
    int flag = sub(&a, &b, &result);
    
    ck_assert_int_eq(flag, 0);
    assert_big_decimal_eq(result, expected);
}
END_TEST

START_TEST(s21_test_sub_huge_num_2) {
    s21_decimal a = new_dec_native(0x00000000, 1, 0xABCDEFFE, 0x00000000);
    s21_decimal b = new_dec_native(0x00000000, 0x00000000, 0xFFFFFFFF, 0x00000000);
    s21_decimal expected = new_dec_native(0x00000000, 0x00000000, 0xABCDEFFF, 0x00000000);
    s21_decimal result = {0};
    
    int flag = s21_sub(a, b, &result);
    
    ck_assert_int_eq(flag, 0);
    assert_decimal_eq(result, expected);
}
END_TEST

START_TEST(test_sub_huge_num_3) {
    s21_big_decimal a = new_big_native(0, 0, 0, 0, 1, 0xFFFDEFFF, 0x579BDFFE, 0x00000000);
    s21_big_decimal b = new_big_native(0, 0, 0, 0, 0x00000000, 0xFFFFFFFF, 0xABCDEFFF, 0x00000000);
    s21_big_decimal expected = new_big_native(0, 0, 0, 0, 0x00000000, 0xFFFDEFFF, 0xABCDEFFF, 0x00000000);
    s21_big_decimal result = {0};
    
    int flag = sub(&a, &b, &result);
    
    ck_assert_int_eq(flag, 0);
    assert_big_decimal_eq(result, expected);
}
END_TEST

START_TEST(s21_test_sub_huge_num_3) {
    s21_decimal a = new_dec_native(1, 0xFFFDEFFF, 0x579BDFFE, 0x00000000);
    s21_decimal b = new_dec_native(0x00000000, 0xFFFFFFFF, 0xABCDEFFF, 0x00000000);
    s21_decimal expected = new_dec_native(0x00000000, 0xFFFDEFFF, 0xABCDEFFF, 0x00000000);
    s21_decimal result = {0};
    
    int flag = s21_sub(a, b, &result);
    
    ck_assert_int_eq(flag, 0);
    assert_decimal_eq(result, expected);
}
END_TEST

START_TEST(test_sub_huge_num_4) {
    s21_big_decimal a = new_big_native(0, 0, 0, 0, 0xFFFDF000, 0xABCDEFFF, 0x789ABCCB, 0x00000000);
    s21_big_decimal b = new_big_native(0, 0, 0, 0, 0x00000000, 0xFFFFFFFF, 0xABCDEFFF, 0x00000000);
    s21_big_decimal expected = new_big_native(0, 0, 0, 0, 0xFFFDEFFF, 0xABCDEFFF, 0xCCCCCCCC, 0x00000000);
    s21_big_decimal result = {0};
    
    int flag = sub(&a, &b, &result);
    
    ck_assert_int_eq(flag, 0);
    assert_big_decimal_eq(result, expected);
}
END_TEST

START_TEST(s21_test_sub_huge_num_4) {
    s21_decimal a = new_dec_native(0xFFFDF000, 0xABCDEFFF, 0x789ABCCB, 0x00000000);
    s21_decimal b = new_dec_native(0x00000000, 0xFFFFFFFF, 0xABCDEFFF, 0x00000000);
    s21_decimal expected = new_dec_native(0xFFFDEFFF, 0xABCDEFFF, 0xCCCCCCCC, 0x00000000);
    s21_decimal result = {0};
    
    int flag = s21_sub(a, b, &result);
    
    ck_assert_int_eq(flag, 0);
    assert_decimal_eq(result, expected);
}
END_TEST

START_TEST(test_sub_huge_num_5) {
    s21_big_decimal a = new_big_native(0, 0, 0x00000000, 0xFFFDEFFF, 0xABCDF000, 0xCCCCCCCC, 0x9ABCDEED, 0x00000000);
    s21_big_decimal b = new_big_native(0, 0, 0x00000000, 0x00000000, 0x00000000, 0xFFFFFFFF, 0xABCDEFFF, 0x00000000);
    s21_big_decimal expected = new_big_native(0, 0, 0x00000000, 0xFFFDEFFF, 0xABCDEFFF, 0xCCCCCCCC, 0xEEEEEEEE, 0x00000000);
    s21_big_decimal result = {0};
    
    int flag = sub(&a, &b, &result);
    
    ck_assert_int_eq(flag, 0);
    assert_big_decimal_eq(result, expected);
}
END_TEST

START_TEST(s21_test_sub_huge_num_5) {
    s21_decimal a = new_dec_native(0xABCDF000, 0xCCCCCCCC, 0x9ABCDEED, 0x00000000);
    s21_decimal b = new_dec_native(0x00000000, 0xFFFFFFFF, 0xABCDEFFF, 0x00000000);
    s21_decimal expected = new_dec_native(0xABCDEFFF, 0xCCCCCCCC, 0xEEEEEEEE, 0x00000000);
    s21_decimal result = {0};
    
    int flag = s21_sub(a, b, &result);
    
    ck_assert_int_eq(flag, 0);
    assert_decimal_eq(result, expected);
}
END_TEST

START_TEST(test_sub_zero) {
    s21_big_decimal a = new_big_native(0, 0, 0, 0, 0, 0, 5, 0);  // 5
    s21_big_decimal b = new_big_native(0, 0, 0, 0, 0, 0, 0, 0);  // 0
    s21_big_decimal expected = new_big_native(0, 0, 0, 0, 0, 0, 5, 0); // 5
    s21_big_decimal result = {0};
    
    int flag = sub(&a, &b, &result);
    
    ck_assert_int_eq(flag, 0);
    assert_big_decimal_eq(result, expected);
}
END_TEST

START_TEST(s21_test_sub_zero) {
    s21_decimal a = new_dec_native(0, 0, 5, 0);  // 5
    s21_decimal b = new_dec_native(0, 0, 0, 0);  // 0
    s21_decimal expected = new_dec_native(0, 0, 5, 0); // 5
    s21_decimal result = {0};
    
    int flag = s21_sub(a, b, &result);
    
    ck_assert_int_eq(flag, 0);
    assert_decimal_eq(result, expected);
}
END_TEST

START_TEST(test_sub_simple) {
    s21_big_decimal a = new_big_native(0, 0, 0, 0, 0, 0, 10, 0);  // 10
    s21_big_decimal b = new_big_native(0, 0, 0, 0, 0, 0, 3, 0);    // 3
    s21_big_decimal expected = new_big_native(0, 0, 0, 0, 0, 0, 7, 0); // 7
    s21_big_decimal result = {0};
    
    ck_assert_int_eq(sub(&a, &b, &result), OK);
    assert_big_decimal_eq(result, expected);
}
END_TEST

START_TEST(s21_test_sub_simple) {
    s21_decimal a = new_dec_native(0, 0, 10, 0);  // 10
    s21_decimal b = new_dec_native(0, 0, 3, 0);    // 3
    s21_decimal expected = new_dec_native(0, 0, 7, 0); // 7
    s21_decimal result = {0};
    
    ck_assert_int_eq(s21_sub(a, b, &result), OK);
    assert_decimal_eq(result, expected);
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

START_TEST(s21_test_sub_max_minus_one) {
    s21_decimal max = new_dec_native(0xFFFFFFFF, 0xFFFFFFFF, 
                                       0xFFFFFFFF, 0);
    s21_decimal one = new_dec_native(0, 0, 1, 0); // 1
    s21_decimal expected = new_dec_native(0xFFFFFFFF, 0xFFFFFFFF,
                                           0xFFFFFFFE, 0);
    s21_decimal result = {0};
    
    ck_assert_int_eq(s21_sub(max, one, &result), OK);
    assert_decimal_eq(result, expected);
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

START_TEST(s21_test_sub_equal_numbers) {
    s21_decimal a = new_dec_native(0, 0, 0xABCDEF01, 0);
    s21_decimal b = a;
    s21_decimal expected = {0};
    s21_decimal result = {0};
    
    ck_assert_int_eq(s21_sub(a, b, &result), OK);
    assert_decimal_eq(result, expected);
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

START_TEST(s21_test_sub_with_borrow) {
    s21_decimal a = new_dec_native(0, 1, 0, 0);  // 0x100000000
    s21_decimal b = new_dec_native(0, 0, 1, 0);   // 1
    s21_decimal expected = new_dec_native(0, 0, 0xFFFFFFFF, 0);
    s21_decimal result = {0};
    
    ck_assert_int_eq(s21_sub(a, b, &result), OK);
    assert_decimal_eq(result, expected);
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

START_TEST(s21_test_sub_null_pointers) {
    s21_decimal a = new_dec_native(0, 0, 5, 0);
    s21_decimal b = new_dec_native(0, 0, 3, 0);
    
    ck_assert_int_eq(s21_sub(a, b, NULL), ERROR);
}
END_TEST

START_TEST(test_sub_a_less_than_b) {
    s21_big_decimal a = new_big_native(0, 0, 0, 0, 0, 0, 3, 0);
    s21_big_decimal b = new_big_native(0, 0, 0, 0, 0, 0, 5, 0);
    s21_big_decimal result = {0};
    
    ck_assert_int_eq(sub(&a, &b, &result), MINUS_INF);
}
END_TEST

START_TEST(s21_test_sub_a_less_than_b) {
    s21_decimal a = new_dec_native(0, 0, 3, 0);
    s21_decimal b = new_dec_native(0, 0, 5, 0);
    s21_decimal result = {0};
    
    ck_assert_int_eq(s21_sub(a, b, &result), MINUS_INF);
}
END_TEST

Suite* sub_suite(void) {
    Suite *s = suite_create("Big Decimal Subtraction");
    
    TCase *tc_core = tcase_create("Core");
    tcase_add_test(tc_core, test_sub_simple);
    tcase_add_test(tc_core, test_sub_max_minus_one);
    tcase_add_test(tc_core, test_sub_equal_numbers);
    tcase_add_test(tc_core, test_sub_with_borrow);

    tcase_add_test(tc_core, test_sub_phrases_0);
    tcase_add_test(tc_core, test_sub_huge_num_0);
    tcase_add_test(tc_core, test_sub_huge_num_1);
    tcase_add_test(tc_core, test_sub_huge_num_2);
    tcase_add_test(tc_core, test_sub_huge_num_3);
    tcase_add_test(tc_core, test_sub_huge_num_4);
    tcase_add_test(tc_core, test_sub_huge_num_5);
    tcase_add_test(tc_core, test_sub_zero);
    
    TCase *tc_errors = tcase_create("Errors");
    tcase_add_test(tc_errors, test_sub_null_pointers);
    tcase_add_test(tc_errors, test_sub_a_less_than_b);
    
    suite_add_tcase(s, tc_core);
    suite_add_tcase(s, tc_errors);
    return s;
}

Suite* s21_sub_suite(void) {
    Suite *s = suite_create("Big Decimal Subtraction");
    
    TCase *tc_core = tcase_create("Core");
    tcase_add_test(tc_core, s21_test_sub_simple);
    tcase_add_test(tc_core, s21_test_sub_max_minus_one);
    tcase_add_test(tc_core, s21_test_sub_equal_numbers);
    tcase_add_test(tc_core, s21_test_sub_with_borrow);

    tcase_add_test(tc_core, s21_test_sub_phrases_0);
    tcase_add_test(tc_core, s21_test_sub_huge_num_0);
    tcase_add_test(tc_core, s21_test_sub_huge_num_1);
    tcase_add_test(tc_core, s21_test_sub_huge_num_2);
    tcase_add_test(tc_core, s21_test_sub_huge_num_3);
    tcase_add_test(tc_core, s21_test_sub_huge_num_4);
    tcase_add_test(tc_core, s21_test_sub_huge_num_5);
    tcase_add_test(tc_core, s21_test_sub_zero);
    
    TCase *tc_errors = tcase_create("Errors");
    tcase_add_test(tc_errors, s21_test_sub_null_pointers);
    tcase_add_test(tc_errors, s21_test_sub_a_less_than_b);
    
    suite_add_tcase(s, tc_core);
    suite_add_tcase(s, tc_errors);
    return s;
}