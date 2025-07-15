#ifndef S21_DECIMAL_H
#define S21_DECIMAL_H

#define DEC_BEGIN 2
#define BIG_BEGIN 6
#define BIG_METAINFO 7
#define DEC_METAINFO 3

#include <stdio.h>

typedef struct {
  unsigned int bits[4];
} s21_decimal;

typedef struct {
  unsigned int bits[8];
} s21_big_decimal;
#endif