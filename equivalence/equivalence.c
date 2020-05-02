#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>

#include "equivalence.h"

TInt **A;
TInt *RC;
TInt **H;


uint64_t ro(TInt **matrix, TUint m, TUint n)
{
    uint64_t res = 0;
    for (size_t i = 0; i < m; ++i) {
        for (size_t j = 0; j < n; ++j) {
            res += matrix[i][j] * pow(2, n * (m - i) + (n - j));
        }
    }
    return res;
}

uint64_t ro2(TInt *row, TUint sz)
{
    uint64_t res = 0;
    for (size_t i = 0; i < sz; ++i) {
        res += row[i] * pow(2, sz * (sz - i));
    }
    return res;
}

void matriscopy(TInt **destmat, TInt **srcmat, TUint order)
{
    for (size_t i = 0; i < order; ++i) {
        for (size_t j = 0; j < order; ++j) {
            destmat[i][j] = srcmat[i][j];
        }
    }
}

void swap_rows(TInt **matrix, TUint order, TUint row1, TUint row2)
{
    for (size_t i = 0; i < order; ++i) {
        TInt tmp = matrix[row2][i];
        matrix[row2][i] = matrix[row1][i];
        matrix[row1][i] = tmp;
    }
}

void swap_columns(TInt **matrix, TUint order, TUint col1, TUint col2)
{
    for (size_t i = 0; i < order; ++i) {
        TInt tmp = matrix[i][col2];
        matrix[i][col2] = matrix[i][col1];
        matrix[i][col1] = tmp;
    }
}

TInt column_comp(TInt **matrix, TUint order, TUint col1, TUint col2)
{
    for (size_t i = 0; i < order; ++i) {
        if (matrix[i][col1] != matrix[i][col2]) {
            return matrix[i][col1] - matrix[i][col2];
        }
    }
    return 0;
}

void column_sort(TInt **matrix, TUint order)
{
    for (size_t i = 0; i < order - 1; ++i) {
        for (size_t j = 0; j < order - i - 1; ++j) {
            if (column_comp(matrix, order, j, j + 1) > 0) {
                swap_columns(matrix, order, j, j + 1);
            }
        }
    }
}

bool array_equal(TInt *arr1, TInt *arr2, TUint order)
{
    for (size_t i = 0; i < order; ++i) {
        if (arr1[i] != arr2[i])
            return false;
    }
    return true;
}

void core(TUint order, TUint r, bool flag)
{
    if (r >= order - 1) {
        column_sort(H, order);

        if (flag || ro2(H[r], order) < ro2(A[r], order)) {
            for (size_t i = 0; i < order; ++i) {
                A[r][i] = H[r][i];
            }
        }
        return;
    }

    TInt **M = matrix_create(1, order);
    for (size_t i = 0; i < order; ++i) {
        M[0][i] = 1;
    }

    int16_t k = -1;

    for (size_t i = r; i < order; ++i) {
        swap_rows(H, order, r, i);
        column_sort(H, order);
        if (ro2(H[r], order) == ro2(M[0], order)) {
            ++k;
            RC[k] = i;
        }

        if (ro2(H[r], order) < ro2(M[0], order)) {
            k = 0;
            RC[k] = i;
            for (size_t j = 0; j < order; ++j) {
                M[0][j] = H[r][j];
            }
        }

        swap_rows(H, order, r, i);
    }

    if (flag || ro2(M[0], order) < ro2(A[r], order)) {
        for (size_t i = 0; i < order; ++i) {
            A[r][i] = M[0][i];
        }

        swap_rows(H, order, r, RC[0]);
        column_sort(H, order);
        core(order, r + 1, true);
        swap_rows(H, order, r, RC[0]);

        for (int16_t i = 1; i < k; ++i) {
            swap_rows(H, order, r, RC[i]);
            column_sort(H, order);
            core(order, r + 1, false);
            swap_rows(H, order, r, RC[i]);
        }
    }

    if (!flag && array_equal(M[0], A[r], order)) {
        for (size_t i = 0; i < k; ++i) {
            swap_rows(H, order, r, RC[i]);
            column_sort(H, order);
            core(order, r + 1, false);
            swap_rows(H, order, r, RC[i]);
        }
    }
    matrix_destroy(M, 1);
}


