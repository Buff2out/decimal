#include <check.h>
#include <stdint.h>

#include "../src/utils/utils.h"

// Простая проверка двух big_decimal
void assert_big_decimal_eq(s21_big_decimal a, s21_big_decimal b) {
    for (int i = 0; i < 8; i++) {
        ck_assert_uint_eq(a.bits[i], b.bits[i]);
    }
}

START_TEST(test_shift_left_one_bit) {
    s21_big_decimal input = from_bits_native(0, 0, 0, 0, 0, 0, 1, 0); // число 1
    s21_big_decimal expected = from_bits_native(0, 0, 0, 0, 0, 0, 2, 0);

    s21_big_decimal result = shift_left(input, 1);
    assert_big_decimal_eq(result, expected);
}
END_TEST

START_TEST(test_shift_left_three_bits) {
    s21_big_decimal input = from_bits_native(0, 0, 0, 0, 0, 0, 1, 0); // 1
    s21_big_decimal expected = from_bits_native(0, 0, 0, 0, 0, 0, 8, 0); // 8

    s21_big_decimal result = shift_left(input, 3);
    assert_big_decimal_eq(result, expected);
}
END_TEST

START_TEST(test_shift_left_with_carry_between_words) {
    s21_big_decimal input = from_bits_native(0, 0, 0, 0, 0, 0, 0xFFFFFFFF, 0); // 32 единичных бита
    s21_big_decimal expected = from_bits_native(0, 0, 0, 0, 0, 1, 0xFFFFFFFE, 0); // << 1

    s21_big_decimal result = shift_left(input, 1);
    assert_big_decimal_eq(result, expected);
}
END_TEST

START_TEST(test_shift_left_does_not_touch_meta) {
    s21_big_decimal input = from_bits_native(1, 0, 0, 0, 0, 0, 0, 0xAABBCCDD);

    s21_big_decimal result = shift_left(input, 1);
    ck_assert_uint_eq(result.bits[7], 0xAABBCCDD); // bits[7] не должен меняться
}
END_TEST

START_TEST(test_normalize_equal_scales) {
    s21_big_decimal num1 = {{0}};
    s21_big_decimal num2 = {{0}};
    
    // Устанавливаем одинаковый scale
    set_big_scale(&num1, 5);
    set_big_scale(&num2, 5);
    
    // Заполняем разными значениями
    num1.bits[BIG_BEGIN] = 123;
    num2.bits[BIG_BEGIN] = 456;
    
    s21_big_decimal original_num1 = num1;
    s21_big_decimal original_num2 = num2;
    
    normalize_scales(&num1, &num2);
    
    // Проверяем что числа не изменились
    ck_assert_int_eq(num1.bits[BIG_BEGIN], original_num1.bits[BIG_BEGIN]);
    ck_assert_int_eq(num2.bits[BIG_BEGIN], original_num2.bits[BIG_BEGIN]);
    ck_assert_int_eq(get_big_scale(&num1), get_big_scale(&num2));
}
END_TEST

START_TEST(test_normalize_num1_greater_scale) {
    s21_big_decimal num1 = from_bits_native(0, 0, 0, 0, 0, 0, 100, 0);
    s21_big_decimal num2 = from_bits_native(0, 0, 0, 0, 0, 0, 10, 0);
    
    set_big_scale(&num1, 3);
    set_big_scale(&num2, 1);
    
    normalize_scales(&num1, &num2);
    
    // num2 должен быть умножен на 100 (3-1=2 разряда)
    ck_assert_int_eq(num2.bits[BIG_BEGIN], 1000);
    ck_assert_int_eq(get_big_scale(&num1), get_big_scale(&num2));
    ck_assert_int_eq(get_big_scale(&num2), 3);
}
END_TEST

START_TEST(test_normalize_num2_greater_scale) {
    s21_big_decimal num1 = from_bits_native(0, 0, 0, 0, 0, 0, 10, 0);
    s21_big_decimal num2 = from_bits_native(0, 0, 0, 0, 0, 0, 100, 0);
    
    set_big_scale(&num1, 1);
    set_big_scale(&num2, 3);
    
    normalize_scales(&num1, &num2);
    // print_big_decimal(&num1);
    // print_big_decimal(&num2);

    
    // num1 должен быть умножен на 100 (3-1=2 разряда)
    ck_assert_int_eq(num1.bits[BIG_BEGIN], 1000);
    ck_assert_int_eq(get_big_scale(&num1), get_big_scale(&num2));
    ck_assert_int_eq(get_big_scale(&num1), 3);
}
END_TEST

