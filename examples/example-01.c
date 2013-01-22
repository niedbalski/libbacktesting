#include "backtest.h"
#define CLEANUP(ctx) backtest_lua_destroy(ctx)

int main(void) 
{
    lua_context *ctx;
    lua_arg argument;
    int ret;

    ctx = backtest_lua_init("./plugins/moving.lua");

    if(ctx == NULL)
        return EXIT_FAILURE;

    argument.value.number = 100.0;
    argument.type = NUMBER;

    if((ret = ctx->hook(ctx, "on_tick", 1, argument)) != 0) {
        CLEANUP(ctx);
        return EXIT_FAILURE;
    }

    CLEANUP(ctx);
    return EXIT_SUCCESS;

}
