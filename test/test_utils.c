// test/test_utils.c
#include "../test/test_utils.h"

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



/* Тесты для to_big() */

START_TEST(test_to_big_null_input) {
    s21_big_decimal big;
    int res = to_big(NULL, &big);
    ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(test_to_big_normal_conversion) {
    s21_decimal num = new_dec_native(0xFFFFFFFF, 0xAAAAAAAA, 0x55555555, 0x80010000);
    s21_big_decimal expected = new_big_native(0, 0, 0, 0, 0xFFFFFFFF, 0xAAAAAAAA, 0x55555555, 0x80010000);
    s21_big_decimal result;
    
    int res = to_big(&num, &result);
    
    ck_assert_int_eq(res, 0);
    assert_big_decimal_eq(result, expected);
}
END_TEST

START_TEST(test_to_big_zero_values) {
    s21_decimal num = new_dec_native(0, 0, 0, 0x30000);
    s21_big_decimal expected = new_big_native(0, 0, 0, 0, 0, 0, 0, 0x30000);
    s21_big_decimal result;
    
    int res = to_big(&num, &result);
    
    ck_assert_int_eq(res, 0);
    assert_big_decimal_eq(result, expected);
}
END_TEST

/* Тесты для to_dec() */

START_TEST(test_to_dec_null_input) {
    s21_decimal num;
    int res = to_dec(NULL, &num);
    ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(test_to_dec_normal_conversion) {
    s21_big_decimal big = new_big_native(0, 0, 0, 0, 0, 0xAAAAAAAA, 0x55555555, 0x80010000);
    s21_decimal expected = new_dec_native(0, 0xAAAAAAAA, 0x55555555, 0x80010000);
    s21_decimal result;
    
    int res = to_dec(&big, &result);
    
    ck_assert_int_eq(res, 0);
    assert_decimal_eq(result, expected);
}
END_TEST

START_TEST(test_to_dec_overflow) {
    s21_big_decimal big = new_big_native(0, 0, 1, 0, 0, 0, 0, 0x10000);
    s21_decimal result;
    
    int res = to_dec(&big, &result);
    
    ck_assert_int_eq(res, 2);
}
END_TEST

START_TEST(test_to_dec_zero_values) {
    s21_big_decimal big = new_big_native(0, 0, 0, 0, 0, 0, 0, 0x30000);
    s21_decimal expected = new_dec_native(0, 0, 0, 0x30000);
    s21_decimal result;
    
    int res = to_dec(&big, &result);
    
    ck_assert_int_eq(res, 0);
    assert_decimal_eq(result, expected);
}
END_TEST

// Тест: простая конвертация
START_TEST(test_to_dec_simple) {
    s21_big_decimal big = new_big_native(0, 0, 0, 0, 0, 0, 123, 0x00020000); // 1.23
    s21_decimal result = {0};

    int res = to_dec(&big, &result);

    ck_assert_int_eq(res, 0);
    ck_assert_uint_eq(result.bits[0], 123);
    ck_assert_int_eq(get_scale(&result), 2);
    ck_assert_int_eq(get_sign(&result), 0);
}
END_TEST

// Тест: переполнение при scale == 0
START_TEST(test_to_dec_overflow_scale_0) {
    s21_big_decimal big = new_big_native(0, 0, 1, 0, 0, 0, 0, 0); // слишком большое
    s21_decimal result = {0};

    int res = to_dec(&big, &result);

    ck_assert_int_eq(res, 2);
}
END_TEST

// // Тест: 1.25 → 1.2 (банковское округление) автотесты с банковским пока что говно
// START_TEST(test_to_dec_bankers_1_25) {
//     // 1.25 = 125 * 10^(-2)
//     s21_big_decimal big = new_big_native(0, 0, 0, 1, 0xFFFFFFFF, 0xFFFFFFFF, 125, 0x00020000);
//     // s21_big_decimal expected = new_big_native(0, 0, 0, 0, 0x33333333, 0x33333333, 0x199999B7, 0x00010000);
    
//     s21_decimal result = {0};

//     int res = to_dec_with_bank_round(&big, &result);

//     ck_assert_int_eq(res, 0);
//     ck_assert_uint_eq(result.bits[0], 12); // 1.2
//     ck_assert_int_eq(get_scale(&result), 2);
// }
// END_TEST

// // Тест: 1.35 → 1.4
// START_TEST(test_to_dec_bankers_1_35) {
//     s21_big_decimal big = new_big_native(0, 0, 0, 1, 0xFFFFFFFF, 0xFFFFFFFF, 135, 0x00020000);
//     s21_decimal result = {0};

//     int res = to_dec_with_bank_round(&big, &result);

//     ck_assert_int_eq(res, 0);
//     ck_assert_uint_eq(result.bits[0], 14);
//     ck_assert_int_eq(get_scale(&result), 2);
// }
// END_TEST

// // Тест: 2.5 → 2 (чётное)
// START_TEST(test_to_dec_bankers_2_5) {
//     s21_big_decimal big = new_big_native(0, 0, 0, 1, 0xFFFFFFFF, 0xFFFFFFFF, 25, 0x00010000);
//     s21_decimal result = {0};

//     int res = to_dec_with_bank_round(&big, &result);

//     ck_assert_int_eq(res, 0);
//     ck_assert_uint_eq(result.bits[0], 2);
//     ck_assert_int_eq(get_scale(&result), 1);
// }
// END_TEST

// // Тест: 3.5 → 4 (нечётное → округляем вверх)
// START_TEST(test_to_dec_bankers_3_5) {
//     s21_big_decimal big = new_big_native(0, 0, 0, 1, 0xFFFFFFFF, 0xFFFFFFFF, 35, 0x00010000);
//     s21_decimal result = {0};

//     int res = to_dec_with_bank_round(&big, &result);

//     ck_assert_int_eq(res, 0);
//     ck_assert_uint_eq(result.bits[0], 4);
//     ck_assert_int_eq(get_scale(&result), 1);
// }
// END_TEST

// Тест: NULL указатель
START_TEST(test_to_dec_null) {
    s21_decimal result = {0};
    int res = to_dec(NULL, &result);
    ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(test_compare_equal) {
    s21_big_decimal a = new_big_native(0, 0, 0, 0, 0, 0, 123, 0);
    s21_big_decimal b = new_big_native(0, 0, 0, 0, 0, 0, 123, 0);
    ck_assert_int_eq(compare_big_decimal(&a, &b), 0);
}
END_TEST

START_TEST(test_compare_first_greater_lsb) {
    s21_big_decimal a = new_big_native(0, 0, 0, 0, 0, 0, 124, 0);
    s21_big_decimal b = new_big_native(0, 0, 0, 0, 0, 0, 123, 0);
    ck_assert_int_eq(compare_big_decimal(&a, &b), 1);
}
END_TEST

START_TEST(test_compare_first_greater_msb) {
    s21_big_decimal a = new_big_native(1, 0, 0, 0, 0, 0, 0, 0);
    s21_big_decimal b = new_big_native(0, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0);
    ck_assert_int_eq(compare_big_decimal(&a, &b), 1);
}
END_TEST

START_TEST(test_compare_second_greear_lsb) {
    s21_big_decimal a = new_big_native(0, 0, 0, 0, 0, 0, 123, 0);
    s21_big_decimal b = new_big_native(0, 0, 0, 0, 0, 0, 124, 0);
    ck_assert_int_eq(compare_big_decimal(&a, &b), -1);
}
END_TEST

START_TEST(test_compare_second_greater_msb) {
    s21_big_decimal a = new_big_native(0, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0);
    s21_big_decimal b = new_big_native(1, 0, 0, 0, 0, 0, 0, 0);
    ck_assert_int_eq(compare_big_decimal(&a, &b), -1);
}
END_TEST

START_TEST(test_compare_max_values) {
    s21_big_decimal max = new_big_native(0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0);
    ck_assert_int_eq(compare_big_decimal(&max, &max), 0);
}
END_TEST

START_TEST(test_compare_zero) {
    s21_big_decimal zero = {0};
    ck_assert_int_eq(compare_big_decimal(&zero, &zero), 0);
}
END_TEST

START_TEST(test_compare_with_zero) {
    s21_big_decimal num = new_big_native(0, 0, 0, 0, 0, 0, 1, 0);
    s21_big_decimal zero = {0};
    ck_assert_int_eq(compare_big_decimal(&num, &zero), 1);
    ck_assert_int_eq(compare_big_decimal(&zero, &num), -1);
}
END_TEST

// Тест: 123 / 10 = 12, остаток 3, scale уменьшается
START_TEST(test_divide_by_10_simple) {
    // Число 12.3 → scale = 1
    s21_big_decimal num = new_big_native(0, 0, 0, 0, 0, 0, 123, 0x00010000);
    unsigned rem = divide_by_10(&num);

    ck_assert_uint_eq(rem, 3);
    ck_assert_uint_eq(num.bits[0], 12);
    ck_assert_int_eq(get_big_scale(&num), 0); // 1 → 0 ?????
}
END_TEST

// Тест: 100 / 10 = 10, остаток 0
START_TEST(test_divide_by_10_exact) {
    s21_big_decimal num = new_big_native(0, 0, 0, 0, 0, 0, 100, 0x00010000); // 10.0
    unsigned rem = divide_by_10(&num);

    ck_assert_uint_eq(rem, 0);
    ck_assert_uint_eq(num.bits[0], 10);
    ck_assert_int_eq(get_big_scale(&num), 0);
}
END_TEST

// Тест: 1 / 10 = 0, остаток 1
START_TEST(test_divide_by_10_small) {
    s21_big_decimal num = new_big_native(0, 0, 0, 0, 0, 0, 1, 0x00010000); // 0.1
    unsigned rem = divide_by_10(&num);

    ck_assert_uint_eq(rem, 1);
    ck_assert_uint_eq(num.bits[0], 0);
    ck_assert_int_eq(get_big_scale(&num), 0);
}
END_TEST

// Тест: 0 / 10 = 0, остаток 0
START_TEST(test_divide_by_10_zero) {
    s21_big_decimal num = new_big_native(0, 0, 0, 0, 0, 0, 0, 0x00010000); // 0.0
    unsigned rem = divide_by_10(&num);

    ck_assert_uint_eq(rem, 0);
    ck_assert_uint_eq(num.bits[0], 0);
    ck_assert_int_eq(get_big_scale(&num), 0);
}
END_TEST

// Тест: 999 / 10 = 99, остаток 9
START_TEST(test_divide_by_10_remainder_9) {
    s21_big_decimal num = new_big_native(0, 0, 0, 0, 0, 0, 999, 0x00010000); // 99.9
    unsigned rem = divide_by_10(&num);

    ck_assert_uint_eq(rem, 9);
    ck_assert_uint_eq(num.bits[0], 99);
    ck_assert_int_eq(get_big_scale(&num), 0);
}
END_TEST

// Тест: число больше 2^32
START_TEST(test_divide_by_10_large) {
    // 10^10 = 10000000000
    s21_big_decimal num = new_big_native(0, 0, 0, 0, 0, 2, 0x08CA6C00, 0x00010000); // 1000000000.0
    unsigned rem = divide_by_10(&num);

    ck_assert_uint_eq(rem, 0);
    ck_assert_uint_eq(num.bits[0], 0x34143E00);
    ck_assert_uint_eq(num.bits[1], 0);
    ck_assert_int_eq(get_big_scale(&num), 0);
}
END_TEST

// Тест: масштаб 2 → 12.34 / 10 = 1.234, scale = 1
START_TEST(test_divide_by_10_scale_2) {
    s21_big_decimal num = new_big_native(0, 0, 0, 0, 0, 0, 1234, 0x00020000); // 12.34
    unsigned rem = divide_by_10(&num);

    ck_assert_uint_eq(rem, 4);
    ck_assert_uint_eq(num.bits[0], 123);
    ck_assert_int_eq(get_big_scale(&num), 1);
}
END_TEST

// deepseek

START_TEST(test_divide_by_10_simple_1) {
    // 50 / 10 = 5 (остаток 0)
    s21_big_decimal num = new_big_native(0, 0, 0, 0, 0, 0, 50, 0);
    unsigned expected_remainder = 0;
    s21_big_decimal expected_num = new_big_native(0, 0, 0, 0, 0, 0, 5, 0);
    
    unsigned remainder = divide_by_10(&num);
    
    ck_assert_uint_eq(remainder, expected_remainder);
    assert_big_decimal_eq(num, expected_num);
}
END_TEST

START_TEST(test_divide_by_10_with_remainder) {
    // 123 / 10 = 12 (остаток 3)
    s21_big_decimal num = new_big_native(0, 0, 0, 0, 0, 0, 123, 0);
    unsigned expected_remainder = 3;
    s21_big_decimal expected_num = new_big_native(0, 0, 0, 0, 0, 0, 12, 0);
    
    unsigned remainder = divide_by_10(&num);
    
    ck_assert_uint_eq(remainder, expected_remainder);
    assert_big_decimal_eq(num, expected_num);
}
END_TEST

START_TEST(test_divide_by_10_large_number) {
    // 12345678901234567890 / 10 = 1234567890123456789 (остаток 0)
    s21_big_decimal num = new_big_native(0, 0, 0xAB54A98C, 0x2DFDC1C0, 0, 0, 0x499602D2, 0x00010000);
    unsigned expected_remainder = 2;
    s21_big_decimal expected_num = new_big_native(0, 0, 0x112210F4, 0x6AFFC693, 0x33333333, 0x33333333, 0x3A8F0048, 0);
    
    unsigned remainder = divide_by_10(&num);
    printf("test_divide_by_10_large_number, num: \n");
    print_big_native_hex(&num);

    printf("test_divide_by_10_large_number, num: \n");
    print_big_native_hex(&expected_num);
    
    ck_assert_uint_eq(remainder, expected_remainder);
    assert_big_decimal_eq(num, expected_num);
}
END_TEST

START_TEST(test_divide_by_10_max_uint32) {
    // 4294967295 / 10 = 429496729 (остаток 5)
    s21_big_decimal num = new_big_native(0, 0, 0, 0, 0, 0, 0xFFFFFFFF, 0);
    unsigned expected_remainder = 5;
    s21_big_decimal expected_num = new_big_native(0, 0, 0, 0, 0, 0, 0x19999999, 0);
    
    unsigned remainder = divide_by_10(&num);
    
    ck_assert_uint_eq(remainder, expected_remainder);
    assert_big_decimal_eq(num, expected_num);
}
END_TEST

START_TEST(test_divide_by_10_multiple_words) {
    // 0x100000000 / 10 = 0x19999999 (остаток 6)
    s21_big_decimal num = new_big_native(0, 0, 0, 0, 0, 1, 0, 0);
    unsigned expected_remainder = 6;
    s21_big_decimal expected_num = new_big_native(0, 0, 0, 0, 0, 0, 0x19999999, 0);
    
    unsigned remainder = divide_by_10(&num);
    
    ck_assert_uint_eq(remainder, expected_remainder);
    assert_big_decimal_eq(num, expected_num);
}
END_TEST

START_TEST(test_divide_by_10_with_scale) {
    // 123.4 (scale=1) / 10 = 12.34 (scale=0), но scale уменьшается на 1
    s21_big_decimal num = new_big_native(0, 0, 0, 0, 0, 0, 1234, 0x10000);
    unsigned expected_remainder = 4;
    s21_big_decimal expected_num = new_big_native(0, 0, 0, 0, 0, 0, 123, 0);
    
    unsigned remainder = divide_by_10(&num);
    
    ck_assert_uint_eq(remainder, expected_remainder);
    assert_big_decimal_eq(num, expected_num);
    ck_assert_uint_eq(get_big_scale(&num), 0);
}
END_TEST

// Сборка тестов
Suite *divide_by_10_suite(void) {
    Suite *s = suite_create("divide_by_10");

    TCase *tc_core = tcase_create("Core");
    tcase_add_test(tc_core, test_divide_by_10_simple);
    tcase_add_test(tc_core, test_divide_by_10_exact);
    tcase_add_test(tc_core, test_divide_by_10_small);
    tcase_add_test(tc_core, test_divide_by_10_zero);
    tcase_add_test(tc_core, test_divide_by_10_remainder_9);
    tcase_add_test(tc_core, test_divide_by_10_large);
    tcase_add_test(tc_core, test_divide_by_10_scale_2);

    tcase_add_test(tc_core, test_divide_by_10_simple_1);
    tcase_add_test(tc_core, test_divide_by_10_with_remainder);
    tcase_add_test(tc_core, test_divide_by_10_large_number);
    tcase_add_test(tc_core, test_divide_by_10_max_uint32);
    tcase_add_test(tc_core, test_divide_by_10_multiple_words);
    tcase_add_test(tc_core, test_divide_by_10_with_scale);


    suite_add_tcase(s, tc_core);
    return s;
}

Suite* compare_suite(void) {
    Suite *s = suite_create("Big Decimal Comparison");
    
    TCase *tc_core = tcase_create("Core");
    tcase_add_test(tc_core, test_compare_equal);
    tcase_add_test(tc_core, test_compare_first_greater_lsb);
    tcase_add_test(tc_core, test_compare_first_greater_msb);
    tcase_add_test(tc_core, test_compare_second_greear_lsb);
    tcase_add_test(tc_core, test_compare_second_greater_msb);
    tcase_add_test(tc_core, test_compare_max_values);
    tcase_add_test(tc_core, test_compare_zero);
    tcase_add_test(tc_core, test_compare_with_zero);
    
    suite_add_tcase(s, tc_core);
    return s;
}

Suite *suite_shift(void) {
    Suite *s = suite_create("Big Decimal Utils Functions Test");

    TCase *tc_shift = tcase_create("Core");
    tcase_add_test(tc_shift, test_shift_left_one_bit);
    tcase_add_test(tc_shift, test_shift_left_three_bits);
    tcase_add_test(tc_shift, test_shift_left_with_carry_between_words);
    tcase_add_test(tc_shift, test_shift_left_does_not_touch_meta);

    suite_add_tcase(s, tc_shift);
    return s;
}

Suite *suite_normalize(void) {
    Suite *s = suite_create("Big Decimal Utils Functions Test");

    TCase *tc_normalize = tcase_create("Core");
    tcase_add_test(tc_normalize, test_normalize_equal_scales);
    tcase_add_test(tc_normalize, test_normalize_num1_greater_scale);
    tcase_add_test(tc_normalize, test_normalize_num2_greater_scale);

    tcase_add_test(tc_normalize, test_multiply_by_10_basic);
    tcase_add_test(tc_normalize, test_normalize_scales_increase_first);
    tcase_add_test(tc_normalize, test_normalize_scales_increase_second);
    tcase_add_test(tc_normalize, test_normalize_scales_equal);
    
    suite_add_tcase(s, tc_normalize);
    return s;
}

Suite *convert_suite(void) {
    Suite *s = suite_create("Conversion Suite");
    
    TCase *tc_to_big = tcase_create("to_big");
    tcase_add_test(tc_to_big, test_to_big_null_input);
    tcase_add_test(tc_to_big, test_to_big_normal_conversion);
    tcase_add_test(tc_to_big, test_to_big_zero_values);
    suite_add_tcase(s, tc_to_big);
    
    TCase *tc_to_dec = tcase_create("to_dec");
    tcase_add_test(tc_to_dec, test_to_dec_null_input);
    tcase_add_test(tc_to_dec, test_to_dec_normal_conversion);
    tcase_add_test(tc_to_dec, test_to_dec_overflow);
    tcase_add_test(tc_to_dec, test_to_dec_zero_values);
    tcase_add_test(tc_to_dec, test_to_dec_null);
    // tcase_add_test(tc_to_dec, test_to_dec_bankers_1_25);
    // tcase_add_test(tc_to_dec, test_to_dec_bankers_1_35);
    // tcase_add_test(tc_to_dec, test_to_dec_bankers_2_5);
    // tcase_add_test(tc_to_dec, test_to_dec_bankers_3_5);


    suite_add_tcase(s, tc_to_dec);
    
    return s;
}