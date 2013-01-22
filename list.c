#include "list.h"

void list_free(list *head) 
{
    list *p, *next;

    for (p = head; p != NULL; p = next) {
        next = p->next;
        free(p);
    }
}

int list_find(list *head, const char *key, compare_function compare)
{
    list *current;

    current = head;

    while(current != NULL) {
        if((int)compare(current->key, key) == 0)
            return 0;
        current = current->next;
    }

    return -1;
}

list *list_add(list *head, const char *key)
{
    list *new, *current;

    new = malloc(sizeof(list));

    if ( new == NULL ) 
        return NULL;

    new->key = key;
    new->next = NULL;

    if ( head == NULL ) {
        head = new;
    } else {
        current = head;
        
        while(current->next != NULL)
            current = current->next;

        current->next = new;
    }

    return head;
}
