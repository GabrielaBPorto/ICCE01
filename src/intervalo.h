#ifndef _INTERVALO_H
#define _INTERVALO_H

#include <stdio.h>

typedef struct intervalo_t{
    double inferior;
    double superior;
    int unitary;
}intervalo_t;

typedef struct dadosIntervalar_t{
    intervalo_t *intervalos;
    int quantidadeVariavel;
    int quantidadeOperacao;
}dadosIntervalar_t;



void printResults(dadosIntervalar_t dados);

void leituraVariaveis(dadosIntervalar_t dados);
int leituraOperacoes(dadosIntervalar_t dados);

int checkIntervalData(dadosIntervalar_t dados, int posicaoAtual);
int relativoEpsilon(double A, double B);
int relativoUlps(double valorA, double valorB, int maxULPs);

//------------------------------------------------------------------------------
#endif
