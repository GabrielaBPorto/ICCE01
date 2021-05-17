#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <math.h>
#include <stdarg.h>
#include "intervalo.h"

double max(int numeroElementos, ...){
    double max, cur;
    va_list valist;
    va_start(valist, numeroElementos);
    
    max = DBL_MIN_EXP;
    
    for(int i=0; i<numeroElementos; i++)
    {
        cur = va_arg(valist, double); // Get next elements in the list
        printf("max atual: %.8e - teste: %.8e\n", max, cur);
        if(max < cur)
            max = cur;
    }
    
    va_end(valist); // Clean memory assigned by valist
    
    return max;
}

double min(int numeroElementos, ...) {
    double min, cur;
    va_list valist;
    va_start(valist, numeroElementos);
    
    min = DBL_MAX;
    
    for(int i=0; i<numeroElementos; i++)
    {
        cur = va_arg(valist, double);
        printf("min atual: %.8e - teste: %.8e\n", min, cur);
        if(min > cur)
            min = cur;
    }
    
    va_end(valist);
    
    return min;
}

void imprimeIntervalo(intervalo *vetor, int m){
    for(int i=0;i<m;i++){
        printf("x:%.20lf inf:%.20lf sup:%.20lf\n", vetor[i].x,vetor[i].inferior,vetor[i].superior);
    }
    return;
}

void leituraVariaveis(intervalo *variaveis, int n){
    char lixo[4];
    for(int i=0; i< n; i++){
        scanf("%s %lf\n",lixo, &(variaveis[i].x));
        variaveis[i].inferior = nextafter(variaveis[i].x, -INFINITY);
        variaveis[i].superior = nextafter(variaveis[i].x, INFINITY);
    }
}

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

void subtracao(intervalo *vetor, int i, int x, int y){
    vetor[i].inferior = min(1, vetor[x-1].inferior - vetor[y-1].inferior);
    vetor[i].superior = max(1, vetor[x-1].superior - vetor[y-1].superior);
}

void soma(intervalo *vetor, int i, int x, int y){
    vetor[i].inferior = min(1, vetor[x-1].inferior + vetor[y-1].inferior);
    vetor[i].superior = max(1, vetor[x-1].superior + vetor[y-1].superior);
}

int leituraOperacoes(intervalo *variaveis,int n, int m){
    char lixo[6];
    char lixoA, lixoB;
    char operacao;
    int operadorA, operadorB;
    for(int i=m; i<n+m; i++){
        fgets(lixo,6,stdin);
        printf("%s",lixo);
        scanf("%c%d %c %c%d\n",&lixoA,&operadorA,&operacao,&lixoB,&operadorB);
        printf("%c-%d-%c-%c-%d\n",lixoA, operadorA, operacao, lixoB, operadorB);
        
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
                return 0;
                break;
        }
    }
    return 1;
}
