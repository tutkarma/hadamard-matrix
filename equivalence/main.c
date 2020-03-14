#include <stdio.h>
#include <stdlib.h>

#include "equivalence.h"


int main()
{
    uint16_t order = 0;
    TInt **matrix;
    FILE *fp = fopen("h4_1", "r");
    if (!fp) {
        printf("ERROR: open file");
        exit(1);
    }

    fscanf(fp, "%d", &order);
    matrix = matrix_create(order, order);

    for (size_t i = 0; i < order; ++i) {
        for (size_t j = 0; j < order; ++j) {
            fscanf(fp, "%d", &matrix[i][j]);
        }
    }

    printf("matrix\n");
    debug_print(matrix, order);
//    printf("%lu ", ro2(matrix[0], order));
//    int16_t (* ptr2)[0] = (int16_t (*)[0]) *(matrix);

//    for (size_t i = 0; i < order; ++i) {
//        printf("%d ", ptr2[0][i]);
//    }

//    printf("kek");
//    printf("%lu ", ro(ptr2, 1, order));

    matrix_destroy(matrix, order);

    return 0;
}