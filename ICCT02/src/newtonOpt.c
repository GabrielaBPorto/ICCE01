#include "newtonOpt.h"
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
#include <likwid.h>

// Função de tratamento de saida -o
// Objetivo: Verificar se o usuário está fazendo / para saida diferente de stdout, e retornando ponteiro para leitura
// Variaveis:
// 			argc: Quantidade de elementos que existem na chamada da execução de arquivo
//			argc: Os elementos que existem na chamada de execução de arquivo
// Retorna o ponteiro de arquivo de saída correto para impressão.
FILE* trataSaidaOpt(int argc, char *argv[]){
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
void alocacaoVariaveisOpt(int n){
	resultados = calloc(n, sizeof(double));
	resultadoJacobiana = calloc(n, sizeof(double));
	derivadas = (void**)malloc(n * n * sizeof(void*));
	jacobiana = calloc(n*n, sizeof(double));
	variaveis = calloc(n, sizeof(char *));
	resultadoEquacoes = calloc(n, sizeof(double));

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
void leituraEquacoesOpt(int n){
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
void leituraVariaveisOpt(int n){
	leituraEquacoesOpt(n);
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
double escreveDerivadasParciaisOpt (int n){
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
void liberacaoMemoriaUsadaOpt(int n){
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

// Metodo de jacobi
// Objetivo: Criar matriz jacobiana para a resposta atual
// Variaveis:
// 			n: Tamanho do vetor
// Retorna o tempo de cálculo utilizando o método de newton
double jacobianaMetodoOpt(int n){
	double tempoExec = timestamp();	

	for (int i = 0; i < n; i++)
	{	
		for (int j = 0; j < n; j++)
		{
			jacobiana[(i*n)+j] = evaluator_evaluate(derivadas[(i*n)+j], n, (char **)variaveis, resultados);
		}
		eval = evaluator_create(equacoes[i]);
		assert(eval);
		resultadoEquacoes[i] = evaluator_evaluate(eval, n, (char **)variaveis, resultados);
	}
	return timestamp() - tempoExec;
}

// Pivoteamento parcial de Gauss
// Objetivo: Aplicar Gauss com Pivoteamento Parcial para refinar a resposta
// Variaveis:
// 			n: Tamanho do vetor
// Retorna o tempo de cálculo utilizando o método de newton
double gaussPivotearParcialOpt(int n){
	double tempoExec = timestamp();

	int i,j,k;
    for(i=0;i<n;i++){
        // Pivoteamento parcial
        for(k=i+1;k<n;k++){
			// Caso o elemento da diagonal for menor que os termos abaixo
            if(fabs(jacobiana[(i*n)+i])<fabs(jacobiana[(k*n)+i])){
                //Troca as linhas
				LIKWID_MARKER_START("TrocaLinhasTrab2");
				trocaLinhasOpt(n,i,k);
				LIKWID_MARKER_STOP("TrocaLinhasTrab2");
            }
        }
		
        //Realiza eliminação de gauss
		LIKWID_MARKER_START("EliminacaoGaussTrab2");
		if(eliminacaoGaussOpt(n,i) == -1){
			return -1;
		}
		LIKWID_MARKER_STOP("EliminacaoGaussTrab2");
    }
	LIKWID_MARKER_START("EscreveParciaisTrab2");
	calculaResultadoMatrizJacobianaOpt(n);
	LIKWID_MARKER_STOP("EscreveParciaisTrab2");
	return timestamp() - tempoExec;
}

// Eliminação de Gauss
// Objetivo: Faz a eliminação de Gauss
// Variaveis:
// 			n: Tamanho do vetor
//			i: Iteração 1
// Não retorna nada
double eliminacaoGaussOpt(int n, int i){
	double term;
	for(int k=i+1;k<n;k++){
		// Caso for 0 vai jogar para mensagem de erro
		if(jacobiana[(i*n)+i] != 0){
			term= jacobiana[(k*n)+i]/ jacobiana[(i*n)+i];
			for(int j=0;j<n;j++){
				jacobiana[(k*n)+j]=jacobiana[(k*n)+j]-term*jacobiana[(i*n)+j];
			}
			resultadoEquacoes[k] = resultadoEquacoes[k] - term * resultadoEquacoes[i];				
		}
		else{
			return -1;
		}
	}
}

// Troca de Linhas
// Objetivo: Trocar as linhas caso necessário
// Variaveis:
// 			n: Tamanho do vetor
//			i: Iteração 1
//			k: Iteração 2
// Não retorna nada
void trocaLinhasOpt(int n, int i, int k){
	for(int j=0;j<n;j++){                
		troca(&jacobiana[(i*n)+j], &jacobiana[(k*n)+j]);
	}
	troca(&resultadoEquacoes[i], &resultadoEquacoes[k]);
}

// Calcula o resultado da matriz Jacobiana
// Objetivo: Calculcar o resultado a partir do pivoteamento de gauss
// Variaveis:
// 			n: Tamanho do vetor
// Não retorna nada
void calculaResultadoMatrizJacobianaOpt(int n){
	int j;
	for (int i = n-1; i >= 0; i--)
	{
		resultadoJacobiana[i] = resultadoEquacoes[i];
		for (j = i +1; j < n; j++)
		{
			resultadoJacobiana[i] = resultadoJacobiana[i] - jacobiana[(i*n)+j] * resultadoJacobiana[j];
		}
		resultadoJacobiana[i] = resultadoJacobiana[i] / jacobiana[(i*n)+i];
	}
}

// Método de Newton
// Objetivo: Devolver uma solução refinada para o Sistema linear passado
// Variaveis:
// 			n: Tamanho do vetor
//			tempos: Vetor que grava tempos por iteração de resposta
// Retorna o tempo de cálculo utilizando o método de newton
double newtonMethodOpt(int n, double *tempos){
	double tempoExec = timestamp();
	int iter;

	//Loop epsilon < max resultadoJacobiana, epsilon < max F(x) , iter < maxIter
	for (iter = 0; iter < maxIter; iter++)
	{
		impressaoResultadosOpt(n);

		LIKWID_MARKER_START("JacobianaTrab2");
		tempos[2] = jacobianaMetodoOpt(n);
		LIKWID_MARKER_STOP("JacobianaTrab2");

		//Verificação de parada
		if (maxOpt(resultadoEquacoes, n) < epsilon){
			return timestamp() - tempoExec;
		}
	
		LIKWID_MARKER_START("PivoteamentoTrab2");
		tempos[3] = gaussPivotearParcialOpt(n);
		LIKWID_MARKER_STOP("PivoteamentoTrab2");
		if(tempos[3] == -1){
			return -1;
		}

		calculoResultadoXOpt(n);

		// Verificação de parada
		if (maxOpt(resultadoJacobiana, n) < epsilon){
			return timestamp() - tempoExec;
		}
				
	}
	return timestamp() - tempoExec;
}

// Calcula resultado
// Objetivo: Função que calcula resultado à partir dos resultados do vetor de jacobiano
// Variaveis:
// 			n: Tamanho do vetor
// Não retorna nada
void calculoResultadoXOpt(int n){
	LIKWID_MARKER_START("ResultadoJacobianaTrab2");
	for(int i=0; i< n; i++){
		resultados[i] -= resultadoJacobiana[i];
	}
	LIKWID_MARKER_STOP("ResultadoJacobianaTrab2");
}


// Impressão do vetor de resultados
// Objetivo: Impressão padronizada para os resultados de cada iteração
// Variaveis:
// 			n: Tamanho do vetor
// Não retorna nada
void impressaoResultadosOpt(int  n){
	fprintf(output,"#\n");
	for (int i = 0; i < n; i++)
	{
		fprintf(output, "x%d = %.6lf\n", i+1, resultados[i]);
	}
	return;
}

// Max
// Objetivo: Encontrar o maior item em um vetor
// Variaveis:
// 			vetor: Em qual vetor que vai ser feito a busca
// 			n: Tamanho do vetor.
// Obs: Como estamos trabalhando com matrizes como vetores, podemos só passar o tamanho da matriz
// Não retorna nada
double maxOpt(double *vetor, int n){

	LIKWID_MARKER_START("MaxTrab2");
	double max = fabs(vetor[0]);
	for(int i=0; i<n;i++){
		if(fabs(max) < fabs(vetor[i])){
			max = fabs(vetor[i]);
		}
	}
	LIKWID_MARKER_STOP("MaxTrab2");
	return max;
}

int trabalho2(FILE *input,FILE *output){

	LIKWID_MARKER_INIT;
  	LIKWID_MARKER_START("StartNewtonTrab2");

	// Declaração de variavel
	int dim;
	void *eval;
	double tempos[4];

	printf("Tratou saida\n");

	while (fscanf(input, "%d\n", &dim) != EOF)
	{
		fprintf(output,"%d\n", dim);
		LIKWID_MARKER_START("AlocVariavelTrab2");
		alocacaoVariaveis(dim);
		LIKWID_MARKER_STOP("AlocVariavelTrab2");

		LIKWID_MARKER_START("LeVariavelTrab2");
		leituraVariaveis(dim);
		LIKWID_MARKER_STOP("LeVariavelTrab2");

		LIKWID_MARKER_START("EscreveParciaisTrab2");
		tempos[1] = escreveDerivadasParciais(dim);
		LIKWID_MARKER_STOP("EscreveParciaisTrab2");

		LIKWID_MARKER_START("MetodoNewtonTrab2");
		tempos[0] = newtonMethod(dim, tempos);
		if(tempos[0] == -1){
			return -1;
		}
		LIKWID_MARKER_STOP("MetodoNewtonTrab2");

		LIKWID_MARKER_START("LiberaMemoriaTrab2");
		liberacaoMemoriaUsada(dim);
		LIKWID_MARKER_STOP("LiberaMemoriaTrab2");


		//Printar os tempos
		fprintf(output,"###########\n");
		fprintf(output,"# Tempo Total : %.6lf \n", tempos[0]);
		fprintf(output,"# Tempo Derivadas: %.6lf \n", tempos[1]);
		fprintf(output,"# Tempo Jacobiana: %.6lf \n", tempos[2]);
		fprintf(output,"# Tempo SL: %.6lf \n", tempos[3]);
		fprintf(output,"###########\n\n");
	}
	LIKWID_MARKER_STOP("StartNewtonTrab2");
	LIKWID_MARKER_CLOSE;

	return 0;
}