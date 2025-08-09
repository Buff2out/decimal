// test/test_div.c
#include "../test/test_div.h"

// Тест: 10 / 2 = 5
START_TEST(test_div_simple) {
    s21_decimal a = new_dec_native(0, 0, 10, 0);
    s21_decimal b = new_dec_native(0, 0, 2, 0);
    s21_decimal result = {0};
    s21_decimal expected = new_dec_native(0, 0, 5, 0);
    int res = s21_div(a, b, &result);
    normalize_scales_decimal(&result, &expected);
    printf("test_div_simple\nresult: ");
    print_dec_native_hex(&result);
    printf("expected: ");
    print_dec_native_hex(&expected);
    
    ck_assert_int_eq(res, 0);
    assert_decimal_eq(result, expected);
}
END_TEST

// Тест: 1 / 3 = 0.3333333333333333333333333333 (28 знаков)
START_TEST(test_div_1_by_3) {
    s21_decimal a = new_dec_native(0, 0, 1, 0);
    s21_decimal b = new_dec_native(0, 0, 3, 0);
    s21_decimal result = {0};

    int res = s21_div(a, b, &result);
    ck_assert_int_eq(res, 0);

    // Проверяем, что результат ~0.3333333333333333333333333333
    ck_assert_uint_eq(result.bits[0], 333333333);
    ck_assert_uint_eq(result.bits[1], 333333333);
    ck_assert_uint_eq(result.bits[2], 333333333);
    ck_assert_int_eq(get_scale(&result), 28);
}
END_TEST

// Тест: 0.1 / 0.01 = 10
START_TEST(test_div_with_scale) {
    s21_decimal a = new_dec_native(0, 0, 1, 0x00010000); // 0.1
    s21_decimal b = new_dec_native(0, 0, 1, 0x00020000); // 0.01
    s21_decimal result = {0};
    s21_decimal expected = new_dec_native(0, 0, 10, 0); // 10

    int res = s21_div(a, b, &result);
    
    ck_assert_int_eq(res, 0);
    assert_decimal_eq(result, expected);
}
END_TEST

// Тест: (-5) / 2 = -2.5
START_TEST(test_div_negative) {
    s21_decimal a = new_dec_native(0, 0, 5, 0);
    s21_decimal b = new_dec_native(0, 0, 2, 0);
    s21_decimal result = {0};
    
    // Устанавливаем знак минус у a
    set_sign(&a, 1);
    
    s21_decimal expected = new_dec_native(0, 0, 25, 0x00010000); // 2.5
    set_sign(&expected, 1); // -2.5

    int res = s21_div(a, b, &result);
    
    ck_assert_int_eq(res, 0);
    assert_decimal_eq(result, expected);
}
END_TEST

// Тест: 5 / 1 = 5
START_TEST(test_div_by_one) {
    s21_decimal a = new_dec_native(0, 0, 5, 0);
    s21_decimal b = new_dec_native(0, 0, 1, 0);
    s21_decimal result = {0};
    s21_decimal expected = new_dec_native(0, 0, 5, 0);

    int res = s21_div(a, b, &result);
    
    ck_assert_int_eq(res, 0);
    assert_decimal_eq(result, expected);
}
END_TEST

// Тест: 5 / 5 = 1
START_TEST(test_div_by_self) {
    s21_decimal a = new_dec_native(0, 0, 5, 0);
    s21_decimal result = {0};
    s21_decimal expected = new_dec_native(0, 0, 1, 0);

    int res = s21_div(a, a, &result);
    
    ck_assert_int_eq(res, 0);
    assert_decimal_eq(result, expected);
}
END_TEST

// Тест: деление на 0 → ошибка
START_TEST(test_div_by_zero) {
    s21_decimal a = new_dec_native(0, 0, 5, 0);
    s21_decimal b = new_dec_native(0, 0, 0, 0);
    s21_decimal result = {0};

    int res = s21_div(a, b, &result);
    
    ck_assert_int_eq(res, 3); // ошибка деления на 0
}
END_TEST

// // Тест: переполнение (1e28 / 1e-28 = 1e56)
// START_TEST(test_div_overflow) {
//     // 1e28
//     s21_decimal a = new_dec_native(0x2386F26FC, 0xD3D3D3D3, 0xD3D3D3D3, 0x001C0000);
//     // 1e-28
//     s21_decimal b = new_dec_native(1, 0, 0, 0x001C0000);
//     s21_decimal result = {0};

//     int res = s21_div(a, b, &result);
    
//     ck_assert_int_eq(res, 1); // переполнение
// }
// END_TEST

// Тест: 0.05 / 0.1 = 0.5 → округление до 0.5
START_TEST(test_div_bankers_rounding) {
    s21_decimal a = new_dec_native(0, 0, 5, 0x00020000); // 0.05
    s21_decimal b = new_dec_native(0, 0, 1, 0x00010000); // 0.1
    s21_decimal result = {0};
    s21_decimal expected = new_dec_native(0, 0, 5, 0x00010000); // 0.5

    int res = s21_div(a, b, &result);
    
    ck_assert_int_eq(res, 0);
    assert_decimal_eq(result, expected);
}
END_TEST

// Тест: 0.005 / 0.01 = 0.5 → округление к чётному (0.5 → 0)
START_TEST(test_div_bankers_rounding_to_even) {
    s21_decimal a = new_dec_native(0, 0, 5, 0x00030000); // 0.005
    s21_decimal b = new_dec_native(0, 0, 1, 0x00020000); // 0.01
    s21_decimal result = {0};
    s21_decimal expected = new_dec_native(0, 0, 0, 0); // 0.0

    int res = s21_div(a, b, &result);
    
    ck_assert_int_eq(res, 0);
    assert_decimal_eq(result, expected);
}
END_TEST

// Тест: 0.015 / 0.01 = 1.5 → округление к чётному (1.5 → 2)
START_TEST(test_div_bankers_rounding_to_even_2) {
    s21_decimal a = new_dec_native(0, 0, 15, 0x00030000); // 0.015
    s21_decimal b = new_dec_native(0, 0, 1, 0x00020000); // 0.01
    s21_decimal result = {0};
    s21_decimal expected = new_dec_native(0, 0, 2, 0x00010000); // 2.0

    int res = s21_div(a, b, &result);
    
    ck_assert_int_eq(res, 0);
    assert_decimal_eq(result, expected);
}
END_TEST

// Сборка тестов
Suite *s21_div_suite(void) {
    Suite *s = suite_create("s21_div");

    TCase *tc_core = tcase_create("Core");
    tcase_add_test(tc_core, test_div_simple);
    tcase_add_test(tc_core, test_div_1_by_3);
    tcase_add_test(tc_core, test_div_with_scale);
    tcase_add_test(tc_core, test_div_negative);
    tcase_add_test(tc_core, test_div_by_one);
    tcase_add_test(tc_core, test_div_by_self);
    tcase_add_test(tc_core, test_div_by_zero);
    // tcase_add_test(tc_core, test_div_overflow);
    tcase_add_test(tc_core, test_div_bankers_rounding);
    tcase_add_test(tc_core, test_div_bankers_rounding_to_even);
    tcase_add_test(tc_core, test_div_bankers_rounding_to_even_2);

    suite_add_tcase(s, tc_core);
    return s;
}