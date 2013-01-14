#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>

#include "nanoluadict.h"

typedef struct lua_context {
    const char *filepath;
    struct lua_hook *hooks;
    struct lua_State *lua_state;
    void (*hook)(struct lua_context *self, char *callback, unsigned long int argc, ...);
} lua_context;


typedef struct lua_hook {
    const char *name;
    struct lua_hook *next;
} lua_hook;


typedef struct lua_arg {
    union {
        void *function;
        char *string;
        float number;       
    } value;

    enum EVENT_TYPE type;
} lua_arg;


static const char *available_hooks[] = {
    "on_tick",
    "before_order",
    "after_order"
};

static inline void lua_hook_free(lua_hook *head);
static inline lua_hook *lua_hook_add(lua_hook *head, const char *name);
static inline int lua_hook_find(lua_hook *head, const char *name);
static inline int lua_hook_call(lua_context *self, const char *callback, unsigned long int argc, ...);

void backtest_lua_destroy(lua_context *self);
lua_context *backtest_lua_init(const char *filepath);
