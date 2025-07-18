#include "../src/utils/utils.h"

#define OK 0;
#define OVERFLOW 1;
#define ERROR 2;

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    int flag = OK;
    if (!result) return ERROR; // Ошибка: указатель NULL
    reset_decimal(result);

    s21_big_decimal big_1 = {0}, big_2 = {0};
    convert_to_big_decimal(&value_1, &big_1);
    convert_to_big_decimal(&value_2, &big_2);

    normalize_scales(&big_1, &big_2);

    int sign_1 = get_sign(&value_1);
    int sign_2 = get_sign(&value_2);

    /*
    
    */
   return flag;
}

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    int flag = OK;
    int scale_1 = get_scale(&value_1);
    int scale_2 = get_scale(&value_2);


    int sign_1 = get_sign(&value_1);
    int sign_2 = get_sign(&value_2);

    /*

    */
   return flag;
}