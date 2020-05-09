#include <stdio.h>
#include <stdlib.h>

#include "matrix.h"


int main(int argc, char** argv)
{
    if (argc != 3) {
        printf("First arg  -- matrix1 file\n");
        printf("Second arg -- matrix2 file\n");
        exit(0);
    }

    const char *mat1_file = argv[1];
    const char *mat2_file = argv[2];

    Matrix mat1 = matrix_from_file(mat1_file);
    Matrix mat2 = matrix_from_file(mat2_file);
    Matrix res = matrix_xor(mat1, mat2);
    debug_print(res);
    matrix_destroy(mat1);
    matrix_destroy(mat2);
    matrix_destroy(res);
    return 0;
}