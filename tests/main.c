#include <CUnit/Basic.h>

#include "backtest.h"
#include "backtest_lua.h"
#include "backtest_reader.h"

int init_suite(void) {
    return 0;
}

int finish_suite(void) {
    return 0;
}

int main() {

    CU_pSuite backtest_suite = NULL;


    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    backtest_suite = CU_add_suite("backtest_library_suite", init_suite, finish_suite);

    if (NULL == backtest_suite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(backtest_suite, "test of backtest reader init", (CU_TestFunc)test_backtest_file_init);
    /* CU_add_test(backtest_suite, "test of backtest reader destroy", test_backtest_file_destroy); */
    /* CU_add_test(backtest_suite, "test for several price condition adds", test_add_price_condition); */
    /* CU_add_test(backtest_suite, "test of evaluation of conditions", test_evaluate_condition); */
    /* CU_add_test(backtest_suite, "test of lua interface init method", test_backtest_lua_init); */
    /* CU_add_test(backtest_suite, "test of call to lua method", test_backtest_lua_hook_call); */
    /* CU_add_test(backtest_suite, "test of lua interface destroy", test_backtest_lua_destroy);     */

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    return CU_get_error();
}
