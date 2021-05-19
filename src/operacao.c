#include "intervalo.h"
#include <math.h>
#include <float.h>
#include <stdarg.h>

//Funções que vê max de vários elementos baseado em 
// https://codeforwin.org/2016/02/c-program-to-find-maximum-and-minimum-using-functions.html
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

//Funções que vê min de vários elementos baseado em 
// https://codeforwin.org/2016/02/c-program-to-find-maximum-and-minimum-using-functions.html
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

//operacao de divisao
void divide(dadosIntervalar_t dados, int i, int variavelA, int variavelB){
	dados.intervalos[i].superior = max(4,dados.intervalos[variavelA-1].inferior * 1/dados.intervalos[variavelB-1].inferior,
                                dados.intervalos[variavelA-1].inferior * 1/dados.intervalos[variavelB-1].superior,
                                dados.intervalos[variavelA-1].superior * 1/dados.intervalos[variavelB-1].inferior,
                                dados.intervalos[variavelA-1].superior * 1/dados.intervalos[variavelB-1].superior);
    dados.intervalos[i].inferior = min(4,dados.intervalos[variavelA-1].inferior * 1/dados.intervalos[variavelB-1].inferior,
                                dados.intervalos[variavelA-1].inferior * 1/dados.intervalos[variavelB-1].superior,
                                dados.intervalos[variavelA-1].superior * 1/dados.intervalos[variavelB-1].inferior,
                                dados.intervalos[variavelA-1].superior * 1/dados.intervalos[variavelB-1].superior);
}

// operacao de multiplicao
void multiplication(dadosIntervalar_t dados, int i, int variavelA, int variavelB){
    dados.intervalos[i].inferior = min(4,dados.intervalos[variavelA-1].inferior * dados.intervalos[variavelB-1].inferior,
                                dados.intervalos[variavelA-1].inferior * dados.intervalos[variavelB-1].superior,
                                dados.intervalos[variavelA-1].superior * dados.intervalos[variavelB-1].inferior,
                                dados.intervalos[variavelA-1].superior * dados.intervalos[variavelB-1].superior);
    dados.intervalos[i].superior = max(4,dados.intervalos[variavelA-1].inferior * dados.intervalos[variavelB-1].inferior,
                                dados.intervalos[variavelA-1].inferior * dados.intervalos[variavelB-1].superior,
                                dados.intervalos[variavelA-1].superior * dados.intervalos[variavelB-1].inferior,
                                dados.intervalos[variavelA-1].superior * dados.intervalos[variavelB-1].superior);
}

// operação de subtracao
void subtract(dadosIntervalar_t dados, int i, int variavelA, int variavelB){
    dados.intervalos[i].inferior = min(1, dados.intervalos[variavelA-1].inferior - dados.intervalos[variavelB-1].superior);
    dados.intervalos[i].superior = max(1, dados.intervalos[variavelA-1].superior - dados.intervalos[variavelB-1].inferior);
}

// operação de soma 
void sum(dadosIntervalar_t dados, int i, int variavelA, int variavelB){
    dados.intervalos[i].inferior = min(1, dados.intervalos[variavelA-1].inferior + dados.intervalos[variavelB-1].inferior);
    dados.intervalos[i].superior = max(1, dados.intervalos[variavelA-1].superior + dados.intervalos[variavelB-1].superior);
}
