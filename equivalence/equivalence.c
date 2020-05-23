#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <pthread.h>
#include <errno.h>

#include "equivalence.h"


Matrix A;
Vector RC;
Matrix H;
uint64_t iteration;

pthread_mutex_t calculating = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t done = PTHREAD_COND_INITIALIZER;

void core(TUint order, TUint r, bool flag);
void sort(Matrix matrix, TUint l, TUint r);
void column_sort(Matrix matrix);
void merge(Matrix matrix, TUint l, TUint m, TUint r);
TInt column_comp(Matrix matrix, TUint col1, TUint col2);
void normalize(Matrix matrix);
void swap_rows(Matrix matrix, TUint row1, TUint row2);
void swap_columns(Matrix matrix, TUint col1, TUint col2);
uint64_t ro2(Matrix matrix, TUint row);


uint64_t ro(Matrix matrix)
{
    uint64_t res = 0;
    for (size_t i = 0; i < matrix->m; ++i) {
        for (size_t j = 0; j < matrix->n; ++j) {
            res += matrix->mat[i][j] * pow(2, matrix->n * (matrix->m - i) + (matrix->n - j));
        }
    }
    return res;
}

// костыль
uint64_t ro2(Matrix matrix, TUint row)
{
    uint64_t res = 0;
    for (size_t i = 0; i < matrix->n; ++i) {
        res += matrix->mat[row][i] * pow(2, matrix->n * (matrix->n - i));
    }
    return res;
}

void swap_rows(Matrix matrix, TUint row1, TUint row2)
{
    for (size_t i = 0; i < matrix->n; ++i) {
        TInt tmp = matrix->mat[row2][i];
        matrix->mat[row2][i] = matrix->mat[row1][i];
        matrix->mat[row1][i] = tmp;
    }
}

void swap_columns(Matrix matrix, TUint col1, TUint col2)
{
    for (size_t i = 0; i < matrix->n; ++i) {
        TInt tmp = matrix->mat[i][col2];
        matrix->mat[i][col2] = matrix->mat[i][col1];
        matrix->mat[i][col1] = tmp;
    }
}

TInt column_comp(Matrix matrix, TUint col1, TUint col2)
{
    for (size_t i = 0; i < matrix->n; ++i) {
        if (matrix->mat[i][col1] != matrix->mat[i][col2]) {
            return matrix->mat[i][col1] - matrix->mat[i][col2];
        }
    }
    return 0;
}

void merge(Matrix matrix, TUint l, TUint m, TUint r)
{
    TUint order = matrix->n;
    TUint i = l, j = m + 1, k = l;
    TUint n1 = m - l + 1;
    TUint n2 = r - m;
    Matrix tmp = matrix_create(order, order);
    matriscopy(tmp, matrix);

    while (i < n1 && j < n2) {
        if (column_comp(tmp, i, j) <= 0) {
            for (size_t l = 0; l < order; ++l) {
                matrix->mat[l][k] = tmp->mat[l][i];
            }
            ++i;
        } else {
            for (size_t l = 0; l < order; ++l) {
                matrix->mat[l][k] = tmp->mat[l][j];
            }
            ++j;
        }
        ++k;
    }

    while (i < n1) {
        for (size_t l = 0; l < order; ++l) {
            matrix->mat[l][k] = tmp->mat[l][i];
        }
        ++i;
        ++k;
    }

    while (j < n2) {
        for (size_t l = 0; l < order; ++l) {
            matrix->mat[l][k] = tmp->mat[l][j];
        }
        ++j;
        ++k;
    }

    matrix_destroy(tmp);
}

void sort(Matrix matrix, TUint l, TUint r)
{
    if (l < r) {
        TUint m = l + (r - l) / 2;
        sort(matrix, l, m);
        sort(matrix, m + 1, r);
        merge(matrix, l, m, r);
    }
}

void column_sort(Matrix matrix)
{
    sort(matrix, 0, matrix->n - 1);
}

