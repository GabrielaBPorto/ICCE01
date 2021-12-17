#ifndef __NEWTON_H__
#define __NEWTON_H__

#include <stdlib.h>
#include <stdio.h>
#include "utils.h"
#include <sys/time.h>
#include <likwid.h>

#define TAM_BUFFER 256

// Declaração de variaveis globais.
double *resultados, *jacobiana, *resultadoJacobiana, helper, maxJacobiana, maxEval, epsilon, *resultadoEquacoes;
char equacoes[TAM_BUFFER][TAM_BUFFER], temp[TAM_BUFFER], **variaveis;
void **derivadas, *eval;
int maxIter;

// Funções 
FILE* trataSaida(int argc, char *argv[]);
void alocacaoVariaveis(int n);
void leituraEquacoes(int n, FILE *input);
void leituraVariaveis(int n, FILE *input);
double escreveDerivadasParciais (int n, FILE *output);
void liberacaoMemoriaUsada(int n);
double newtonMethod(int n, double *tempos);
double jacobianaMetodo(int n);
void impressaoResultados(int  n, FILE *output);
double gaussPivotearParcial(int n);
double max(double *vetor, int n);
void calculaResultadoMatrizJacobiana(int n);
void trocaLinhas(int n, int i, int k);
double eliminacaoGauss(int n, int i);
void calculoResultadoX(int n);
int trabalho1(FILE *input,FILE *output);

#endif // __NEWTON_H__