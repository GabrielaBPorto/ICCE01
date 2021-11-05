#ifndef __SL_H__
#define __SL_H__

#include <stdlib.h>
#include <sys/time.h>

#define TAM_BUFFER 100

void leituraVariavel(int *n, int *k, double *epsilon, int *maxIter);

void printarMatrizKDiagonal(double *matriz, int n, int k);

double * gaussSeidel(double *matriz, double*coeficientes, double eps, int n, int k, int maxIter);

// A construção da matriz diagonal se dá para
// A diagonal principal
// As diagonais superiores a partir da banda passada de parametro input
// As diagonais inferior a partir da banda passada de parametro input
// O tamanho da diagonal dentro da matriz
// A quantidade de diagonais a partir do parametro de input
// 
double *construcaoMatrizKDiagonal(int n, int k) ;

// Função para gerar o vetor de coeficientes independentes
double* gerarMatrizIndependente(int n, int k);

#endif // __SL_H__
