#ifndef _EQUIVALENCE_H_
#define _EQUIVALENCE_H_

#include <stdbool.h>
#include <inttypes.h>

#include "matrix.h"


int find_min_matrix(Matrix H0);
void *min_matrix(void *arg);
Matrix get_result();
void reset();


#endif