#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#ifndef LIST_MALLOC
#define LIST_MALLOC(x) malloc(x)
#include <stdlib.h>
#endif // !LIST_MALLOC

#ifndef LIST_FREE
#define LIST_FREE(x) free(x)
#include <stdlib.h>
#endif // !LIST_FREE

typedef struct {
    void *next;
    void *data;
} LinkedList;

void list_push(LinkedList **list, void *value) {
    if (!(*list)) {
        *list = malloc(sizeof(LinkedList));
        **list = (LinkedList){0, value};
        return;
    }

    LinkedList *list_item = *list;

    while (list_item->next) {
        list_item = list_item->next;
    }

    LinkedList *new_elem = LIST_MALLOC(sizeof(LinkedList));
    *new_elem = (LinkedList){0, value};
    list_item->next = new_elem;
}

void *get(LinkedList *list, unsigned index) {
    unsigned i = 0;

    while (i < index) {
        if (list->next)
            list = list->next;
        else
            return 0;
    }

    return list->data;
}

void *list_pop(LinkedList *list) {
    LinkedList *current = list;
    LinkedList *next = list->next;

    while (next) {
        current = next;
        next = next->next;
    }

    void *res = next->data;
    current->next = 0;
    LIST_FREE(next);

    return res;
}

void *list_pop_front(LinkedList **list_ref) {
    void *result = get(*list_ref, 0);
    LinkedList *head = (*list_ref)->next;
    LIST_FREE(*list_ref);
    *list_ref = head;

    return result;
}

int list_length(LinkedList *list) {
    int len = 1;

    while (list->next) {
        len++;
        list = list->next;
    }

    return len;
}

void list_free(LinkedList *list) {
    if (!list)
        return;

    list_free(list->next);
    LIST_FREE(list);
}

void list_free_with_data(LinkedList *list) {
    if (!list)
        return;

    if (list->data)
        LIST_FREE(list->data);

    list_free_with_data(list->next);
    LIST_FREE(list);
}

#endif // !LINKED_LIST_H
