#ifndef LIST_H
#define LIST_H

#include <stdint.h>
#include <stdbool.h>

struct point {
    int8_t x;
    int8_t y;
};

typedef struct elem {
    struct point pos;
    struct elem* next;
    struct elem* prev;
} elem;

typedef struct list {
    struct elem* start;
    struct elem* end;
} list;


list l_create();
void l_add(list* arr, struct point pos);
void l_add_first(list* arr, struct point pos);
void l_print(list* arr);
void l_free(list* arr);
void l_del(list* arr, elem* one);
uint64_t l_len(list* arr);
bool l_check_coll(list* arr);

#endif // LIST_H
