#ifndef __NEWTON_H__
#define __NEWTON_H__

#include <stdlib.h>
#include <stdio.h>
#include "utils.h"
#include <sys/time.h>

#define TAM_BUFFER 256

// Declaração de variaveis globais.
FILE *output, *input;
double *resultados, *jacobiana, *resultadoJacobiana, helper, maxJacobiana, maxEval, epsilon, *resultadoEquacoes;
char equacoes[TAM_BUFFER][TAM_BUFFER], temp[TAM_BUFFER], **variaveis;
void **derivadas, *eval;
int maxIter;

// Funções 
FILE* trataSaida(int argc, char *argv[]);
void alocacaoVariaveis(int n);
void leituraEquacoes(int n);
void leituraVariaveis(int n);
double escreveDerivadasParciais(int n);
void liberacaoMemoriaUsada(int n);
double newtonMethod(int n, double *tempos);
double jacobianaMetodo(int n);
void impressaoResultados(int  n);
double gaussPivotearParcial(int n);
void troca(double *a, double *b);
double max(double *vetor, int n);
void calculaResultadoMatrizJacobiana(int n);
void trocaLinhas(int n, int i, int k);
double eliminacaoGauss(int n, int i);
void calculoResultadoX(int n);


#endif // __NEWTON_H__