#include <stdio.h>
#include <matheval.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <limits.h>
#include <float.h>
#include <stdint.h>
#include "utils.h"

typedef union Double_t
{
    int64_t i;
    double f;
    struct
    {   // Bitfields for exploration.
        uint64_t mantissa : 53;
        uint64_t exponent : 11;
        uint64_t sign : 1;
    } parts;
} Double_t;

#define TAM_BUFFER 100
char sistema[TAM_BUFFER][TAM_BUFFER];
double x0;
double epsilon;
int n, k, maxIter;
// Variaveis usadas para Newton
double resultado_anterior_newton, resultado_newton, parada_newton;
// Variaveis usadas para Secante
double resultado_anteanterior_secante, resultado_anterior_secante, resultado_secante, parada_secante;
// Variaveis de erro
double erro_absoluto, erro_relativo;
//Variavéis de controle de iteração
int refino_newton = 0, refino_secante = 0;

//Variaveis para uso de biblioteca de tratamento de funções
void *f, *f_prim;

// Essa função tem como o objetivo de centralizar a leitura das variavéis globais
void leituraVariavel(){
	
	char temp[TAM_BUFFER];
	int length;

	scanf("%d %d\n", &n, &k);
	for(int i=0; i<n; i++){
		fgets(temp, TAM_BUFFER, stdin);
		length = strlen (temp);
		if (length > 0 && temp[length - 1] == '\n')
			temp[length - 1] = '\0';
		memcpy(sistema[i], temp, length);
	}
	scanf("%lg %d", &epsilon, &maxIter);

	return;
}

int ulpsFunction(double A, double B)
{
	//Converte O double para o tipo Double_t
    Double_t double_newton, double_secante;
	double_secante.f = A;
	double_newton.f = B;
	int ulpsDiff;

	//Sinais diferentes não são proximos
    if (double_secante.parts.sign != double_newton.parts.sign)
    {
        // Checa +0==-0
        ulpsDiff = abs(abs(double_secante.i) - abs(double_newton.i));
    }
	else
	{
    	ulpsDiff = abs(double_secante.i - double_newton.i);
	}
    //Pega a diferença
    
    return ulpsDiff;
}

//Função genérica de verificação de valor
double verifica(double result1, double result2){
	return fabs(result1 - result2);
}

//Função de verificação de proximo de zero usando epsilon
int verification_proximidade_zero(double divisor){
	if(fabs(divisor) < epsilon){
		return 1;
	}
	return 0;
}


void eliminacaoGaussSeidel(double *d, double *a, double *c, double *b, double*x, int n){
	// Triangulização: 5(n-1) operações

	for(int i =0; i< n-1; i++){
		double m = a[i] / d[i];
		a[i] = 0.0;
		d[i+1] -= c[i] * m;
		b[i+1] -= b[i] * m;
	}
	
}

// A construção da matriz diagonal se dá para
// A diagonal principal
// As diagonais superiores a partir da banda passada de parametro input
// As diagonais inferior a partir da banda passada de parametro input
// O tamanho da diagonal dentro da matriz
// A quantidade de diagonais a partir do parametro de input
// 
void construcaoMatrizKDiagonal(double *diagonalPrincipal, double **superior, double **inferior, int n, int k) {
	int band = (k-1)/2;
	
	for(int i=0; i< n ; i++){
		diagonalPrincipal[i] = executa_funcao(sistema[band], i);
		for(int x=0; x<band; x++){
			if(band-x+1 > 0)
				superior[x][i] = executa_funcao(sistema[(band-(x+1))], i);
			if(band+x+1 > n)
				inferior[x][i] = executa_funcao(sistema[(band+(x+1))], i);
		}
	}
	
}

// Função para gerar o vetor de coeficientes independentes
double* gerarMatrizIndependente(char *funcao, int n)
{
	double *independentes = calloc(n, sizeof(double));
	void *eval = evaluator_create(funcao);
	assert(eval);

	for (int i = 0; i < n; i++)
	{
		independentes[i] = evaluator_evaluate_x(eval, i);
	}

	return independentes;
}

int main(){

	leituraVariavel();

	int solution[n][n];
	int variaveis[k];

	//Impressão da entrade DEBUG
	printf("O valor de n : %d o valor de k %d \n", n, k);
	for(int i=0; i<n; i++){
		printf("A função %d is %s\n", i, sistema[i]);
	}
	printf("O valor de epsilon : %1.16e o valor de maxit %d \n", epsilon, maxIter);


	// Gerar um sistema k-diagonal
	// Para cada i e j o valor dele é dado pela função jque a gente deu.
	// Organizar como deve ficar
	// Metodo a utilizar para a solução de um SL k-diagonal
	// Gauss-Seidel

	double diagonalPrincipal[n];

	double superior[(k-1)/2][n];
	double inferior[(k-1)/2][n];
	//construcaoMatrizKDiagonal(diagonalPrincipal, superior, inferior, n, k);
	//eliminacaoGaussSeidel(diagonalPrincipal, superior, inferior, n, k, solution, variaveis);
	double *independentes = gerarMatrizIndependente(sistema[n-1],n);

	for (int i = 0; i<n; i++)
	{
		printf("Coeficiente %d : %lg\n", i, independentes[i]);
	}

	// Impressão da solução
	// for(int i=0; i< n; i++){
	// 	for(int j=0; j<n; j++){
	// 		printf("%d \n", solution[i][j]);	
	// 	}
	// }
	// printf("----");
	
	// for(int i=0; i< k; i++){
	// 	printf("%d ", variaveis[i]);
	// }
	// printf("\ntempo: %1.16e \n", timestamp());

	return 0;
}