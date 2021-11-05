#include <stdio.h>
#include "utils.h"
#include "sl.h"

int main(){

	double epsilon;
	int n, k, maxIter;

	leituraVariavel(&n, &k, &epsilon, &maxIter);

	double solution[n][n];
	double variaveis[k];

	//Impressão da entrade DEBUG
	// printf("O valor de n : %d o valor de k %d \n", n, k);
	// for(int i=0; i<n; i++){
	// 	printf("A função %d is %s\n", i, sistema[i]);
	// }
	// printf("O valor de epsilon : %1.16e o valor de maxit %d \n", epsilon, maxIter);

	double tempoExec = timestamp();

	double *matrizKDiagonal = construcaoMatrizKDiagonal(n, k);

	printarMatrizKDiagonal(matrizKDiagonal, n, k);

	double *independentes = gerarMatrizIndependente(n,k);

	double *resultado = gaussSeidel(matrizKDiagonal, independentes, epsilon, n, k, maxIter);

	printf("termos independentes: ");
	for (int i = 0; i<n; i++)
	{
		printf("[%.6lf] ", independentes[i]);
	}
	printf("\n----\n");
	printf("solucao: ");
	for (int i = 0; i < n; i++)
	{
		printf("%.6lf ", resultado[i]);
	}

	printf("\ntempo: %.6lf \n", timestamp() - tempoExec);
	return 0;
}