#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <math.h>
#include <stdarg.h>
#include "intervalo.h"
#include "operacao.h"

//Função que imprime output
void printResults(dadosIntervalar_t dados){
    for(int i=0;i<(dados.quantidadeVariavel + dados.quantidadeOperacao);i++){
        printf("X%d = [      %.8e,       %.8e]\n", i+1,dados.intervalos[i].inferior, dados.intervalos[i].superior);
    }

    printf("\nNão unitários:\n");
    for(int i = dados.quantidadeVariavel; i < (dados.quantidadeVariavel + dados.quantidadeOperacao); i++){
        if(!dados.intervalos[i].unitary){
            printf("X%d = [      %.8e,       %.8e]\n", i+1,dados.intervalos[i].inferior,dados.intervalos[i].superior);
        }
    }
    return;
}


void leituraVariaveis(dadosIntervalar_t dados){
    double value;
    char temp[4];
    for(int i=0; i< dados.quantidadeVariavel; i++){
        scanf("%s %lf\n",temp, &value);
        dados.intervalos[i].inferior = nextafter(value, -INFINITY);
        dados.intervalos[i].superior = nextafter(value, INFINITY);
    }
}


// Função para realizar o cálculo das operações
int leituraOperacoes(dadosIntervalar_t dados){
    char temp[6];
    char tempA, tempB;
    char operacao;
    int variavelA, variavelB;
    for(int i=dados.quantidadeVariavel; i<(dados.quantidadeOperacao+dados.quantidadeVariavel); i++){
        fgets(temp,6,stdin);
        scanf("%c%d %c %c%d\n",&tempA,&variavelA,&operacao,&tempB,&variavelB);
        dados.intervalos[i].unitary = 1;

        if(operacao == '/')
            divide(dados,i,variavelA,variavelB);
        else if(operacao == '*')
            multiplication(dados,i,variavelA,variavelB);
        else if(operacao == '-')
            subtract(dados,i,variavelA,variavelB);
        else if(operacao == '+')
            sum(dados,i,variavelA, variavelB);

        if (checkIntervalData(dados, i))
            return -1;
    }
    return 0;
}

// A função verifica se o intervalo é unitario, e então parte para verificações de valores
int checkIntervalData(dadosIntervalar_t dados, int posicaoAtual){
    if (isnan(dados.intervalos[posicaoAtual].superior) || isnan(dados.intervalos[posicaoAtual].inferior)){
        return -1;
    }

    if (dados.intervalos[posicaoAtual].inferior > dados.intervalos[posicaoAtual].superior){
        return -1;
    }

    if (isinf(dados.intervalos[posicaoAtual].inferior) == -1  && isinf(dados.intervalos[posicaoAtual].superior) == -1){
        return -1;
    }

    if (isinf(dados.intervalos[posicaoAtual].inferior) == 1  && isinf(dados.intervalos[posicaoAtual].superior) == -1){
        return -1;
    }

    if (isinf(dados.intervalos[posicaoAtual].inferior) == 1  && isinf(dados.intervalos[posicaoAtual].superior) == 1){
        return -1;
    }

    if(!relativoEpsilon(dados.intervalos[posicaoAtual].inferior,dados.intervalos[posicaoAtual].superior) 
        || !relativoUlps(dados.intervalos[posicaoAtual].inferior, dados.intervalos[posicaoAtual].superior, 10)){
        dados.intervalos[posicaoAtual].unitary = 0;
    }

    return 0;
}

int relativoUlps(double valorA, double valorB, int maxULPs){
    if(valorA == valorB){
        return 1;
    }
 
    // Find the difference in ULPs.
    int ulpsDiff = abs(valorA - valorB);

    if (ulpsDiff < maxULPs)
        return 1;
    return 0;
}

// Função auxiliar para conferir relatividade entre intervalos
int relativoEpsilon(double valorA, double valorB){
    if(valorA == valorB){
        return 1;
    }
    double diff, largest, relEpsilon;

    diff = fabs(valorA - valorB);
    valorA = fabs(valorA);
    valorB = fabs(valorB);
    largest = (valorB > valorA) ? valorB : valorA;

    relEpsilon = largest * DBL_EPSILON;
    
    if (diff <= relEpsilon)
        return 1;
    return 0;
}