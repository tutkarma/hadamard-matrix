#ifndef _EQUIVALENCE_H_
#define _EQUIVALENCE_H_

#include <stdbool.h>
#include <inttypes.h>

typedef int16_t TInt;
typedef uint16_t TUint;


uint64_t ro(TInt **matrix, TUint m, TUint n);
uint64_t ro2(TInt *row, TUint sz);
void matriscopy(TInt **destmat, TInt **srcmat, TUint order);
void min_matrix(TInt **H0, TUint order);
void negation_column(TInt **matrix, TUint order, TUint column);
void negation_row(TInt **matrix, TUint order, TUint row);
TInt **normalize(TInt **matrix, TUint order);
TInt **matrix_create(TUint m, TUint n);
void matrix_destroy(TInt **matrix, TUint order);
void debug_print(TInt **matrix, TUint order);
void swap_rows(TInt **matrix, TUint order, TUint row1, TUint row2);
void swap_columns(TInt **matrix, TUint order, TUint col1, TUint col2);

#endif