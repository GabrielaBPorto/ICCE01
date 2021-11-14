#include <stdio.h>
#include "utils.h"
#include "newton.h"
#include <string.h>
#include <matheval.h>
#include <assert.h>

int main(){

	double epsilon, *resultados, *jacobiana, *resultadoJacobiana, helper, maxJacobiana, maxEval;
	int dim, maxIter, length, end, iter;
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
		iter = 0;

		resultados = calloc(dim, sizeof(double));
		resultadoJacobiana = calloc(dim, sizeof(double));
		derivadas = (void**)malloc(dim * dim * sizeof(void*));
		jacobiana = calloc(dim*dim, sizeof(double));

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

		//Criar vetor das derivadas parciais, necessário computar o tempo
		for (int i = 0; i < dim; i++)
		{
			eval = evaluator_create(equacoes[i]);
			assert(eval);
			printf("%s = 0\n", evaluator_get_string(eval));
			for (int j = 0; j < dim; j++)
			{
				char variavel[5];
				sprintf(variavel, "x%d", j+1);
				derivadas[(i*dim)+j] = evaluator_derivative(eval, variavel);
				assert(derivadas[(i*dim)+j]);
			}
		}

		printf("#\n");

		//Loop epsilon < max resultadoJacobiana, epsilon < max F(x) , iter < maxIter
		//exemplo: https://moodle.c3sl.ufpr.br/pluginfile.php/140644/mod_resource/content/16/Resolu%C3%A7%C3%A3o%20de%20Sistemas%20N%C3%A3o-Lineares.pdf slide 38

			//Criar matriz jacobiana para a resposta atual, necessário computar o tempo
			//Para criar a matriz jacobiana, só aplicar jacobiana[(i*dim)+j] = evaluator_evaluate(derivadas[(i*dim)+j], resultados[j])
			

			//Aplicar Gauss com Pivoteamento Parcial para refinar a resposta, necessário computar o tempo
			//Exemplo: https://www.bragitoff.com/2018/02/gauss-elimination-c-program/

			//Calcular novo resultado (resultados[i] += resultadoJacobiana[i])
			//Encontrar maior F(X) e maior resultadoJacobiana
		//Fim Loop

		if (scanf("%d\n", &dim) == EOF)
		{
			end = 1;
		}
		printf("\n");
		//Fazer free de tudo que precisa aqui
		free(derivadas);
		free(jacobiana);
		free(resultados);
		free(resultadoJacobiana);
	}
	
	
	return 0;
}