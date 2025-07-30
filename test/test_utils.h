// test/test_utils.h
#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <check.h>
#include "../src/utils/utils.h"
#include "../test/test.h"

Suite *convert_suite(void);
Suite *suite_normalize(void);
Suite *suite_shift(void);
Suite* compare_suite(void);
Suite *divide_by_10_suite(void);

#endif