#ifndef __NEWTON_H__
#define __NEWTON_H__

#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>

#define TAM_BUFFER 256
FILE *output, *input;


void leituraVariavel(FILE *input, int *n, int *k, double *epsilon, int *maxIter);
FILE* trataSaida(int argc, char *argv[]);

#endif // __NEWTON_H__
