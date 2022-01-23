#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

list l_create();
void l_add(list* arr, struct point pos);
void l_print(list* arr);
void l_free(list* arr);
void l_del(list* arr, elem* one);
uint64_t l_len(list* arr);
bool l_check_coll(list* arr);


list l_create() {
    list one = {
        .start = NULL,
        .end = NULL
    };
    return one;
}

uint64_t l_len(list* arr) {
    if(arr->start == NULL) {
        return 0;
    }

    elem* current = arr->start;
    uint64_t l = 0;

    do {
        l++;
        current = current->next;
    } while (current != NULL);

    return l;
}

bool l_check_coll(list* arr) {
    if(arr->start == NULL || arr->start->next == NULL) {
        return 0;
    }

    elem* head = arr->start;
    elem* current = arr->start->next;

    do {
        if (
                head->pos.x == current->pos.x &&
                head->pos.y == current->pos.y
        ) {
            return true;
        }
        current = current->next;
    } while (current != NULL);

    return false;
}

// New tail
void l_add(list* arr, struct point pos) {
    elem* new = calloc(1, sizeof(elem));
    new->pos = pos;
    new->next = NULL;
    new->prev = arr->end;

    if(arr->start == NULL) {
        arr->start = new;
    } else {
        arr->end->next = new;   // old last element
    }

    arr->end = new;
}

// New head
void l_add_first(list* arr, struct point pos) {
    elem* new = calloc(1, sizeof(elem));
    new->pos = pos;
    new->next = arr->start;
    new->prev = NULL;

    if(arr->start == NULL) {
        arr->start = new;
    } else {
        arr->start->prev = new;   // old first element
    }

    arr->start = new;
}

void l_print(list* arr) {
    if(arr->start == NULL) {
        return;
    }

    elem* current = arr->start;

    do {
        printf("x:%d y:%d", current->pos.x, current->pos.y);
        current = current->next;
    } while (current != NULL);
}


void l_free(list* arr) {
    if(arr->start == NULL) {
        arr->end = NULL;
        return;
    }

    elem* current = arr->start;
    elem* next = NULL;

    do {
        next = current->next;
        free(current);
        current = next;
    } while (next != NULL);
}


void l_del(list* arr, elem* one) {

    if(one->prev != NULL) {
        one->prev->next = one->next;
    } else {
        arr->start = one->next;
    }

    if(one->next != NULL) {
        one->next->prev = one->prev;
    } else {
        arr->end = one->prev;
    }

    free(one);
}
