#ifndef _EQUIVALENCE_H_
#define _EQUIVALENCE_H_

#include <stdbool.h>
#include <inttypes.h>

#include "matrix.h"


// void min_matrix(Matrix H0);
int find_min_matrix(Matrix H0, struct timespec *max_wait);
void *min_matrix(void *arg);
Matrix get_result();
void reset();


#endif