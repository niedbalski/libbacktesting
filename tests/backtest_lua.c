#include <CUnit/Basic.h>

#include "backtest.h"
#include "backtest_lua.h"

void test_backtest_lua_init(void) {
    CU_ASSERT(backtest_lua_init("./tests/fixtures/not-defined") == -1);
    CU_ASSERT(backtest_lua_init("./tests/fixtures/moving.lua") == 0);

    backtest_lua_destroy();
}

void test_backtest_lua_get_ctx(void) {
    struct lua_context *ctx;
    backtest_lua_init("./tests/fixtures/moving.lua");

    ctx = backtest_lua_get_ctx();

    CU_ASSERT(ctx != NULL);
    CU_ASSERT(ctx->filepath != NULL);
    CU_ASSERT(ctx->hooks != NULL);
    CU_ASSERT(ctx->lua_state != NULL);

    backtest_lua_destroy();
}
