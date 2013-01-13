#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "nanoluadict.h"

typedef struct lua_context {
    const char *filepath;
    struct lua_hook *hooks;
    struct lua_State *lua_state;
    void (*hook)(char *callback, float number);
} lua_context;

typedef struct lua_hook {
    const char *name;
    struct lua_hook *next;
} lua_hook;

static const char *available_hooks[] = {
    "on_tick",
    "before_order",
    "after_order"
};

static struct lua_context *context = NULL;


static inline void lua_hook_free(struct lua_hook *head) 
{
    struct lua_hook *p, *next;

    for (p = context->hooks; p != NULL; p = next) {
        next = p->next;
        free(p);
    }
}

static inline int lua_hook_find(struct lua_hook *head, char *name)
{
    struct lua_hook *current;

    current = head;

    while(current != NULL) {
        if(strncmp(current->name, name, strlen(current->name)) == 0)
            return 0;
        current = current->next;
    }

    return -1;
}

static inline struct lua_hook *lua_hook_add(struct lua_hook *head, const char *name)
{
    struct lua_hook *new;
    struct lua_hook *current;

    new = malloc(sizeof(struct lua_hook));

    if ( new == NULL ) 
        return NULL;

    new->name = name;
    new->next = NULL;

    if ( head == NULL ) {
        head = new;
    } else {
        current = head;
        
        while(current->next != NULL) {
            current = current->next;
        }

        current->next = new;
    }

    return head;
}
 

static int backtest_lua_hook(char *callback, float number) 
{

    if(lua_hook_find(context->hooks, callback) != 0)
        return -1;

    lua_getglobal(context->lua_state, callback);
    lua_pushnumber(context->lua_state, number);
    lua_call(context->lua_state, 1, 0);

    return 0;
}

static int backtest_lua_init(const char *filepath) 
{
    struct lua_State *L = NULL;
    struct stat st;

    int i = 0;

    if ( (i =  stat(filepath, &st)) != 0)
       return -1; 

    L = lua_open();

    if (L == NULL)
        return -1;

    luaL_openlibs(L);
    i = luaL_dofile(L, filepath);

    context = malloc(sizeof(lua_context));

    if (context == NULL) 
        return -1;

    context->hooks = NULL;

    for(i = 0; available_hooks[i] != NULL; i++) {
        lua_getglobal(L, available_hooks[i]);

        if ( lua_type(L, -1) == LUA_TFUNCTION)
            context->hooks = lua_hook_add(context->hooks, available_hooks[i]);
    }

    context->filepath = filepath;
    context->lua_state = L;
    context->hook = (void *)&backtest_lua_hook;

    return 0;
}

static int backtest_lua_destroy(void) 
{   
    lua_close(context->lua_state);
    lua_hook_free(context->hooks);    

    free(context);
    return 0;
}
