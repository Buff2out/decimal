// src/operations.c
#include "../src/operations.h"

#define OK 0;
#define PLUS_INF 1;
#define MINUS_INF 2;
#define DIV_BY_ZERO 3;
#define ERROR 4;

int add(const s21_big_decimal *a, const s21_big_decimal *b, s21_big_decimal *result) {
    if (!a || !b || !result) return 1;  // Проверка на NULL
    
    *result = (s21_big_decimal){0};
    unsigned long long carry = 0;
    
    // Складываем по битам, начиная с младших
    for (int i = 0; i < 7; i++) {  // 7 слов (без метаинформации)
        unsigned long long sum = (unsigned long long)a->bits[i] + b->bits[i] + carry;
        result->bits[i] = (unsigned)(sum & 0xFFFFFFFF);
        carry = sum >> 32;
    }
    
    // Переносим метаинформацию (знак и степень) из первого числа
    result->bits[7] = a->bits[7];
    
    // Проверка на переполнение
    if (carry) {
        return 1;  // Флаг переполнения
    }
    
    return 0;
}

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

// int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
//     s21_decimal flag = value_1;
//     flag = value_2;
//     s21_decimal* flag = result;
//     int flag = OK;
//     // int scale_1 = get_scale(&value_1);
//     // int scale_2 = get_scale(&value_2);


//     // int sign_1 = get_sign(&value_1);
//     // int sign_2 = get_sign(&value_2);

//     /*

//     */
//    return flag;
// }