// Тест multiply_by_10
START_TEST(test_multiply_by_10_basic) {
    s21_big_decimal val = from_bits_native(0, 0, 0, 0, 0, 0, 1, 0);  // value: 1, scale = 0

    multiply_by_10(&val);
    multiply_by_10(&val);

    s21_big_decimal expected = from_bits_native(0, 0, 0, 0, 0, 0, 100, 0x020000);

    assert_big_decimal_eq(val, expected);
    // ck_assert_int_eq(expected.bits[BIG_BEGIN], val.bits[BIG_BEGIN]);
}
END_TEST

// Тест normalize_scales когда вторая меньше масштаб
START_TEST(test_normalize_scales_increase_first) {
    s21_big_decimal num1 = from_bits_native(0, 0, 0, 0, 0, 0, 123, 0);
    s21_big_decimal num2 = from_bits_native(0, 0, 0, 0, 0, 0, 123000, 0);
    set_big_scale(&num1, 2);
    set_big_scale(&num2, 5);

    normalize_scales(&num1, &num2);

    // num1 должен увеличиться scale до 5 и значение сдвинуться * 10^(3)
    s21_big_decimal expected1 = from_bits_native(0, 0, 0, 0, 0, 0, 123000, 0);
    s21_big_decimal expected2 = from_bits_native(0, 0, 0, 0, 0, 0, 123000, 0);
    set_big_scale(&expected1, 5);
    set_big_scale(&expected2, 5);

    assert_big_decimal_eq(num1, expected1);
    assert_big_decimal_eq(num2, expected2);
    ck_assert_int_eq(get_big_scale(&num1), 5);
    ck_assert_int_eq(get_big_scale(&num2), 5);
}
END_TEST

// Тест normalize_scales когда первая меньше масштаб
START_TEST(test_normalize_scales_increase_second) {
    s21_big_decimal num1 = from_bits_native(0, 0, 0, 0, 0, 0, 123000, 0);
    s21_big_decimal num2 = from_bits_native(0, 0, 0, 0, 0, 0, 123, 0);
    set_big_scale(&num1, 5);
    set_big_scale(&num2, 2);

    normalize_scales(&num1, &num2);

    s21_big_decimal expected1 = from_bits_native(0, 0, 0, 0, 0, 0, 123000, 0);
    s21_big_decimal expected2 = from_bits_native(0, 0, 0, 0, 0, 0, 123000, 0);
    set_big_scale(&expected1, 5);
    set_big_scale(&expected2, 5);

    assert_big_decimal_eq(num1, expected1);
    assert_big_decimal_eq(num2, expected2);
    ck_assert_int_eq(get_big_scale(&num1), 5);
    ck_assert_int_eq(get_big_scale(&num2), 5);
}
END_TEST

// Тест normalize_scales когда масштабы равны
START_TEST(test_normalize_scales_equal) {
    s21_big_decimal num1 = from_bits_native(0, 0, 0, 0, 0, 0, 12345, 0);
    s21_big_decimal num2 = from_bits_native(0, 0, 0, 0, 0, 0, 12345, 0);
    set_big_scale(&num1, 3);
    set_big_scale(&num2, 3);

    normalize_scales(&num1, &num2);

    assert_big_decimal_eq(num1, num2);
    ck_assert_int_eq(get_big_scale(&num1), 3);
    ck_assert_int_eq(get_big_scale(&num2), 3);
}
END_TEST

Suite *make_suite(void) {
    Suite *s = suite_create("Big Decimal Utils Functions Test");

    TCase *tc_core = tcase_create("Core");
    tcase_add_test(tc_core, test_shift_left_one_bit);
    tcase_add_test(tc_core, test_shift_left_three_bits);
    tcase_add_test(tc_core, test_shift_left_with_carry_between_words);
    tcase_add_test(tc_core, test_shift_left_does_not_touch_meta);

    TCase *tc_normalize = tcase_create("Core");
    tcase_add_test(tc_normalize, test_normalize_equal_scales);
    tcase_add_test(tc_normalize, test_normalize_num1_greater_scale);
    tcase_add_test(tc_normalize, test_normalize_num2_greater_scale);

    tcase_add_test(tc_core, test_multiply_by_10_basic);
    tcase_add_test(tc_core, test_normalize_scales_increase_first);
    tcase_add_test(tc_core, test_normalize_scales_increase_second);
    tcase_add_test(tc_core, test_normalize_scales_equal);
    
    suite_add_tcase(s, tc_normalize);

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