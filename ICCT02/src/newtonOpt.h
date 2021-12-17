#ifndef __NEWTONOPT_H__
#define __NEWTONOPT_H__

#include <stdlib.h>
#include <stdio.h>
#include "utils.h"
#include <sys/time.h>
#include <likwid.h>

#define TAM_BUFFER 256

// Declaração de variaveis globais.
double *resultadosOpt, *jacobianaOpt, *resultadoJacobianaOpt, *resultadoEquacoes;
char **variaveis;
void **derivadas, **equacoes;


// Funções 
FILE* trataSaidaOpt(int argc, char *argv[]);
void alocacaoVariaveisOpt(int n);
void leituraEquacoesOpt(int n, FILE *input);
void leituraVariaveisOpt(int n, FILE *input, double *epsilon, int *maxIter);
double escreveDerivadasParciaisOpt (int n, FILE *output);
void liberacaoMemoriaUsadaOpt(int n);
double newtonMethodOpt(int n, double *tempos);
double jacobianaMetodoOpt(int n, double *jacobiana, void **derivadas, char **variaveis, 
						  double *resultados, double *resultadoEquacoes);
void impressaoResultadosOpt(int  n, FILE *output, double *resultados);
double gaussPivotearParcialOpt(int n);
double maxOpt(double *vetor, int n);
void calculaResultadoMatrizJacobianaOpt(int n);
void trocaLinhasOpt(int n, int i, int k);
double eliminacaoGaussOpt(int n, int i, double *jacobiana, double *resultadoEquacoes);
void calculoResultadoXOpt(int n);
int trabalho2(FILE *input, FILE *output);

#endif // __NEWTOPT_H__