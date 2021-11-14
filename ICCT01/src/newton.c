#include "newton.h"
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

// Função de tratamento de saida -o
// Objetivo: Verificar se o usuário está fazendo chamada para saida diferente de stdout, e retornando ponteiro para leitura
// Variaveis:
// 			argc: Quantidade de elementos que existem na chamada da execução de arquivo
//			argc: Os elementos que existem na chamada de execução de arquivo
// Retorna o ponteiro de arquivo de saída correto para impressão.
FILE* trataSaida(int argc, char *argv[]){
	char outputName[TAM_BUFFER];
	FILE *output;

	for(int i=0;i<argc;i++){
		if(strcmp(argv[i],"-o") == 0){
			if(i+1<argc){
				strcpy(outputName, argv[i+1]);
			}
		}
	}

	// Caso tiver um nome aponta para a saida, se não saida padrão
	if(strlen(outputName) > 0){
		output = fopen(outputName, "w+");
	}
	else{
		output = stdout;
	}

	return output;
}

// Função para fazer a alocação das variaveis que serão utilizadas nas soluções
// Objetivo: Fazer alocação de memória
// Variaveis:
// 			n: Tamanho do vetor
// Não retorna nada
void alocacaoVariaveis(int n){
	resultados = calloc(n, sizeof(double));
	resultadoJacobiana = calloc(n, sizeof(double));
	derivadas = (void**)malloc(n * n * sizeof(void*));
	jacobiana = calloc(n*n, sizeof(double));
	variaveis = calloc(n, sizeof(char *));

	for (int i = 0; i < n; i++){
		variaveis[i] = calloc(5, sizeof(char));
	}

	return;
}

// Função que realiza a leitura das equações a serem utilizadas
// Objetivo: Escrever dados em equações
// Variaveis:
// 			n: Tamanho do vetor
// Não retorna nada
void leituraEquacoes(int n){
	int length;
	for (int i = 0; i < n; i++)
	{
		fgets(temp, TAM_BUFFER, input);
		length = strlen (temp);
		if (length > 0 && temp[length - 1] == '\n')
			temp[length - 1] = '\0';
		memcpy(equacoes[i], temp, length);
	}
	return;
}

// Função que realiza a leitura de todas as variaveis a serem usadas para a solução que vem do input
// Objetivo: Escrever dados em variaveis prévia a utilização
// Variaveis:
// 			n: Tamanho do vetor
// Não retorna nada
void leituraVariaveis(int n){
	leituraEquacoes(n);
	for (int i = 0; i < n; i++)
	{
		fscanf(input,"%lf", &helper);
		resultados[i] = helper;
	}
	fscanf(input,"%lf\n", &epsilon);
	fscanf(input,"%d\n", &maxIter);
}

// Função que calcula as derivadas parciais
// Objetivo: Cálculo das derivadas parciais a partir das equações lidas no input
// Variaveis:
// 			n: Tamanho do vetor
// Retorna o tempo de execução que levou para fazer o cálculo para essa iteração
double escreveDerivadasParciais (int n){
	double tempoExec = timestamp();
	for (int i = 0; i < n; i++)
	{
		eval = evaluator_create(equacoes[i]);
		assert(eval);
		fprintf(output, "%s = 0\n", evaluator_get_string(eval));
		for (int j = 0; j < n; j++)
		{
			char variavel[5];
			sprintf(variavel, "x%d", j+1);
			if (i == 0)
			{
				memcpy(variaveis[j], variavel, strlen (variavel));
			}
			derivadas[(i*n)+j] = evaluator_derivative(eval, variavel);
			assert(derivadas[(i*n)+j]);
		}
	}
	return timestamp() - tempoExec;
}

// Liberação de memória
// Objetivo: Liberar a memória das variaveis previamente alocadas
// Variaveis:
// 			n: Tamanho do vetor
// Não retorna nada
void liberacaoMemoriaUsada(int n){
	free(derivadas);
	free(jacobiana);
	free(resultados);
	free(resultadoJacobiana);
	for (int i = 0; i < n; i++)
	{
		free(variaveis[i]);
	}
	free(variaveis);
}

// 
// Objetivo: 
// Variaveis:
// 			n: Tamanho do vetor
// 
void jacobiano(int n){
	//Loop epsilon < max resultadoJacobiana, epsilon < max F(x) , iter < maxIter
	//exemplo: https://moodle.c3sl.ufpr.br/pluginfile.php/140644/mod_resource/content/16/Resolu%C3%A7%C3%A3o%20de%20Sistemas%20N%C3%A3o-Lineares.pdf slide 38
	for (iter = maxIter-1; iter < maxIter; iter++) //Adicionar os epsilons na verificação ali
	{
		for (int i = 0; i < n; i++)
		{
			fprintf(output, "x%d = %.6lf\n", i+1, resultados[i]);
		}
		//Criar matriz jacobiana para a resposta atual, necessário computar o tempo
		//Para criar a matriz jacobiana, só aplicar jacobiana[(i*n)+j] = evaluator_evaluate(derivadas[(i*n)+j], resultados[j])
		for (int i = 0; i < n; i++)
		{	
			for (int j = 0; j < n; j++)
			{
				jacobiana[(i*n)+j] = evaluator_evaluate(derivadas[(i*n)+j], n, (char **)variaveis, resultados);
			}

		}		

		//Aplicar Gauss com Pivoteamento Parcial para refinar a resposta, necessário computar o tempo
		//Exemplo: https://www.bragitoff.com/2018/02/gauss-elimination-c-program/

		//Calcular novo resultado (resultados[i] += resultadoJacobiana[i])
		//Encontrar maior F(X) e maior resultadoJacobiana
	}
}