void core(TUint order, TUint r, bool flag)
{
    ++iteration;
    if (iteration == 10000) {
        pthread_cancel(pthread_self());
    }

    if (r >= order - 1) {
        column_sort(H);

        if (flag || ro2(H, r) < ro2(A, r)) {
            for (size_t i = 0; i < order; ++i) {
                A->mat[r][i] = H->mat[r][i];
            }
        }
        return;
    }

    Matrix M = matrix_create(1, order);
    for (size_t i = 0; i < order; ++i) {
        M->mat[0][i] = 1;
    }

    TUint k = -1;

    for (size_t i = r; i < order; ++i) {
        swap_rows(H, r, i);
        column_sort(H);
        if (ro2(H, r) == ro2(M, 0)) {
            ++k;
            RC->v[k] = i;
        }

        if (ro2(H, r) < ro2(M, 0)) {
            k = 0;
            RC->v[k] = i;
            for (size_t j = 0; j < order; ++j) {
                M->mat[0][j] = H->mat[r][j];
            }
        }

        swap_rows(H, r, i);
    }

    if (flag || ro2(M, 0) < ro2(A, r)) {
        for (size_t i = 0; i < order; ++i) {
            A->mat[r][i] = M->mat[0][i];
        }

        swap_rows(H, r, RC->v[0]);
        column_sort(H);
        core(order, r + 1, true);
        swap_rows(H, r, RC->v[0]);

        for (size_t i = 1; i < k; ++i) {
            swap_rows(H, r, RC->v[i]);
            column_sort(H);
            core(order, r + 1, false);
            swap_rows(H, r, RC->v[i]);
        }
    }

    if (!flag && rowequal(M, 0, A, r)) {
        for (size_t i = 0; i < k; ++i) {
            swap_rows(H, r, RC->v[i]);
            column_sort(H);
            core(order, r + 1, false);
            swap_rows(H, r, RC->v[i]);
        }
    }

    matrix_destroy(M);
}

void *min_matrix(void *arg)
{
    iteration = 0;
    int oldtype;
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &oldtype);

    Matrix H0 = arg;
    TUint order = H0->m;
    RC = vector_create(order);
    for (size_t i = 0; i < order; ++i) {
        RC->v[i] = i;
    }

    A = matrix_create(order, order);
    matriscopy(A, H0);
    normalize(A);
    H = H0;

    for (size_t j = 0; j < order; ++j) {
        swap_columns(H, 0, j);
        for (size_t i = 0; i < order; ++i) {
            swap_rows(H, 0, i);
            normalize(H);
            core(order, 1, false);
            swap_rows(H, 0, i);
        }
        H = H0;
    }

    pthread_cond_signal(&done);
    vector_destroy(RC);
    return NULL;
}

void negation_column(Matrix matrix, TUint column)
{
    for (size_t i = 0; i < matrix->n; ++i) {
        if (matrix->mat[i][column] == 0)
            matrix->mat[i][column] = 1;
        else
            matrix->mat[i][column] = 0;
    }
}

void negation_row(Matrix matrix, TUint row)
{
    for (size_t i = 0; i < matrix->n; ++i) {
        if (matrix->mat[row][i] == 0)
            matrix->mat[row][i] = 1;
        else
            matrix->mat[row][i] = 0;
    }
}

void normalize(Matrix matrix)
{
    TUint order = matrix->n;
    // normalize row
    for (size_t i = 0; i < order; ++i) {
        if (matrix->mat[0][i] != 0) {
            negation_column(matrix, i);
        }
    }

    // normalize column
    for (size_t i = 0; i < order; ++i) {
        if (matrix->mat[i][0] != 0) {
            negation_row(matrix, i);
        }
    }
}

Matrix get_result()
{
    TUint order = matrix_size(A);
    Matrix mat = matrix_create(order, order);
    matriscopy(mat, A);
    return mat;
}

void reset()
{
    matrix_destroy(A);
}

int find_min_matrix(Matrix H0, uint64_t time_limit)
{
    struct timespec abs_time;

    pthread_attr_t tattr;
    pthread_t tid;
    int err;

    pthread_mutex_lock(&calculating);
    clock_gettime(CLOCK_REALTIME, &abs_time);
    abs_time.tv_sec += time_limit;
    abs_time.tv_nsec += time_limit;

    pthread_attr_init(&tattr);
    pthread_attr_setdetachstate(&tattr, PTHREAD_CREATE_DETACHED);
    pthread_create(&tid, &tattr, min_matrix, H0);

    err = pthread_cond_timedwait(&done, &calculating, &abs_time);

    if (err == ETIMEDOUT)
        fprintf(stderr, "%s: calculation timed out\n", __func__);
    pthread_mutex_unlock(&calculating);
    pthread_detach(tid);

    return err;
}
