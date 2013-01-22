#include <string.h>
#include <stdlib.h>

typedef struct list {
    const char *key;
    void *value;
    struct list *next;
} list;

typedef int (*compare_function)(const char *primary, const char *secondary);

void list_free(list *head);
int list_find(list *head, const char *key, compare_function function);
list *list_add(list *head, const char *key);
