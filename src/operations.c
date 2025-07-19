#include "../src/utils/utils.h"

#define OK 0;
#define PLUS_INF 1;
#define MINUS_INF 2;
#define DIV_BY_ZERO 3;
#define ERROR 4;

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    int flag = OK;
    if (!result) return ERROR; // Ошибка: указатель NULL
    *result = (s21_decimal){0};

    s21_big_decimal big_1 = {0}, big_2 = {0};
    to_big(&value_1, &big_1);
    to_big(&value_2, &big_2);

    normalize_scales(&big_1, &big_2);

    int sign_1 = get_sign(&value_1);
    int sign_2 = get_sign(&value_2);

    s21_big_decimal result_big = {0};

    // Складываем или вычитаем в зависимости от знаков
    if (sign_1 == sign_2) {
        flag = add(&big_1, &big_2, &result_big);
    }

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