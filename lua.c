#include "lua.h"

static inline void lua_hook_free(lua_hook *head) 
{
    lua_hook *p, *next;

    for (p = head; p != NULL; p = next) {
        next = p->next;
        free(p);
    }
}

static inline int lua_hook_find(lua_hook *head, const char *name)
{
    lua_hook *current;

    current = head;
    while(current != NULL) {
        if(strncmp(current->name, name, strlen(current->name)) == 0)
            return 0;
        current = current->next;
    }

    return -1;
}

static inline struct lua_hook *lua_hook_add(lua_hook *head, const char *name)
{
    lua_hook *new, *current;

    new = malloc(sizeof(lua_hook));

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
 
static inline int lua_hook_call(lua_context *self, const char *callback, unsigned long argc, ...) 
{
    int i;
    lua_arg argument;
    va_list arguments;

    if(lua_hook_find(self->hooks, callback) != 0)
        return -1;

    lua_getglobal(self->lua_state, callback);
    va_start(arguments, argc);
    
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
    
    lua_call(self->lua_state, argc, 0);
    return 0;
}


void backtest_lua_destroy(struct lua_context *self) 
{   
    lua_close(self->lua_state);
    lua_hook_free(self->hooks);
    free(self);
}


lua_context *backtest_lua_init(const char *filepath) 
{
    lua_context *context;
    struct lua_State *L = NULL;
    struct stat st;

    int i = 0;

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

        if ( lua_type(L, -1) == LUA_TFUNCTION)
            context->hooks = lua_hook_add(context->hooks, available_hooks[i]);
    }

    context->filepath = filepath;
    context->lua_state = L;
    context->hook = (void *)&lua_hook_call;

    return context;
}
