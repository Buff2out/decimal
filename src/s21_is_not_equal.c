#include "s21_decimal.h"
#include "utils/utils.h"

int s21_is_not_equal(s21_decimal num1, s21_decimal num2) {
  int is_not_equal = 0;
  s21_big_decimal num1_big;
  s21_big_decimal num2_big;
  convert_to_big_decimal(&num1, &num1_big);
  convert_to_big_decimal(&num2, &num2_big);
  normalize_scales(&num1_big, &num2_big);
  int sign1 = get_big_sign(&num1_big);
  int sign2 = get_big_sign(&num2_big);
  if (is_big_zero(&num1_big) && is_big_zero(&num2_big)) {
    is_not_equal = 0;
  } else if (sign1 != sign2) {
    is_not_equal = 1;
  } else {
    int cmp = compare_big_decimal(&num1_big, &num2_big);
    is_not_equal = cmp == 1 || cmp == -1;
  }

  return is_not_equal;
}