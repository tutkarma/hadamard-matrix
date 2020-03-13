#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>

int16_t **A;
int16_t *RC;

uint64_t ro(int16_t **matrix, uint16_t m, uint16_t n);
uint64_t ro2(int16_t *row, uint16_t sz);
void matriscopy(int16_t **destmat, int16_t **srcmat, uint16_t order);
void min_matrix(int16_t **H0, uint16_t order);
void negation_column(int16_t **matrix, uint16_t order, uint16_t column);
void negation_row(int16_t **matrix, uint16_t order, uint16_t row);
int16_t **normalize(int16_t **matrix, uint16_t order);
int16_t **matrix_create(uint16_t m, uint16_t n);
void matrix_destroy(int16_t **matrix, uint16_t order);
void debug_print(int16_t **matrix, uint16_t order);
void swap_rows(int16_t **matrix, uint16_t order, uint16_t row1, uint16_t row2);
void swap_columns(int16_t **matrix, uint16_t order, uint16_t col1, uint16_t col2);


uint64_t ro(int16_t **matrix, uint16_t m, uint16_t n)
{
    uint64_t res = 0;
    for (size_t i = 0; i < m; ++i) {
        for (size_t j = 0; j < n; ++j) {
            res += matrix[i][j] * pow(2, n * (m - i) + (n - j));
        }
    }
    return res;
}

uint64_t ro2(int16_t *row, uint16_t sz)
{
    uint64_t res = 0;
    for (size_t i = 0; i < sz; ++i) {
        res += row[i] * pow(2, sz * (sz - i));
    }
    return res;
}

void matriscopy(int16_t **destmat, int16_t **srcmat, uint16_t order)
{
    for (size_t i = 0; i < order; ++i) {
        for (size_t j = 0; j < order; ++j) {
            destmat[i][j] = srcmat[i][j];
        }
    }
}

void swap_rows(int16_t **matrix, uint16_t order, uint16_t row1, uint16_t row2)
{
    for (size_t i = 0; i < order; ++i) {
        int16_t tmp = matrix[row2][i];
        matrix[row2][i] = matrix[row1][i];
        matrix[row1][i] = tmp;
    }
}

void swap_columns(int16_t **matrix, uint16_t order, uint16_t col1, uint16_t col2)
{
    for (size_t i = 0; i < order; ++i) {
        int16_t tmp = matrix[i][col2];
        matrix[i][col2] = matrix[i][col1];
        matrix[i][col1] = tmp;
    }
}

void column_sort(int16_t **matrix, uint16_t order, uint16_t r)
{
    return;
}

bool array_equal(int16_t *arr1, int16_t *arr2, uint16_t order)
{
    for (size_t i = 0; i < order; ++i) {
        if (arr1[i] != arr2[i])
            return false;
    }
    return true;
}

void core(int16_t **H, uint16_t order, uint16_t r, bool flag)
{
    if (r == order) {
        column_sort(H, order, r);
    }

    if (flag || ro2(H[r], order) < ro2(A[r], order)) {
        for (size_t i = 0; i < order; ++i) {
            A[r][i] = H[r][i];
        }
    }

    int16_t **M = matrix_create(1, order);
    for (size_t i = 0; i < order; ++i) {
        M[0][i] = 1;
    }

    uint16_t k = -1;

    for (size_t i = r; r < order; ++i) {
        swap_rows(H, order, r, i);
        column_sort(H, order, r);
        if (ro2(H[r], order) == ro(M, 1, order)) {
            ++k;
            RC[k] = i;
        }
        if (ro2(H[r], order) < ro(M, 1, order)) {
            k = 0;
            RC[k] = i;
            for (size_t j = 0; j < order; ++j) {
                M[0][j] = H[r][j];
            }
        }
        swap_rows(H, order, r, i);
    }

    if (flag || ro(M, 1, order) < ro2(A[r], order)) {
        for (size_t i = 0; i < order; ++i) {
            A[r][i] = M[0][i];
        }
        swap_rows(H, order, r, RC[0]);
        column_sort(H, order, r);
        if (r < order)
            core(H, order, r + 1, true);
        swap_rows(H, order, r, RC[0]);
        for (size_t i = 0; i < k; ++i) {
            swap_rows(H, order, r, RC[i]);
            column_sort(H, order, r);
            if (r < order)
                core(H, order, r + 1, false);
            swap_rows(H, order, r, RC[i]);
        }
    }

    if (!flag && array_equal(M[0], A[r], order)) {
        for (size_t i = 0; i < k; ++i) {
            swap_rows(H, order, r, RC[i]);
            column_sort(H, order, r);
            if (r < order)
                core(H, order, r + 1, false);
            swap_rows(H, order, r, RC[i]);
        }
    }
}


void min_matrix(int16_t **H0, uint16_t order)
{
    RC = malloc(order * sizeof(*RC));
    A = normalize(H0, order);
    int16_t **H;
    matriscopy(H, H0, order);

    for (size_t j = 0; j < order; ++j) {
        swap_columns(H, order, 0, j);
        
        for (size_t i = 0; i < order; ++i) {
            swap_rows(H, order, 0, i);
            H = normalize(H, order);
            core(H, order, 2, false);
            swap_rows(H, order, 0, i);
        }

        H = H0;
    }
}

void negation_column(int16_t **matrix, uint16_t order, uint16_t column)
{
    for (size_t i = 0; i < order; ++i) {
        if (matrix[i][column] == 0)
            matrix[i][column] = 1;
        else
            matrix[i][column] = 0;
    }
}

void negation_row(int16_t **matrix, uint16_t order, uint16_t row)
{
    for (size_t i = 0; i < order; ++i) {
        if (matrix[row][i] == 0)
            matrix[row][i] = 1;
        else
            matrix[row][i] = 0;
    }
}


int16_t **normalize(int16_t **matrix, uint16_t order)
{
    int16_t **norm_matrix;
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

int16_t **matrix_create(uint16_t m, uint16_t n)
{
    int16_t **matrix;
    matrix = malloc(m * sizeof(*matrix));
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

void matrix_destroy(int16_t **matrix, uint16_t order)
{
    for (size_t i = 0; i < order; ++i) {
        free(matrix[i]);              
    } 
    free(matrix);
}

void debug_print(int16_t **matrix, uint16_t order)
{
    for (size_t i = 0; i < order; ++i) {
         for (size_t j = 0; j < order; ++j) {
            printf("%d ", matrix[i][j]);                       
         }
         printf("\n");                      
     }
}

int main()
{
    uint16_t order = 0;
    int16_t **matrix;
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

