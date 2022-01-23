#include <stdint.h>
#include <stdbool.h>
#include "matrix.h"

struct matrix mtr_create () {
    struct matrix mtr = {
        .c_num = 0,
        .r_num = 0,
        .str = NULL
    };
    return mtr;
}

void mtr_free (struct matrix* mtr) {
    for (uint64_t i = 0; i < mtr->r_num; i++) {
        free(mtr->str[i]);
    }
    free(mtr->str);
    mtr->str = NULL;
    mtr->c_num = 0;
    mtr->r_num = 0;
}


void mtr_init (struct matrix* mtr, uint64_t x, uint64_t y) {
    mtr->c_num = x;
    mtr->r_num = y;

    mtr->str = calloc(mtr->r_num, sizeof(uint64_t*));

    for (uint64_t i = 0; i < mtr->r_num; i++) {
        mtr->str[i] = calloc(mtr->c_num, sizeof(uint64_t));
    }
    for (uint64_t i = 0; i < mtr->r_num; i++) {
        for (uint64_t j = 0; j < mtr->c_num; j++) {
            mtr->str[i][j] = ' ';
        }
    }
}
