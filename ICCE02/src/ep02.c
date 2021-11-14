#include <stdio.h>
#include "utils.h"
#include "sl.h"

int main(){

	double epsilon;
	int n, k, maxIter;

	leituraVariavel(&n, &k, &epsilon, &maxIter);

	double solution[n][n];
	double variaveis[k];

	double tempoExec = timestamp();

	double *matrizKDiagonal = construcaoMatrizKDiagonal(n, k);

	printarMatrizKDiagonal(matrizKDiagonal, n, k);

	double *independentes = gerarMatrizIndependente(n,k);

	double *resultado = gaussSeidel(matrizKDiagonal, independentes, epsilon, n, k, maxIter);

	printf("Termos Independentes: ");
	for (int i = 0; i<n; i++)
	{
		printf("%.6lf ", independentes[i]);
	}
	printf("\n----\n");
	printf("Solucao: ");
	for (int i = 0; i < n; i++)
	{
		printf("%.6lf ", resultado[i]);
	}

	printf("\ntempo: %.6lf \n", timestamp() - tempoExec);
	return 0;
}