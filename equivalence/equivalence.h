#ifndef _EQUIVALENCE_H_
#define _EQUIVALENCE_H_

#include <stdbool.h>
#include <inttypes.h>

#include "matrix.h"


void min_matrix(Matrix H0);
Matrix get_result(TUint order);
void reset(TUint order);


#endif