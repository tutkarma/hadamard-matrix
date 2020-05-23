#ifndef _MATRIX_H_
#define _MATRIX_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>

#define MEMORY_ERROR 1
#define OPEN_FILE_ERROR 2

typedef int32_t TInt;
typedef uint32_t TUint;

typedef struct _matrix {
    TInt **mat;
    TUint n;
    TUint m;
} *Matrix;

typedef struct _vector {
    TInt *v;
    TUint n;
} *Vector;

Matrix matrix_create(TUint m, TUint n);
Vector vector_create(TUint order);
void matrix_destroy(Matrix matrix);
void vector_destroy(Vector vec);
TUint matrix_size(Matrix matrix);
void matriscopy(Matrix destmat, Matrix srcmat);
bool matrisequal(Matrix mat1, Matrix mat2);
bool rowequal(Matrix mat1, TUint row1, Matrix mat2, TUint row2);
void debug_print(Matrix matrix);
Matrix matrix_xor(Matrix mat1, Matrix mat2);
Matrix matrix_from_file(const char *file_path);

#endif