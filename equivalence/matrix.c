#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "matrix.h"


Matrix matrix_create(TUint m, TUint n)
{
    Matrix matrix = malloc(sizeof(matrix));
    if (!matrix) {
        printf("ERROR: malloc matrix");
        exit(MEMORY_ERROR);
    }

    matrix->mat = malloc(m * sizeof(*matrix->mat));
    if (!matrix->mat) {
        printf("ERROR: malloc matrix");
        exit(MEMORY_ERROR);
    }
    for (size_t i = 0; i < m; ++i) {
        matrix->mat[i] = malloc(n * sizeof(**matrix->mat));
        if (!matrix->mat[i]) {
            free(matrix->mat);
            printf("ERROR: malloc matrix");
            exit(MEMORY_ERROR);
        }
    }

    matrix->n = n;
    matrix->m = m;

    return matrix;
}

Vector vector_create(TUint order)
{
    Vector vec = malloc(sizeof(vec));
    if (!vec) {
        printf("ERROR: malloc vector\n");
        exit(MEMORY_ERROR);
    }

    vec->v = malloc(order * sizeof(vec->v));
    if (!vec->v) {
        printf("ERROR: malloc vector\n");
        exit(MEMORY_ERROR);
    }

    vec->n = order;

    return vec;
}

void matrix_destroy(Matrix matrix)
{
    for (size_t i = 0; i < matrix->m; ++i) {
        free(matrix->mat[i]);
    }
    free(matrix->mat);
    free(matrix);
}

void vector_destroy(Vector vec)
{
    free(vec->v);
    free(vec);
}

TInt matrix_size(Matrix matrix)
{
    return matrix->n;
}

void debug_print(Matrix matrix)
{
    for (size_t i = 0; i < matrix->n; ++i) {
        for (size_t j = 0; j < matrix->m; ++j) {
            printf("%d ", matrix->mat[i][j]);
        }
        printf("\n");
    }
}

bool matrisequal(Matrix mat1, Matrix mat2)
{
    for (size_t i = 0; i < mat1->n; ++i) {
        for (size_t j = 0; j < mat1->m; ++j) {
            if (mat1->mat[i][j] != mat2->mat[i][j])
                return false;
        }
    }
    return true;
}

bool rowequal(Matrix mat1, TUint row1, Matrix mat2, TUint row2)
{
    for (size_t i = 0; i < mat1->n; ++i) {
        if (mat1->mat[row1][i] != mat2->mat[row2][i])
            return false;
    }
    return true;
}

void matriscopy(Matrix destmat, Matrix srcmat)
{
    for (size_t i = 0; i < destmat->n; ++i) {
        for (size_t j = 0; j < destmat->m; ++j) {
            destmat->mat[i][j] = srcmat->mat[i][j];
        }
    }
}

Matrix matrix_xor(Matrix mat1, Matrix mat2)
{
    TUint order = mat1->n;
    Matrix mat_xor = matrix_create(order, order);
    for (size_t i = 0; i < order; ++i) {
        for (size_t j = 0; j < order; ++j) {
            mat_xor->mat[i][j] = mat1->mat[i][j] ^ mat2->mat[i][j];
        }
    }
    return mat_xor;
}

Matrix matrix_from_file(const char *file_path)
{
    FILE *fp = fopen(file_path, "r");
    if (!fp) {
        printf("ERROR: open file");
        exit(OPEN_FILE_ERROR);
    }

    TUint order;
    fscanf(fp, "%d", &order);

    Matrix matrix = matrix_create(order, order);
    for (size_t i = 0; i < order; ++i) {
        for (size_t j = 0; j < order; ++j) {
            fscanf(fp, "%d", &matrix->mat[i][j]);
        }
    }
    fclose(fp);

    return matrix;
}