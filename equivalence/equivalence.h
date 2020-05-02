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
TInt **get_result(TUint order);
void reset(TUint order);


#endif