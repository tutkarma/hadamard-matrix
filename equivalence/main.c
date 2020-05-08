#include <stdio.h>
#include <stdlib.h>

#include "equivalence.h"


void read_matrix(FILE *fp, Matrix matrix)
{
    TUint order = matrix->n;
    for (size_t i = 0; i < order; ++i) {
        for (size_t j = 0; j < order; ++j) {
            fscanf(fp, "%d", &matrix->mat[i][j]);
        }
    }
}

int main()
{
    int32_t order = 0, order2 = 0;
    FILE *fp = fopen("../data/h16_1", "r");
    if (!fp) {
        printf("ERROR: open file");
        exit(1);
    }

    fscanf(fp, "%d", &order);
    Matrix mat1 = matrix_create(order, order);
    read_matrix(fp, mat1);

    printf("matrix1\n");
    debug_print(mat1);
    printf("min matrix1\n");
    min_matrix(mat1);
    Matrix res1 = get_result(order);
    debug_print(res1);

    reset(order);
    fclose(fp);

    FILE *fp2 = fopen("../data/h16_2", "r");
    if (!fp2) {
        printf("ERROR: open file");
        exit(1);
    }

    fscanf(fp2, "%d", &order2);
    Matrix mat2 = matrix_create(order2, order2);
    read_matrix(fp2, mat2);

    printf("matrix2\n");
    debug_print(mat2);
    printf("min matrix2\n");
    min_matrix(mat2);
    Matrix res2 = get_result(order2);
    debug_print(res2);

    if (matrisequal(res1, res2)) {
        printf("Min matrices are equal\n");
    } else {
        printf("Min matrices are not equal\n");
    }

    matrix_destroy(mat1);
    matrix_destroy(res1);
    matrix_destroy(mat2);
    matrix_destroy(res2);
    reset(order);
    return 0;
}
