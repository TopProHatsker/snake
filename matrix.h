#ifndef MATRIX_H
#define MATRIX_H

#include <stdlib.h>
#include <stdint.h>

struct matrix {
    uint64_t r_num;
    uint64_t c_num;
    char** str;
};

struct matrix mtr_create ();
void mtr_free (struct matrix* mtr);
void mtr_init (struct matrix* mtr, uint64_t x, uint64_t y);

#endif // MATRIX_H
