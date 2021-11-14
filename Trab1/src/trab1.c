#include <stdio.h>
#include "utils.h"
#include "newton.h"
#include <string.h>
#include <matheval.h>
#include <assert.h>

int main(){

	double epsilon, *resultados, **jacobiana, helper;
	int dim, maxIter, length, end;
	char equacoes[TAM_BUFFER][TAM_BUFFER], temp[TAM_BUFFER];
	void *eval, **derivadas;

	if (scanf("%d\n", &dim) == EOF)
	{
		printf("Erro: Arquivo vazio.\n");
		return -1;
	}

	while (end != 1)
	{
		printf("%d\n", dim);

		resultados = calloc(dim, sizeof(double));
		derivadas =  (void**)malloc(dim * dim * sizeof(void*));

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

		//Criar vetor das derivadas parciais
		for (int i = 0; i < dim; i++)
		{
			eval = evaluator_create(equacoes[i]);
			assert(eval);
			printf("%s = 0\n", evaluator_get_string(eval));
			for (int j = 0; j < dim; j++)
			{
				char variavel[5];
				sprintf(variavel, "x%d", j);
				derivadas[(i*dim)+j] = evaluator_derivative(eval, variavel);
				assert(derivadas[(i*dim)+j]);
			}
		}

		printf("#\n");

		//Loop epsilon + maxIter
		//Criar matriz jacobiana para a resposta atual

		//Aplicar Gauss com Pivoteamento Parcial para refinar a resposta
		//Fim Loop

		if (scanf("%d\n", &dim) == EOF)
		{
			end = 1;
		}
		printf("\n");
	}
	
	
	return 0;
}