void min_matrix(TInt **H0, TUint order)
{
    RC = vector_create(order);
    for (size_t i = 0; i < order; ++i) {
        RC[i] = i;
    }

    A = normalize(H0, order);
    H = matrix_create(order, order);
    matriscopy(H, H0, order);

    for (size_t j = 0; j < order; ++j) {
        swap_columns(H, order, 0, j);
        for (size_t i = 0; i < order; ++i) {
            swap_rows(H, order, 0, i);
            H = normalize(H, order);
            core(order, 1, false);
            swap_rows(H, order, 0, i);
        }

        H = H0;
    }

    free(RC);
    matrix_destroy(H, order);
}

void negation_column(TInt **matrix, TUint order, TUint column)
{
    for (size_t i = 0; i < order; ++i) {
        if (matrix[i][column] == 0)
            matrix[i][column] = 1;
        else
            matrix[i][column] = 0;
    }
}

void negation_row(TInt **matrix, TUint order, TUint row)
{
    for (size_t i = 0; i < order; ++i) {
        if (matrix[row][i] == 0)
            matrix[row][i] = 1;
        else
            matrix[row][i] = 0;
    }
}


TInt **normalize(TInt **matrix, TUint order)
{
    TInt **norm_matrix;
    norm_matrix = matrix_create(order, order);
    matriscopy(norm_matrix, matrix, order);

    // normalize row
    for (size_t i = 0; i < order; ++i) {
        if (norm_matrix[0][i] != 0) {
            negation_column(norm_matrix, order, i);
        }
    }

    // normalize column
    for (size_t i = 0; i < order; ++i) {
        if (norm_matrix[i][0] != 0) {
            negation_row(norm_matrix, order, i);
        }
    }

    return norm_matrix;
}

TInt **matrix_create(TUint m, TUint n)
{
    TInt **matrix = malloc(m * sizeof(*matrix));
    if (!matrix) {
        printf("ERROR: malloc matrix");
        exit(1);
    }
    for (size_t i = 0; i < m; ++i) {
        matrix[i] = malloc(n * sizeof(**matrix));
        if (!matrix[i]) {
            // осовободить память
            printf("ERROR: malloc matrix");
            exit(1);
        }
    }
    return matrix;
}

TInt *vector_create(TUint order)
{
    TInt *vec = malloc(order * sizeof(*vec));
    if (!vec) {
        printf("ERROR: malloc vector\n");
        exit(1);
    }
    return vec;
}

void matrix_destroy(TInt **matrix, TUint order)
{
    for (size_t i = 0; i < order; ++i) {
        free(matrix[i]);
    }
    free(matrix);
}

void vector_destroy(TInt *vec)
{
    free(vec);
}

void debug_print(TInt **matrix, TUint order)
{
    for (size_t i = 0; i < order; ++i) {
         for (size_t j = 0; j < order; ++j) {
            printf("%d ", matrix[i][j]);
         }
         printf("\n");
     }
}

TInt **get_result(TUint order)
{
    TInt **mat = matrix_create(order, order);
    matriscopy(mat, A, order);
    return mat;
}

void reset(TUint order)
{
    matrix_destroy(A, order);
    matrix_destroy(H, order);
    vector_destroy(RC);
}


bool matrisequal(TInt **mat1, TInt **mat2, TUint order)
{
    for (size_t i = 0; i < order; ++i) {
        for (size_t j = 0; j < order; ++j) {
            if (mat1[i][j] != mat2[i][j])
                return false;
        }
    }
    return true;
}