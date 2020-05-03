#include <stdio.h>
#include <stdlib.h>

#include "equivalence.h"


void read_matrix(FILE *fp, TInt **matrix, TInt order)
{
    for (size_t i = 0; i < order; ++i) {
        for (size_t j = 0; j < order; ++j) {
            fscanf(fp, "%d", &matrix[i][j]);
        }
    }
}

int main()
{
    int32_t order = 0, order2 = 0;
    FILE *fp = fopen("data/h4_1", "r");
    if (!fp) {
        printf("ERROR: open file");
        exit(1);
    }

    fscanf(fp, "%d", &order);
    TInt **mat1 = matrix_create(order, order);
    read_matrix(fp, mat1, order);

    printf("matrix1\n");
    debug_print(mat1, order);
    printf("min matrix1\n");
    min_matrix(mat1, order);
    TInt **res1 = get_result(order);
    debug_print(res1, order);

    reset(order);
    fclose(fp);

    FILE *fp2 = fopen("data/h4_2", "r");
    if (!fp2) {
        printf("ERROR: open file");
        exit(1);
    }

    fscanf(fp2, "%d", &order2);
    TInt **mat2 = matrix_create(order2, order2);
    read_matrix(fp2, mat2, order2);

    printf("matrix2\n");
    debug_print(mat2, order2);
    printf("min matrix2\n");
    min_matrix(mat2, order2);
    TInt **res2 = get_result(order2);
    debug_print(res2, order2);

    if (matrisequal(res1, res2, order2)) {
        printf("Min matrices are equal\n");
    } else {
        printf("Min matrices are not equal\n");
    }

    matrix_destroy(mat1, order);
    matrix_destroy(res1, order);
    matrix_destroy(mat2, order2);
    matrix_destroy(res2, order2);
    reset(order);
    return 0;
}
