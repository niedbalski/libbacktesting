#include <CUnit/Basic.h>
#include "backtest.h"
#include "backtest_reader.h"

void test_backtest_file_init(void) {
    CU_ASSERT(backtest_file_init("./tests/not-defined", BACKTEST_FILE_CSV) == -1);
}
