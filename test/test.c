#include <check.h>
#include <stdlib.h>
#include "test_utils.h"

int main(void) {
    Suite *s = make_suite();
    // Создаем runner
    SRunner *runner = srunner_create(s);
    srunner_add_suite(runner, convert_suite());

    srunner_run_all(runner, CK_VERBOSE);
    int failed = srunner_ntests_failed(runner);
    srunner_free(runner);

    return (failed) ? 1 : 0;
}