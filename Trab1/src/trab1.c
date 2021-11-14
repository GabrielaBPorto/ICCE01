#include <stdio.h>
#include "utils.h"
#include "newton.h"
#include <string.h>

int main(){

	double epsilon, *resultados, **jacobiana, helper;
	int dim, maxIter, length, end;
	char equacoes[TAM_BUFFER][TAM_BUFFER], temp[TAM_BUFFER];

	if (scanf("%d\n", &dim) == EOF)
	{
		printf("Arre egua\n");
		end = 1;
	}

	while (end != 1)
	{
		resultados = calloc(dim, sizeof(double));

		for (int i = 0; i < dim; i++)
		{
			fgets(temp, TAM_BUFFER, stdin);
			length = strlen (temp);
			if (length > 0 && temp[length - 1] == '\n')
				temp[length - 1] = '\0';
			memcpy(equacoes[i], temp, length);
		}

		for (int i = 0; i < dim; i++)
		{
			scanf("%lf", &helper);
			resultados[i] = helper;
		}

		scanf("%lf\n", &epsilon);
		scanf("%d\n", &maxIter);
		scanf("\n");

		if (scanf("%d\n", &dim) == EOF)
		{
			end = 1;
		}
		printf("Dimensao: %d\n", dim);
	}
	
	
	return 0;
}