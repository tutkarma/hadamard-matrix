#ifndef _EQUIVALENCE_H_
#define _EQUIVALENCE_H_

#include <stdbool.h>
#include <inttypes.h>

typedef int32_t TInt;
typedef uint32_t TUint;


TInt **matrix_create(TUint m, TUint n);
TInt *vector_create(TUint order);
void matrix_destroy(TInt **matrix, TUint order);
void vector_destroy(TInt *vec);
void matriscopy(TInt **destmat, TInt **srcmat, TUint order);
bool matrisequal(TInt **mat1, TInt **mat2, TUint order);
void debug_print(TInt **matrix, TUint order);

void min_matrix(TInt **H0, TUint order);
void core(TUint order, TUint r, bool flag);
void column_sort(TInt **matrix, TUint order);
TInt column_comp(TInt **matrix, TUint order, TUint col1, TUint col2);
void normalize(TInt **matrix, TUint order);
void swap_rows(TInt **matrix, TUint order, TUint row1, TUint row2);
void swap_columns(TInt **matrix, TUint order, TUint col1, TUint col2);
void negation_row(TInt **matrix, TUint order, TUint row);
void negation_column(TInt **matrix, TUint order, TUint column);
uint64_t ro(TInt **matrix, TUint m, TUint n);
uint64_t ro2(TInt *row, TUint sz);
TInt **get_result(TUint order);
void reset(TUint order);


#endif