// src/operations.c
#include "../src/operations.h"

/*
какие бывают случаи
+a, +b + => a + b (sign a)
+a, +b - (a > b) => a - b (sign a)
+a, +b - (a <= b) => b - a (sign b)

-a, +b - => a + b (sign a)
-a, +b + (a > b) => a - b (sign a)
-a, +b + (a <= b) => b - a (sign b)

+a, -b - => a + b (sign a)
+a, -b + (a > b) => a - b (sign a)
+a, -b + (a <= b) => b - a (sign b)

-a, -b + => a + b (sign a)
-a, -b - (a > b) => a - b (sign a)
-a, -b - (a <= b) => b - a (sign b)

Сгруппируем:
+a, +b + => a + b (sign a)
+a, -b - => a + b (sign a)
-a, +b - => a + b (sign a)
-a, -b + => a + b (sign a)

+a, +b - (a > b) => a - b (sign a)
+a, -b + (a > b) => a - b (sign a)
-a, +b + (a > b) => a - b (sign a)
-a, -b - (a > b) => a - b (sign a)

+a, +b - (a <= b) => b - a (sign b)
+a, -b + (a <= b) => b - a (sign b)
-a, +b + (a <= b) => b - a (sign b)
-a, -b - (a <= b) => b - a (sign b)
*/

int add(const s21_big_decimal *a, const s21_big_decimal *b, s21_big_decimal *result) {
    int flag = OK;
    if (!a || !b || !result) {
        flag = ERROR;
    }
    else {
        *result = (s21_big_decimal){0};
        unsigned long long carry = 0;
        for (int i = BIG_BEGIN; i <= BIG_END; ++i) {
            unsigned long long sum = (unsigned long long)a->bits[i] + b->bits[i] + carry;
            result->bits[i] = (unsigned)(sum & 0xFFFFFFFF);
            carry = sum >> 32;
        }
        result->bits[BIG_METAINFO] = a->bits[BIG_METAINFO];
        if (carry) {
            flag = PLUS_INF;
        }
    }
    return flag;
}


// Функция подразумевает, что из большего числа вычитают меньшее.
int sub(const s21_big_decimal *a, const s21_big_decimal *b, s21_big_decimal *result) {
    int flag = OK;
    if (!a || !b || !result) {
        flag = ERROR;
    }
    else {
        *result = (s21_big_decimal){0};
        long long borrow = 0;
        for (int i = BIG_BEGIN; i <= BIG_END; ++i) {
            long long diff = (long long)a->bits[i] - b->bits[i] - borrow;
            borrow = 0;
            if (diff < 0) {
                diff += 0x100000000LL; // 2^32
                borrow = 1;
            }
            result->bits[i] = (unsigned int)(diff & 0xFFFFFFFF);
        }
        result->bits[BIG_METAINFO] = a->bits[BIG_METAINFO];
        if (borrow) flag = MINUS_INF;
    }
    return flag;
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

