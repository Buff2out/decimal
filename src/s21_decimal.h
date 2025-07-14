#ifndef S21_DECIMAL_H
#define S21_DECIMAL_H

#define dec_begin 2
#define big_begin 6
#define big_metainfo 7
#define dec_metainfo 3

#include <stdio.h>

typedef struct {
  unsigned int bits[4];
} s21_decimal;

typedef struct {
  unsigned int bits[8];
} s21_big_decimal;
#endif