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
    va_list listaArgumentos;
    va_start(listaArgumentos, numeroElementos);
    
    max = DBL_MIN_EXP;
    
    for(int i=0; i<numeroElementos; i++)
    {
        cur = va_arg(listaArgumentos, double);
        if(max < cur)
            max = cur;
    }
    
    va_end(listaArgumentos);
    
    return max;
}

// Função variádica para retornar o menor número passado
// int numeroElementos: Número de elementos que a função recebe como parâmetro
// ...: Parâmetros variádicos
double min(int numeroElementos, ...) {
    double min, cur;
    va_list listaArgumentos;
    va_start(listaArgumentos, numeroElementos);
    
    min = DBL_MAX;
    
    for(int i=0; i<numeroElementos; i++)
    {
        cur = va_arg(listaArgumentos, double);
        if(min > cur)
            min = cur;
    }
    
    va_end(listaArgumentos);
    
    return min;
}

// Função auxiliar para imprimir um intervalo
// intervalo *vetor: vetor dos intervalos lidos e calculados
// int tamanho: tamanho do vetor de intervalos
void imprimeIntervalos(intervalo *vetor, int tamanho){
    for(int i=0;i<tamanho;i++){
        printf("x%d - [inf:%.16e sup:%.16e]\n", i+1,vetor[i].inferior,vetor[i].superior);
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
            printf("x%d - [inf:%.16e sup:%.16e]\n", i+1,vetor[i].inferior,vetor[i].superior);
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
// intervalo *vetor: vetor de intervalos
// int indiceResultante: indice para salvar o resultado da operação
// int operandoA: indice do primeiro operando da operação
// int operandoB: indice do segundo operando da operação
void divisao(intervalo *vetor, int indiceResultante, int operandoA, int operandoB){
    vetor[indiceResultante].inferior = min(4,vetor[operandoA-1].inferior * 1/vetor[operandoB-1].inferior,
                                vetor[operandoA-1].inferior * 1/vetor[operandoB-1].superior,
                                vetor[operandoA-1].superior * 1/vetor[operandoB-1].inferior,
                                vetor[operandoA-1].superior * 1/vetor[operandoB-1].superior);
    vetor[indiceResultante].superior = max(4,vetor[operandoA-1].inferior * 1/vetor[operandoB-1].inferior,
                                vetor[operandoA-1].inferior * 1/vetor[operandoB-1].superior,
                                vetor[operandoA-1].superior * 1/vetor[operandoB-1].inferior,
                                vetor[operandoA-1].superior * 1/vetor[operandoB-1].superior);
}

// Função auxiliar para realizar as multiplicações
// intervalo *vetor: vetor de intervalos
// int indiceResultante: indice para salvar o resultado da operação
// int operandoA: indice do primeiro operando da operação
// int operandoB: indice do segundo operando da operação
void multiplicacao(intervalo *vetor, int indiceResultante, int operandoA, int operandoB){
    vetor[indiceResultante].inferior = min(4,vetor[operandoA-1].inferior * vetor[operandoB-1].inferior,
                                vetor[operandoA-1].inferior * vetor[operandoB-1].superior,
                                vetor[operandoA-1].superior * vetor[operandoB-1].inferior,
                                vetor[operandoA-1].superior * vetor[operandoB-1].superior);
    vetor[indiceResultante].superior = max(4,vetor[operandoA-1].inferior * vetor[operandoB-1].inferior,
                                vetor[operandoA-1].inferior * vetor[operandoB-1].superior,
                                vetor[operandoA-1].superior * vetor[operandoB-1].inferior,
                                vetor[operandoA-1].superior * vetor[operandoB-1].superior);
}

// Função auxiliar para realizar a subtração
// intervalo *vetor: vetor de intervalos
// int indiceResultante: indice para salvar o resultado da operação
// int operandoA: indice do primeiro operando da operação
// int operandoB: indice do segundo operando da operação
void subtracao(intervalo *vetor, int indiceResultante, int operandoA, int operandoB){
    vetor[indiceResultante].inferior = min(1, vetor[operandoA-1].inferior + vetor[operandoB-1].superior);
    vetor[indiceResultante].superior = max(1, vetor[operandoA-1].superior + vetor[operandoB-1].inferior);
}

// Função auxiliar para realizar a soma
// intervalo *vetor: vetor de intervalos
// int indiceResultante: indice para salvar o resultado da operação
// int operandoA: indice do primeiro operando da operação
// int operandoB: indice do segundo operando da operação
void soma(intervalo *vetor, int indiceResultante, int operandoA, int operandoB){
    vetor[indiceResultante].inferior = min(1, vetor[operandoA-1].inferior + vetor[operandoB-1].inferior);
    vetor[indiceResultante].superior = max(1, vetor[operandoA-1].superior + vetor[operandoB-1].superior);
}

// Função para realizar o cálculo das operações
// intervalo *variaveis: vetor de intervalos para realizar e salvar operações
// int quantidadeOperacoes: quantidade de operações a serem realizadas
// int quantidadeVariaveis: quantidade de variaveis lidas
int leituraOperacoes(intervalo *variaveis,int quantidadeOperacoes, int quantidadeVariaveis){
    char lixo[6];
    char lixoA, lixoB;
    char operacao;
    int operandoA, operandoB;
    for(int i=quantidadeVariaveis; i<quantidadeOperacoes+quantidadeVariaveis; i++){
        fgets(lixo,6,stdin);
        scanf("%c%d %c %c%d\n",&lixoA,&operandoA,&operacao,&lixoB,&operandoB);

        switch(operacao)
        {
            case '+':
                soma(variaveis,i, operandoA,operandoB);
                break;
            case '-':
                subtracao(variaveis,i, operandoA,operandoB);
                break;
            case '*':
                multiplicacao(variaveis,i,operandoA,operandoB);
                break;
            case '/':
                divisao(variaveis,i,operandoA,operandoB);
                break;
            default:
                printf("Operação inválida\n");
                return -1;
                break;
        }

        if (verificaIntervalos(variaveis[i]))
            return -1;
        variaveis[i].isUnitario = igualdadeRelativa(variaveis[i].inferior, variaveis[i].superior);
    }
    return 0;
}

// Função auxiliar para verificar se um intervalo de operação é ou não válido
// intervalo intervalo: intervalo para verificação de validade
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

// Função auxiliar para conferir relatividade entre intervalos
// double operandoA: um dos operandos para verificação de igualdade
// double operandoB: um dos operandos para verificação de igudalde
int igualdadeRelativa(double operandoA, double operandoB){
    double diferenca, maioroperando, epsilonRelativo;
    diferenca = fabs(operandoA - operandoB);
    operandoA = fabs(operandoA);
    operandoB = fabs(operandoB);

    maioroperando = (operandoB > operandoA) ? operandoB : operandoA;
    epsilonRelativo = maioroperando * DBL_EPSILON;
    
    if (diferenca <= epsilonRelativo)
        return 1;
    return 0;
}