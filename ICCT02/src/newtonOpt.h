#ifndef __NEWTONOPT_H__
#define __NEWTONOPT_H__

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
FILE* trataSaidaOpt(int argc, char *argv[]);
void alocacaoVariaveisOpt(int n);
void leituraEquacoesOpt(int n, FILE *input);
void leituraVariaveisOpt(int n, FILE *input);
double escreveDerivadasParciaisOpt(int n, FILE *output);
void liberacaoMemoriaUsadaOpt(int n);
double newtonMethodOpt(int n, double *tempos);
double jacobianaMetodoOpt(int n);
void impressaoResultadosOpt(int  n, FILE *output);
double gaussPivotearParcialOpt(int n);
double maxOpt(double *vetor, int n);
void calculaResultadoMatrizJacobianaOpt(int n);
void trocaLinhasOpt(int n, int i, int k);
double eliminacaoGaussOpt(int n, int i);
void calculoResultadoXOpt(int n);
int trabalho2(FILE *input,FILE *output);

#endif // __NEWTOPT_H__