#include <stdio.h>
#include "utils.h"
#include "newton.h"
#include <string.h>
#include <matheval.h>
#include <assert.h>

int main(int argc, char *argv[]){

	// Declaração de variavel
	double epsilon, *resultados, *jacobiana, *resultadoJacobiana, helper, maxJacobiana, maxEval;
	int dim, maxIter, length, end, iter;
	char equacoes[TAM_BUFFER][TAM_BUFFER], temp[TAM_BUFFER], **variaveis;
	void *eval, **derivadas;

	output = trataSaida(argc, argv);
	input = fopen("../input/sistemas.dat","r+");

	while (fscanf(input, "%d\n", &dim) != EOF)
	{
		fprintf(output,"%d\n", dim);

		resultados = calloc(dim, sizeof(double));
		resultadoJacobiana = calloc(dim, sizeof(double));
		derivadas = (void**)malloc(dim * dim * sizeof(void*));
		jacobiana = calloc(dim*dim, sizeof(double));
		variaveis = calloc(dim, sizeof(char *));
		for (int i = 0; i < dim; i++)
		{
			variaveis[i] = calloc(5, sizeof(char));
		}

		for (int i = 0; i < dim; i++)
		{
			fgets(temp, TAM_BUFFER, input);
			length = strlen (temp);
			if (length > 0 && temp[length - 1] == '\n')
				temp[length - 1] = '\0';
			memcpy(equacoes[i], temp, length);
		}

		for (int i = 0; i < dim; i++)
		{
			fscanf(input,"%lf", &helper);
			resultados[i] = helper;
		}

		fscanf(input,"%lf\n", &epsilon);
		fscanf(input,"%d\n", &maxIter);

		//Criar vetor das derivadas parciais, necessário computar o tempo
		for (int i = 0; i < dim; i++)
		{
			eval = evaluator_create(equacoes[i]);
			assert(eval);
			fprintf(output, "%s = 0\n", evaluator_get_string(eval));
			for (int j = 0; j < dim; j++)
			{
				char variavel[5];
				sprintf(variavel, "x%d", j+1);
				if (i == 0)
				{
					length = strlen (variavel);
					memcpy(variaveis[j], variavel, length);
				}
				derivadas[(i*dim)+j] = evaluator_derivative(eval, variavel);
				assert(derivadas[(i*dim)+j]);
			}
		}

		fprintf(output,"#\n");

		//Loop epsilon < max resultadoJacobiana, epsilon < max F(x) , iter < maxIter
		//exemplo: https://moodle.c3sl.ufpr.br/pluginfile.php/140644/mod_resource/content/16/Resolu%C3%A7%C3%A3o%20de%20Sistemas%20N%C3%A3o-Lineares.pdf slide 38
		for (iter = maxIter-1; iter < maxIter; iter++) //Adicionar os epsilons na verificação ali
		{
			for (int i = 0; i < dim; i++)
			{
				fprintf(output, "x%d = %.6lf\n", i+1, resultados[i]);
			}
			//Criar matriz jacobiana para a resposta atual, necessário computar o tempo
			//Para criar a matriz jacobiana, só aplicar jacobiana[(i*dim)+j] = evaluator_evaluate(derivadas[(i*dim)+j], resultados[j])
			for (int i = 0; i < dim; i++)
			{	
				for (int j = 0; j < dim; j++)
				{
					jacobiana[(i*dim)+j] = evaluator_evaluate(derivadas[(i*dim)+j], dim, (char **)variaveis, resultados);
				}

			}		

			//Aplicar Gauss com Pivoteamento Parcial para refinar a resposta, necessário computar o tempo
			//Exemplo: https://www.bragitoff.com/2018/02/gauss-elimination-c-program/

			//Calcular novo resultado (resultados[i] += resultadoJacobiana[i])
			//Encontrar maior F(X) e maior resultadoJacobiana
		}
		//Fim Loop

		//Fazer free de tudo que precisa aqui
		free(derivadas);
		free(jacobiana);
		free(resultados);
		free(resultadoJacobiana);
		for (int i = 0; i < dim; i++)
		{
			free(variaveis[i]);
		}
		free(variaveis);

		//Printar os tempos

		if (fscanf(input,"%d\n", &dim) == EOF)
		{
			end = 1;
		}
		fprintf(output, "\n");
	}
	
	
	return 0;
}