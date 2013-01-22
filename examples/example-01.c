#include "backtest.h"

int main(void) {
    lua_context *ctx;
    lua_arg argument;

    int ret;

    ctx = backtest_lua_init("./plugins/moving.luas");

    if(ctx == NULL)
        goto error;

    argument.value.number = 100.0;
    argument.type = NUMBER;

    if( (ret = ctx->hook(ctx, "on_tick", 1, argument)) != 0)
        goto error;

error:
    if(ctx != NULL) {
        backtest_lua_destroy(ctx);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
