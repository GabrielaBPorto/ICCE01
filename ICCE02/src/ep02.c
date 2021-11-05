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

// Essa função tem como o objetivo de centralizar a leitura das variavéis globais
void leituraVariavel(int *n, int *k, double *epsilon, int *maxIter){
	
	char temp[TAM_BUFFER];
	int length;

	scanf("%d %d\n", n, k);
	for(int i=0; i<*n; i++){
		fgets(temp, TAM_BUFFER, stdin);
		length = strlen (temp);
		if (length > 0 && temp[length - 1] == '\n')
			temp[length - 1] = '\0';
		memcpy(sistema[i], temp, length);
	}
	scanf("%lg %d", epsilon, maxIter);

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


// void eliminacaoGaussSeidel(double *b, double*x, int n, double tol){

// 	// double resultado_anterior[]
// 	// double erro = 1.0 + tol;
// 	// while ( erro < tol ){
// 	// 	x[0] = (b[0] - c[0] * x[1]) / d[0];
// 	// 	for(int i=1;i<n-1;++i){
// 	// 		x[i] = (b[i] -a[i-1] * x[i-q] - c[i] * x[i+1])/d[i];
// 	// 	}
// 	// 	x[n-1] = (b[n-1] - a[n-2] * x[n-2]) / d[n-1];
// 	// 	//Calcula erro
// 	// 	double erroAcumulativo = 0.0;
// 	// 	for(int i=0;i<n;i++){
// 	// 		erroAcumulativo = x[i]
// 	// 	}
// 	// 	// Pegar vetor de resultado e diminuir do vetor de resultado passado
// 	// }
	
// }

// A construção da matriz diagonal se dá para
// A diagonal principal
// As diagonais superiores a partir da banda passada de parametro input
// As diagonais inferior a partir da banda passada de parametro input
// O tamanho da diagonal dentro da matriz
// A quantidade de diagonais a partir do parametro de input
// 
double *construcaoMatrizKDiagonal(int n, int k) {
	
	int band = (k-1)/2;
	int *contadorDiagonal = calloc(k, sizeof(int));
	double *matrizKDiagonal = calloc(n*n, sizeof(double));
	
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			if (i > (j+band) || i < (j-band))
			{
				matrizKDiagonal[(i*n)+j] = 0;
			}
			else
			{
				int diag = band+i-j;
				void *eval = evaluator_create(sistema[diag]);				
				matrizKDiagonal[(i*n)+j] = evaluator_evaluate_x(eval, contadorDiagonal[diag]);
				contadorDiagonal[diag]+=1;
			}
		}
	}

	return matrizKDiagonal;
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

	double epsilon;
	int n, k, maxIter;

	leituraVariavel(&n, &k, &epsilon, &maxIter);

	double solution[n][n];
	double variaveis[k];

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

	double *matrizKDiagonal = construcaoMatrizKDiagonal(n, k);

	//eliminacaoGaussSeidel(solution, variaveis, n, k);
	double *independentes = gerarMatrizIndependente(sistema[n-1],n);

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			printf("[%lg] ", matrizKDiagonal[i*n+j]);
		}
		printf("\n");
	}


	// for (int i = 0; i<n; i++)
	// {
	// 	printf("Coeficiente %d : %lg\n", i, independentes[i]);
	// }

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