#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdbool.h>

#include "naive_algorithm.h"

#define COMB_SIZE 10000

typedef struct _perm {
    TInt *arr;
    TUint sz;
    TInt *indexes;
    TInt increase;
} *Permutation;

typedef struct _comb {
    TInt arr[COMB_SIZE][COMB_SIZE];
    TUint sz;
    TUint tmp_sz;
} *Combinations;

typedef struct _tmp
{
    TInt arr[100];
    TUint sz;
} *Tmp;


Permutation permutation_create(TUint sz)
{
    Permutation p = malloc(sizeof(*p));
    p->sz = sz;
    p->arr = malloc(sz * sizeof(*p->arr));
    p->indexes = malloc(p->sz * sizeof(*p->indexes));
    for (size_t i = 0; i < sz; ++i) {
        p->arr[i] = i;
        p->indexes[i] = i;
    }

    p->increase = 0;
    return p;
}

void permutation_destroy(Permutation p)
{
    free(p->arr);
    free(p->indexes);
    free(p);
}

bool perm_has_next(Permutation p)
{
    return p->increase != p->sz - 1;
}

void perm_swap(Permutation p, TUint i, TUint j)
{
    TInt tmp = p->indexes[i];
    p->indexes[i] = p->indexes[j];
    p->indexes[j] = tmp;
}

TInt *perm_get_next(Permutation p)
{
    if (p->increase == 0) {
        perm_swap(p, p->increase, p->increase + 1);
        ++p->increase;
        while (p->increase < p->sz - 1 && p->indexes[p->increase] > p->indexes[p->increase + 1]) {
            ++p->increase;
        }
    } else {
        if (p->indexes[p->increase + 1] > p->indexes[0]) {
            perm_swap(p, p->increase + 1, 0);
        } else {
            TInt l = 0;
            TInt r = p->increase;
            TInt m = l + (r - l) / 2;
            TInt t = p->indexes[p->increase + 1];
            while (!(p->indexes[m] < t && p->indexes[m - 1] > t)) {
                if (p->indexes[m] < t) {
                    r = m - 1;
                } else {
                    l = m + 1;
                }
                m = l + (r - l) / 2;
            }
            perm_swap(p, p->increase + 1, m);
        }

        for (size_t i = 0; i < p->increase / 2; ++i) {
            perm_swap(p, i, p->increase - i);
        }

        p->increase = 0;
    }

    TInt *cur_perm = malloc(p->sz * sizeof(*cur_perm));
    for (size_t i = 0; i < p->sz; ++i) {
        cur_perm[i] = p->arr[p->indexes[i]];
    }

    return cur_perm;
}

void make_comb_util(Combinations c, Tmp tmp, TInt n, TInt l, TInt k)
{
    if (k == 0) {
        for (size_t i = 0; i < tmp->sz; ++i) {
            c->arr[c->sz][i] = tmp->arr[i];
        }
        ++c->sz;
        return;
    }

    for (size_t i = l; i <= n; ++i) {
        tmp->arr[tmp->sz] = i;
        ++tmp->sz;
        make_comb_util(c, tmp, n, i + 1, k - 1);
        --tmp->sz;
    }
}

Combinations make_combintaion(TInt k, TInt n)
{
    Combinations c = (Combinations) malloc(sizeof(*c));
    c->sz = 0;
    c->tmp_sz = 0;
    Tmp tmp = malloc(sizeof(*tmp));
    tmp->sz = 0;
    make_comb_util(c, tmp, n, 1, k);
    free(tmp);
    return c;
}


void matrix_from_perm_rows(Matrix H0, Matrix tmp, TInt *cur_perm_rows)
{
    TUint order = matrix_size(H0);

    for (size_t i = 0; i < order; ++i) {
        for (size_t j = 0; j < order; ++j) {
            tmp->mat[i][j] = H0->mat[cur_perm_rows[i]][j];
        }
    }
}

void matrix_from_perm_cols(Matrix H0, Matrix tmp, TInt *cur_perm_cols)
{
    TUint order = matrix_size(H0);
    for (size_t i = 0; i < order; ++i) {
        for (size_t j = 0; j < order; ++j) {
            tmp->mat[i][j] = H0->mat[i][cur_perm_cols[j]];
        }
    }
}

void matrix_from_comb_rows(Matrix m1, Matrix m2, TInt *arr, TUint sz)
{
    matriscopy(m2, m1);
    for (size_t i = 0; i < sz; ++i) {
        negation_row(m2, arr[i] - 1);
    }
}

void matrix_from_comb_cols(Matrix m1, Matrix m2, TInt *arr, TUint sz)
{
    matriscopy(m2, m1);
    for (size_t i = 0; i < sz; ++i) {
        negation_column(m2, arr[i] - 1);
    }
}

Matrix naive_algorithm(Matrix H0)
{
    TUint order = matrix_size(H0);
    Matrix min_mat = matrix_create(order, order);
    matriscopy(min_mat, H0);
    TUint min_ro = ro(H0);
    Matrix tmp1 = matrix_create(order, order);
    Matrix tmp2 = matrix_create(order, order);
    Matrix tmp3 = matrix_create(order, order);
    Matrix tmp4 = matrix_create(order, order);

    Permutation perm_rows = permutation_create(order);
    Permutation perm_cols = permutation_create(order);

    while (perm_has_next(perm_rows)) {
        TInt *cur_perm_rows = perm_get_next(perm_rows);
        matrix_from_perm_rows(H0, tmp1, cur_perm_rows);
        free(cur_perm_rows);
        while (perm_has_next(perm_cols)) {
            TInt *cur_perm_cols = perm_get_next(perm_cols);
            matrix_from_perm_cols(tmp1, tmp2, cur_perm_cols);
            free(cur_perm_cols);

            for (size_t i = 1; i <= order; ++i) {
                Combinations c = make_combintaion(i, order);
                for (size_t j = 0; j < c->sz; ++j) {

                    matrix_from_comb_rows(tmp2, tmp3, c->arr[j], i);

                    for (size_t k = 1; k <= order; ++k) {
                        Combinations c2 = make_combintaion(k, order);

                        for (size_t l = 0; l < c2->sz; ++l) {
                            matrix_from_comb_cols(tmp3, tmp4, c2->arr[l], k);
                            uint64_t cur_ro = ro(tmp4);
                            if (cur_ro < min_ro) {
                                min_ro = cur_ro;
                                matriscopy(min_mat, tmp4);
                            }
                        }
                        free(c2);
                    }
                }
                free(c);
            }
        }
    }

    permutation_destroy(perm_rows);
    permutation_destroy(perm_cols);
    matrix_destroy(tmp1);
    matrix_destroy(tmp2);
    matrix_destroy(tmp3);
    matrix_destroy(tmp4);

    return min_mat;
}
