#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <math.h>
#include <stdarg.h>
#include "intervalo.h"

// Função variádica para retornar o maior número passado
// int numeroElementos: Número de elementos que a função recebe como parâmetro
// ...: Parâmetros variádicos
double max(int numeroElementos, ...){
    double max, cur;
    va_list valist;
    va_start(valist, numeroElementos);
    
    max = DBL_MIN_EXP;
    
    for(int i=0; i<numeroElementos; i++)
    {
        cur = va_arg(valist, double); // Get next elements in the list
        if(max < cur)
            max = cur;
    }
    
    va_end(valist); // Clean memory assigned by valist
    
    return max;
}

// Função variádica para retornar o menor número passado
// int numeroElementos: Número de elementos que a função recebe como parâmetro
// ...: Parâmetros variádicos
double min(int numeroElementos, ...) {
    double min, cur;
    va_list valist;
    va_start(valist, numeroElementos);
    
    min = DBL_MAX;
    
    for(int i=0; i<numeroElementos; i++)
    {
        cur = va_arg(valist, double);
        if(min > cur)
            min = cur;
    }
    
    va_end(valist);
    
    return min;
}

// Função auxiliar para imprimir um intervalo
// intervalo *vetor: vetor dos intervalos lidos e calculados
// int tamanho: tamanho do vetor de intervalos
void imprimeIntervalos(intervalo *vetor, int tamanho){
    for(int i=0;i<tamanho;i++){
        printf("x%d - [inf:%.20e sup:%.20e]\n", i+1,vetor[i].inferior,vetor[i].superior);
    }

    return;
}

// Função auxiliar para imprimir intervalos de operação não unitários
// intervalo *vetor: vetor dos intervalos lidos e calculados
// int quantidadeVariaveis: quantidade de intervalos lidos
// int quantidadeOperacoes: quantidade de intervalos calculados
void imprimeNaoUnitarios(intervalo *vetor, int quantidadeVariaveis, int quantidadeOperacoes){
    printf("Intervalos não unitários\n");
    for(int i = quantidadeVariaveis; i < quantidadeVariaveis + quantidadeOperacoes; i++){
        if(!vetor[i].isUnitario){
            printf("x%d - [inf:%.20e sup:%.20e]\n", i+1,vetor[i].inferior,vetor[i].superior);
        }
    }
}

// Função auxiliar para ler as variáveis dadas e adquirir o intervalo delas
// intervalo *variaveis: vetor dos intervalos lidos e calculados
// int quantidadeVariaveis: quantidade de intervalos para serem lidos
void leituraVariaveis(intervalo *variaveis, int quantidadeVariaveis){
    char lixo[4];
    for(int i=0; i< quantidadeVariaveis; i++){
        scanf("%s %lf\n",lixo, &(variaveis[i].x));
        variaveis[i].inferior = nextafter(variaveis[i].x, -INFINITY);
        variaveis[i].superior = nextafter(variaveis[i].x, INFINITY);
    }
}

// Função auxiliar para realizar as divisões

void divisao(intervalo *vetor, int i, int x, int y){
    vetor[i].inferior = min(4,vetor[x-1].inferior * 1/vetor[y-1].inferior,
                                vetor[x-1].inferior * 1/vetor[y-1].superior,
                                vetor[x-1].superior * 1/vetor[y-1].inferior,
                                vetor[x-1].superior * 1/vetor[y-1].superior);
    vetor[i].superior = max(4,vetor[x-1].inferior * 1/vetor[y-1].inferior,
                                vetor[x-1].inferior * 1/vetor[y-1].superior,
                                vetor[x-1].superior * 1/vetor[y-1].inferior,
                                vetor[x-1].superior * 1/vetor[y-1].superior);
}

// Função auxiliar para realizar as multiplicações

void multiplicacao(intervalo *vetor, int i, int x, int y){
    vetor[i].inferior = min(4,vetor[x-1].inferior * vetor[y-1].inferior,
                                vetor[x-1].inferior * vetor[y-1].superior,
                                vetor[x-1].superior * vetor[y-1].inferior,
                                vetor[x-1].superior * vetor[y-1].superior);
    vetor[i].superior = max(4,vetor[x-1].inferior * vetor[y-1].inferior,
                                vetor[x-1].inferior * vetor[y-1].superior,
                                vetor[x-1].superior * vetor[y-1].inferior,
                                vetor[x-1].superior * vetor[y-1].superior);
}

// Função auxiliar para realizar a subtração
void subtracao(intervalo *vetor, int i, int x, int y){
    vetor[i].inferior = min(1, vetor[x-1].inferior - vetor[y-1].superior);
    vetor[i].superior = max(1, vetor[x-1].superior - vetor[y-1].inferior);
}

// Função auxiliar para realizar a soma
void soma(intervalo *vetor, int i, int x, int y){
    vetor[i].inferior = min(1, vetor[x-1].inferior + vetor[y-1].inferior);
    vetor[i].superior = max(1, vetor[x-1].superior + vetor[y-1].superior);
}

// Função para realizar o cálculo das operações
int leituraOperacoes(intervalo *variaveis,int quantidadeOperacoes, int quantidadeVariaveis){
    char lixo[6];
    char lixoA, lixoB;
    char operacao;
    int operadorA, operadorB;
    for(int i=quantidadeVariaveis; i<quantidadeOperacoes+quantidadeVariaveis; i++){
        fgets(lixo,6,stdin);
        scanf("%c%d %c %c%d\n",&lixoA,&operadorA,&operacao,&lixoB,&operadorB);

        switch(operacao)
        {
            case '+':
                soma(variaveis,i, operadorA,operadorB);
                break;
            case '-':
                subtracao(variaveis,i, operadorA,operadorB);
                break;
            case '*':
                multiplicacao(variaveis,i,operadorA,operadorB);
                break;
            case '/':
                divisao(variaveis,i,operadorA,operadorB);
                break;
            default:
                printf("Operação inválida\n");
                return -1;
                break;
        }

        if (verificaIntervalos(variaveis[i]))
            return -1;
        verificaIntervaloUnitario(variaveis[i]);

    }
    return 0;
}

// Função auxiliar para verificar se um intervalo de operação é ou não válido
int verificaIntervalos(intervalo intervalo){

    if (isnan(intervalo.superior) || isnan(intervalo.inferior)){
        return -1;
    }

    if (intervalo.inferior > intervalo.superior){
        return -1;
    }

    if (isinf(intervalo.inferior) == -1  && isinf(intervalo.superior) == -1){
        return -1;
    }

    if (isinf(intervalo.inferior) == 1  && isinf(intervalo.superior) == -1){
        return -1;
    }

    if (isinf(intervalo.inferior) == 1  && isinf(intervalo.superior) == 1){
        return -1;
    }
    return 0;
}

// Função para verificar se um intervalo de operação é unitário ou não
void verificaIntervaloUnitario(intervalo intervalo){
    if(!AlmostEqualRelative(intervalo.inferior,intervalo.superior)){
        intervalo.isUnitario = 0;
    }
    else{
        intervalo.isUnitario = 1;
    }
}

// Função auxiliar para conferir relatividade entre intervalos
int AlmostEqualRelative(double A, double B){
    // Calculate the difference.
    double diff, largest, relEpsilon;
    diff = fabs(A - B);
    A = fabs(A);
    B = fabs(B);
    // Find the largest
    largest = (B > A) ? B : A;
    relEpsilon = largest * DBL_EPSILON;
    
    if (diff <= relEpsilon)
        return 1;
    return 0;
}