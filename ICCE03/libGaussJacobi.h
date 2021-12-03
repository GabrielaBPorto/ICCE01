#ifndef __LIB_GAUSS_JACOBI__
#define __LIB_GAUSS_JACOBI__

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define MAXIT 50

void gaussJacobi(FILE *f_in, FILE *f_out);
void gaussJacobiOpt(FILE *f_in, FILE *f_out);

#endif
