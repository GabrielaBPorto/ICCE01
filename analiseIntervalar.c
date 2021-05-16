#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <stdarg.h>

typedef struct intervalo {
    double inferior;
    double superior;
    double x;
}intervalo;


int max(double args, ...){
    double max, cur;
    va_list valist;
    va_start(valist, args);
    
    max = DBL_MIN_EXP;
    
    for(int i=0; i<args; i++)
    {
        cur = va_arg(valist, int); // Get next elements in the list
        if(max < cur)
            max = cur;
    }
    
    va_end(valist); // Clean memory assigned by valist
    
    return max;
}

int min(double args, ...) {
    double min, cur;
    va_list valist;
    va_start(valist, args);
    
    min = DBL_MAX_EXP;
    
    for(int i=0; i<args; i++)
    {
        cur = va_arg(valist, int);
        if(min > cur)
            min = cur;
    }
    
    va_end(valist);
    
    return min;
}

void imprimeIntervalo(intervalo *vetor, int m){
    for(int i=0;i<m;i++){
        printf("x[%d]:%.8e inf:%.8e sup:%.8e\n",i, vetor[i].x,vetor[i].inferior,vetor[i].superior);
    }
    return;
}

void leituraVariaveis(intervalo *variaveis, int n){
    char lixo[4];
    for(int i=0; i< n; i++){
        scanf("%s %lf\n",lixo, &(variaveis[i].x));
        variaveis[i].inferior = variaveis[i].x - DBL_EPSILON;
        variaveis[i].superior = variaveis[i].x + DBL_EPSILON;
    }
}

void leituraOperacoes(intervalo *variaveis,int n, int m){
    char lixo[6];
    char lixoA, lixoB;
    char operacao;
    int operadorA, operadorB;
    for(int i=m; i<n+m; i++){
        fgets(lixo,6,stdin);
        printf("%s",lixo);
        scanf("%c%d %c %c%d\n",&lixoA,&operadorA,&operacao,&lixoB,&operadorB);
        //printf("%c-%d-%c-%c-%d\n",lixoA, operadorA, operacao, lixoB, operadorB);
        
        switch(operacao)
        {
            case '+':
            case '-':
            case '*':
            case '/':
                continue;
            default:
                printf("Operação inválida\n");
                break;
        }
        break;
    }
}

int main() {
    int numeroVariaveis, numeroOperacoes;

    scanf("%d %d\n",&numeroVariaveis,&numeroOperacoes);

    intervalo variaveis[numeroOperacoes+numeroVariaveis];
    memset(variaveis, 0, sizeof(variaveis));

    leituraVariaveis(variaveis,numeroVariaveis);
    leituraOperacoes(variaveis,numeroOperacoes,numeroVariaveis);
    imprimeIntervalo(variaveis,numeroVariaveis+numeroOperacoes);
    for(int i=0; i<numeroVariaveis; i++){
        printf("Maximum of three numbers: (%lf, %lf, %lf) = %lf\n", variaveis[i].inferior,variaveis[i].x, variaveis[i].superior, max(variaveis[i].inferior, variaveis[i].x, variaveis[i].superior));
        printf("Minimum of three numbers: (%lf, %lf, %lf) = %lf\n", variaveis[i].inferior,variaveis[i].x, variaveis[i].superior,min(variaveis[i].inferior, variaveis[i].x, variaveis[i].superior));
    }


}