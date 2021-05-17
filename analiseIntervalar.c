#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <math.h>
#include <stdarg.h>

        /* TODO LIST: 
                1. Verificar intervalos inválidos após realizar as operações na função leituraOperacoes
                    1.1. Trocar continues por breaks para poder ir para a verificação
                    1.2. Criar função auxiliar para realizar verificação de acordo com o princípio da responsabilidade
                    1.3. Alterar o retorno da função leituraOperacoes para int, pois caso dê erro na verificação, 
                         devemos retornar -1 e encerrar a execução atual do programa
                    1.4. Realizar verificação do retorno desta função no main com o mesmo objetivo da anterior
                2. Implementar função para verificar se alguma operação retorna um intervalo não unitário
                    2.1. A função deve implementar uma das estratégias de Epsilon e/ou ULP para comparar operadores
                         de ponto flutuante (i.e., comparação de valores)
                    2.2. Caso um intervalo não seja unitário, deve ser printado novamente.
                    2.3. Importante printar uma linha avisando 'Intervalos não unitários' ao entrar na função
                3. Gerar script para testar o código com todos os casos de teste
                    3.1. Pegar todos os .in disponíveis em https://moodle.c3sl.ufpr.br/mod/folder/view.php?id=27968
                    3.2. Realizar script, como os que fizemos para ML, para rodar todos os testes
                    3.3. IMPORTANTE: Os resultados não precisam estar exatos com os dos professores pois diferenças de
                         implementação podem levar a algumas diferenças de valores
                4. Criar arquivos .c e .h para que o arquivo principal contenha apenas o fluxo de execução do código
                    4.1. Atualizar o makefile para fazer uso destes arquivos.
        */

typedef struct intervalo {
    double inferior;
    double superior;
    double x;
}intervalo;

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
        //fgets(lixo,4,stdin);
        scanf("%s %lf\n",lixo, &(variaveis[i].x));
        variaveis[i].inferior = nextafter(variaveis[i].x, -INFINITY);
        variaveis[i].superior = nextafter(variaveis[i].x, INFINITY);
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
        printf("%c-%d-%c-%c-%d\n",lixoA, operadorA, operacao, lixoB, operadorB);
        
        switch(operacao)
        {
            case '+':
                variaveis[i].inferior = min(1, variaveis[operadorA-1].inferior + variaveis[operadorB-1].inferior);
                variaveis[i].superior = max(1, variaveis[operadorA-1].superior + variaveis[operadorB-1].superior);
                continue;
            case '-':
                variaveis[i].inferior = min(1, variaveis[operadorA-1].inferior - variaveis[operadorB-1].inferior);
                variaveis[i].superior = max(1, variaveis[operadorA-1].superior - variaveis[operadorB-1].superior);
                continue;
            case '*':
                variaveis[i].inferior = min(4,variaveis[operadorA-1].inferior * variaveis[operadorB-1].inferior,
                                            variaveis[operadorA-1].inferior * variaveis[operadorB-1].superior,
                                            variaveis[operadorA-1].superior * variaveis[operadorB-1].inferior,
                                            variaveis[operadorA-1].superior * variaveis[operadorB-1].superior);
                variaveis[i].superior = max(4,variaveis[operadorA-1].inferior * variaveis[operadorB-1].inferior,
                                            variaveis[operadorA-1].inferior * variaveis[operadorB-1].superior,
                                            variaveis[operadorA-1].superior * variaveis[operadorB-1].inferior,
                                            variaveis[operadorA-1].superior * variaveis[operadorB-1].superior);
                continue;
            case '/':
                variaveis[i].inferior = min(4,variaveis[operadorA-1].inferior * 1/variaveis[operadorB-1].inferior,
                                            variaveis[operadorA-1].inferior * 1/variaveis[operadorB-1].superior,
                                            variaveis[operadorA-1].superior * 1/variaveis[operadorB-1].inferior,
                                            variaveis[operadorA-1].superior * 1/variaveis[operadorB-1].superior);
                variaveis[i].superior = max(4,variaveis[operadorA-1].inferior * 1/variaveis[operadorB-1].inferior,
                                            variaveis[operadorA-1].inferior * 1/variaveis[operadorB-1].superior,
                                            variaveis[operadorA-1].superior * 1/variaveis[operadorB-1].inferior,
                                            variaveis[operadorA-1].superior * 1/variaveis[operadorB-1].superior);
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

    printf("%d-%d\n", numeroVariaveis,numeroOperacoes);
}
