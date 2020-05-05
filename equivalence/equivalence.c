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

void core(TUint order, TUint r, bool flag);
void sort(TInt **matrix, TUint order, TUint l, TUint r);
void column_sort(TInt **matrix, TUint order);
void merge(TInt **matrix, TUint order, TUint l, TUint m, TUint r);
TInt column_comp(TInt **matrix, TUint order, TUint col1, TUint col2);
void normalize(TInt **matrix, TUint order);
void swap_rows(TInt **matrix, TUint order, TUint row1, TUint row2);
void swap_columns(TInt **matrix, TUint order, TUint col1, TUint col2);
void negation_row(TInt **matrix, TUint order, TUint row);
void negation_column(TInt **matrix, TUint order, TUint column);
uint64_t ro(TInt **matrix, TUint m, TUint n);
uint64_t ro2(TInt *row, TUint sz);


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

void merge(TInt **matrix, TUint order, TUint l, TUint m, TUint r)
{
    TUint i = l, j = m + 1, k = l;
    TUint n1 = m - l + 1;
    TUint n2 = r - m;
    TInt **tmp = matrix_create(order, order);
    matriscopy(tmp, matrix, order);

    while (i < n1 && j < n2) {
        if (column_comp(tmp, order, i, j) <= 0) {
            for (size_t l = 0; l < order; ++l) {
                matrix[l][k] = tmp[l][i];
            }
            ++i;
        } else {
            for (size_t l = 0; l < order; ++l) {
                matrix[l][k] = tmp[l][j];
            }
            ++j;
        }
        ++k;
    }

    while (i < n1) {
        for (size_t l = 0; l < order; ++l) {
            matrix[l][k] = tmp[l][i];
        }
        ++i;
        ++k;
    }

    while (j < n2) {
        for (size_t l = 0; l < order; ++l) {
            matrix[l][k] = tmp[l][j];
        }
        ++j;
        ++k;
    }

    matrix_destroy(tmp, order);
}

void sort(TInt **matrix, TUint order, TUint l, TUint r)
{
    if (l < r) {
        TUint m = l + (r - l) / 2;
        sort(matrix, order, l, m);
        sort(matrix, order, m + 1, r);
        merge(matrix, order, l, m, r);
    }
}

void column_sort(TInt **matrix, TUint order)
{
    sort(matrix, order, 0, order - 1);
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

    TUint k = -1;

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

        for (size_t i = 1; i < k; ++i) {
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

    A = matrix_create(order, order);
    matriscopy(A, H0, order);
    normalize(A, order);
    H = H0;

    for (size_t j = 0; j < order; ++j) {
        swap_columns(H, order, 0, j);
        for (size_t i = 0; i < order; ++i) {
            swap_rows(H, order, 0, i);
            normalize(H, order);
            core(order, 1, false);
            swap_rows(H, order, 0, i);
        }
        H = H0;
    }
    vector_destroy(RC);
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


void normalize(TInt **matrix, TUint order)
{
    // normalize row
    for (size_t i = 0; i < order; ++i) {
        if (matrix[0][i] != 0) {
            negation_column(matrix, order, i);
        }
    }

    // normalize column
    for (size_t i = 0; i < order; ++i) {
        if (matrix[i][0] != 0) {
            negation_row(matrix, order, i);
        }
    }
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
            free(matrix);
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
