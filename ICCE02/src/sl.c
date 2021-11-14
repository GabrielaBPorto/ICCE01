#include "sl.h"
#include <matheval.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <limits.h>
#include <float.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

char sistema[TAM_BUFFER][TAM_BUFFER];

// Essa função tem como o objetivo de centralizar a leitura das variavéis globais
void leituraVariavel(int *n, int *k, double *epsilon, int *maxIter){
	
	char temp[TAM_BUFFER];
	int length;

	scanf("%d %d\n", n, k);
	for(int i=0; i <= *k; i++){
		fgets(temp, TAM_BUFFER, stdin);
		length = strlen (temp);
		if (length > 0 && temp[length - 1] == '\n')
			temp[length - 1] = '\0';
		memcpy(sistema[i], temp, length);
	}
	scanf("%lg %d", epsilon, maxIter);
	return;
}

// Função para printar as diagonais
void printarMatrizKDiagonal(double *matriz, int n, int k)
{
	int band = (k-1)/2;
	for (int i = 0; i < n; i++)
	{
		printf("Valores das diagonais na linha %d: ", i);
		for (int j = 0; j < n; j++)
		{
			if (i > (j+band) || i < (j-band))
			{
				continue;
			}
			else
			{
				printf("%.6lf ", matriz[(i*n)+j]);
			}			
		}
		printf("\n");
	}
}

// Função que implementa Gauss Seidel
double * gaussSeidel(double *matriz, double*coeficientes, double eps, int n, int k, int maxIter){

	// Instanciar 
	double *resultado = calloc(n, sizeof(double));
	double anterior, temp, error = 1;
	int iter = 0, band = (k-1)/2;	

	//Loop de refinação/convergência
	while ((error > eps) && iter < maxIter)
	{
		//Loop para percorrer linhas da matriz
		for (int i = 0; i < n; i++)
		{
			anterior = resultado[i];
			resultado[i] = coeficientes[i];

			// Loop para percorrer colunas da matriz
			for (int j = 0; j < n; j++)
			{	
				if (i == j || i > (j+band) || i < (j-band))
				{
					continue;
				}
				else
				{
					resultado[i] = resultado[i] - matriz[(i*n)+j]*resultado[j];
				}
			}
			resultado[i] = resultado[i] / matriz[(i*n)+i];
			temp = fabs(resultado[i] - anterior);
			if (temp > error)
			{
				error = temp;
			}
		}
		
		iter++;
	} 
	
	return resultado;
}

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
				continue;
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
double* gerarMatrizIndependente(int n, int k)
{
	double *independentes = calloc(n, sizeof(double));
	void *eval = evaluator_create(sistema[k]);
	assert(eval);

	for (int i = 0; i < n; i++)
	{
		independentes[i] = evaluator_evaluate_x(eval, i);
	}

	return independentes;
}