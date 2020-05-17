#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "equivalence.h"


int main()
{
    Matrix mat1 = matrix_from_file("../data/h32_1");
    printf("matrix1\n");
    debug_print(mat1);
    printf("min matrix1\n");
    find_min_matrix(mat1);
    Matrix res1 = get_result();
    debug_print(res1);
    reset();

    Matrix mat2 = matrix_from_file("../data/h32_3");
    printf("matrix2\n");
    debug_print(mat2);
    printf("min matrix2\n");
    find_min_matrix(mat2);
    Matrix res2 = get_result();
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
    reset();
    return 0;
}
