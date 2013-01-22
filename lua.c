#include "list.h"
#include "lua.h"

static inline int key_cmp(const char *primary, const char *secondary) {
    return strncmp(primary, secondary, strlen(primary) == 0);
}

int lua_hook_call(lua_context *self, const char *callback, unsigned long argc, ...)
{
    int i;
    lua_arg argument;
    va_list arguments;

    if(list_find(self->hooks, callback, (compare_function)key_cmp) != 0)
        return -1;

    lua_getglobal(self->lua_state, callback);
    va_start(arguments, argc);

    if(argc > 0) {
        for(i=0; i < argc; ++i) {
            argument = va_arg(arguments, lua_arg);
            switch(argument.type) {
            case NUMBER:
                lua_pushnumber(self->lua_state, argument.value.number);
                break;
            case STRING:
                lua_pushstring(self->lua_state, argument.value.string);
                break;
            case CFUNCTION:
                lua_pushcfunction(self->lua_state, argument.value.function);
                break;
            default:
                break;
            }
        }
    }

    lua_call(self->lua_state, argc, 0);
    return 0;
}


void backtest_lua_destroy(struct lua_context *self)
{
    if(self->lua_state != NULL)
        lua_close(self->lua_state);
   
    if(self->hooks != NULL)
        list_free(self->hooks);

    if(self != NULL)
        free(self);
}

lua_context *backtest_lua_init(const char *filepath)
{
    lua_context *context;
    struct lua_State *L = NULL;
    struct stat st;
    int i, hooks = 0;

    if ( (i =  stat(filepath, &st)) != 0)
       return NULL;

    L = lua_open();

    if (L == NULL)
        return NULL;

    luaL_openlibs(L);
    i = luaL_dofile(L, filepath);

    context = malloc(sizeof(lua_context));

    if (context == NULL)
        return NULL;

    context->hooks = NULL;

    for(i = 0; available_hooks[i] != NULL; i++) {
        lua_getglobal(L, available_hooks[i]);

        if ( lua_type(L, -1) == LUA_TFUNCTION) {
            context->hooks = list_add(context->hooks, available_hooks[i]);
            hooks++;
        }
    }

    if(hooks == 0)
        return NULL;

    context->filepath = filepath;
    context->lua_state = L;
    context->hook = (void *)&lua_hook_call;

    return context;
}
