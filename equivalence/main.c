#include <stdio.h>
#include <stdlib.h>

#include "equivalence.h"


int main()
{
    uint16_t order = 0;
    FILE *fp = fopen("data/h4_2", "r");
    if (!fp) {
        printf("ERROR: open file");
        exit(1);
    }

    fscanf(fp, "%d", &order);
    TInt **matrix = matrix_create(order, order);

    for (size_t i = 0; i < order; ++i) {
        for (size_t j = 0; j < order; ++j) {
            fscanf(fp, "%d", &matrix[i][j]);
        }
    }

    printf("matrix\n");
    debug_print(matrix, order);
    printf("min matrix\n");

    min_matrix(matrix, order);
    TInt **res = get_result(order);
    debug_print(res, order);

    matrix_destroy(matrix, order);

    return 0;
}
