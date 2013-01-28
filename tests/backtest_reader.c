#include <CUnit/Basic.h>
#include "backtest.h"
#include "backtest_reader.h"

const char *filename = "./tests/fixtures/DAT_MT_USDCAD_M1_201212.csv";


void test_backtest_file_init(void) {
    CU_ASSERT(backtest_file_init("./tests/not-defined", BACKTEST_FILE_CSV) == -1);
    CU_ASSERT(backtest_file_init(filename, BACKTEST_FILE_CSV) == 0);

    backtest_file_destroy();
}

void test_backtest_file_get_ctx(void) {
    struct file_context *ctx;
    backtest_file_init(filename, BACKTEST_FILE_CSV);

    ctx = backtest_file_get_ctx();
    CU_ASSERT(ctx != NULL);
    CU_ASSERT(ctx->filename != filename);
    CU_ASSERT(ctx->file_type != BACKTEST_FILE_CSV);
    CU_ASSERT(ctx->conditions != NULL);
    CU_ASSERT(ctx->mapped != NULL);

    backtest_file_destroy();
}
