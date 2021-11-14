#ifndef __NEWTON_H__
#define __NEWTON_H__

#include <stdlib.h>
#include <stdio.h>
#include "utils.h"
#include <sys/time.h>

#define TAM_BUFFER 256

// Declaração de variaveis globais.
FILE *output, *input;
double *resultados, *jacobiana, *resultadoJacobiana, helper, maxJacobiana, maxEval;
char equacoes[TAM_BUFFER][TAM_BUFFER], temp[TAM_BUFFER], **variaveis;
void **derivadas, *eval;
double epsilon;
int maxIter, end, iter;

// Funções 
FILE* trataSaida(int argc, char *argv[]);
void alocacaoVariaveis(int n);
void leituraEquacoes(int n);
void leituraVariaveis(int n);
double escreveDerivadasParciais(int n);
void liberacaoMemoriaUsada(int n);
void jacobiano(int n);


#endif // __NEWTON_H__
