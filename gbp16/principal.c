#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <math.h>
#include <stdarg.h>
#include "intervalo.h"

int main() {
    dadosIntervalar_t data;
    scanf("%d %d\n",&data.quantidadeVariavel,&data.quantidadeOperacao);

    data.intervalos = (intervalo_t *) malloc((data.quantidadeVariavel + data.quantidadeOperacao) * sizeof(intervalo_t));

    readInput(data);


    printResults(data);

    return 0;
}
