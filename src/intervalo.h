#ifndef _INTERVALO_H
#define _INTERVALO_H

#include <stdio.h>

//------------------------------------------------------------------------------
// (apontador para) estrutura de dados para representar um grafo
// 
// o grafo tem um nome, que é uma "string"

typedef struct intervalo {
    double inferior;
    double superior;
    double x;
    int isUnitario = 1;
}intervalo;


double max(int numeroElementos, ...);
double min(int numeroElementos, ...);

void imprimeIntervalo(intervalo *vetor, int m);
void leituraVariaveis(intervalo *variaveis, int n);
int leituraOperacoes(intervalo *variaveis,int n, int m);

void multiplicacao(intervalo *vetor, int i, int x, int y);
void divisao(intervalo *vetor, int i, int x, int y);
void subtracao(intervalo *vetor, int i, int x, int y);
void soma(intervalo *vetor, int i, int x, int y);

int verificaIntervalos(intervalo intervalo);

//------------------------------------------------------------------------------
#endif
