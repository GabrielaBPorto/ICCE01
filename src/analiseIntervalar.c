#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <math.h>
#include <stdarg.h>
#include "intervalo.h"

int main() {
    int numeroVariaveis, numeroOperacoes;

    scanf("%d %d\n",&numeroVariaveis,&numeroOperacoes);

    intervalo variaveis[numeroOperacoes+numeroVariaveis];
    memset(variaveis, 0, sizeof(variaveis));

    leituraVariaveis(variaveis,numeroVariaveis);
    if(leituraOperacoes(variaveis,numeroOperacoes,numeroVariaveis)){
        printf("ERROOO");
        return -1;
    }

    imprimeIntervalos(variaveis,numeroVariaveis+numeroOperacoes);
    imprimeNaoUnitarios(variaveis, numeroVariaveis, numeroOperacoes);

    printf("%d-%d\n", numeroVariaveis,numeroOperacoes);
    return 0;
}
