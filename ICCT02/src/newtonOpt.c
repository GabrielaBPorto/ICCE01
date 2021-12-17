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
// n: Tamanho do vetor
// Não retorna nada
void alocacaoVariaveisOpt(int n){	
	resultadosOpt = calloc(n, sizeof(double));
	resultadoJacobianaOpt = calloc(n, sizeof(double));
	equacoes = (void **)malloc(n * sizeof(void*));
	derivadas = (void**)malloc(n * n * sizeof(void*));
	jacobianaOpt = calloc(n*n, sizeof(double));
	variaveis = calloc(n, sizeof(char *));
	resultadoEquacoes = calloc(n, sizeof(double));

	for (int i = 0; i < n; i++){
		variaveis[i] = calloc(8, sizeof(char));
	}

	return;
}

// Função que realiza a leitura das equações a serem utilizadas
// Objetivo: Escrever dados em equações
// Variaveis:
// n: Tamanho do vetor
// Não retorna nada
void leituraEquacoesOpt(int n, FILE *input){
	int length;
	char temp[TAM_BUFFER];

	for (int i = 0; i < n; i++)
	{
		fgets(temp, TAM_BUFFER, input);
		length = strlen (temp);
		if (length > 0 && temp[length - 1] == '\n')
			temp[length - 1] = '\0';
		equacoes[i] = evaluator_create(temp);
		assert(equacoes[i]);
	}
	return;
}

// Função que realiza a leitura de todas as variaveis a serem usadas para a solução que vem do input
// Objetivo: Escrever dados em variaveis prévia a utilização
// Variaveis:
// 			n: Tamanho do vetor
// Não retorna nada
void leituraVariaveisOpt(int n, FILE *input, double *epsilon, int *maxIter){
	// char helper[TAM_BUFFER];
	// char *token, *ptr;
	double helper;
	leituraEquacoesOpt(n, input);
	
	// fgets(helper, TAM_BUFFER, input);
	// token = strtok(helper, " ");

	for (int i = 0; i < n; i++)
	{
		fscanf(input,"%lf", &helper);
		resultadosOpt[i] = helper;
	}
	fscanf(input,"%lf\n", epsilon);
	fscanf(input,"%d\n", maxIter);
}

// Função que calcula as derivadas parciais
// Objetivo: Cálculo das derivadas parciais a partir das equações lidas no input
// Variaveis:
// 			n: Tamanho do vetor
// Retorna o tempo de execução que levou para fazer o cálculo para essa iteração
double escreveDerivadasParciaisOpt (int n, FILE *output){
	void *eval;
	double tempoExec = timestamp();
	for (int i = 0; i < n; i++)
	{
		fprintf(output, "%s = 0\n", evaluator_get_string(equacoes[i]));
		for (int j = 0; j < n; j++)
		{
			char variavel[8];
			sprintf(variavel, "x%d", j+1);
			if (i == 0)
			{
				memcpy(variaveis[j], variavel, strlen (variavel));
			}
			derivadas[(i*n)+j] = evaluator_derivative(equacoes[i], variavel);
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
	free(jacobianaOpt);
	free(resultadosOpt);
	free(resultadoJacobianaOpt);
	for (int i = 0; i < n; i++)
	{
		free(variaveis[i]);
	}
	free(variaveis);
	free(equacoes);
}

// Metodo de jacobi
// Objetivo: Criar matriz jacobianaOpt para a resposta atual
// Variaveis:
// 			n: Tamanho do vetor
// Retorna o tempo de cálculo utilizando o método de newton
double jacobianaMetodoOpt(int n, double *jacobianaOpt, void **derivadas, char **variaveis, 
						  double *resultadosOpt, double *resultadoEquacoes){
	double tempoExec = timestamp();	
	void *eval;

	for (int i = 0; i < n; i++)
	{	
		for (int j = 0; j < n; j++)
		{
			jacobianaOpt[(i*n)+j] = evaluator_evaluate(derivadas[(i*n)+j], n, (char **)variaveis, resultadosOpt);
		}
		resultadoEquacoes[i] = evaluator_evaluate(equacoes[i], n, (char **)variaveis, resultadosOpt);
	}
	return timestamp() - tempoExec;
}

// Pivoteamento parcial de Gauss
// Objetivo: Aplicar Gauss com Pivoteamento Parcial para refinar a resposta
// Variaveis:
// 			n: Tamanho do vetor
// Retorna o tempo de cálculo utilizando o método de newton
// double gaussPivotearParcialOpt(int n, double *jacobianaOpt, double *resultadoJacobianaOpt, double *resultadoEquacoes){
// 	double tempoExec = timestamp();

// 	int i,j,k;
//     for(i=0;i<n;i++){
//         // Pivoteamento parcial
//         for(k=i+1;k<n;k++){
// 			// Caso o elemento da diagonal for menor que os termos abaixo
//             if(fabs(jacobianaOpt[(i*n)+i])<fabs(jacobianaOpt[(k*n)+i])){
//                 //Troca as linhas
// 				LIKWID_MARKER_START("TrocaLinhasTrab2");
// 				trocaLinhasOpt(n,i,k, jacobianaOpt, resultadoEquacoes);
// 				LIKWID_MARKER_STOP("TrocaLinhasTrab2");
//             }
//         }
		
//         //Realiza eliminação de gauss
// 		LIKWID_MARKER_START("EliminacaoGaussTrab2");
// 		if(eliminacaoGaussOpt(n,i, jacobianaOpt, resultadoEquacoes) == -1){
// 			return -1;
// 		}
// 		LIKWID_MARKER_STOP("EliminacaoGaussTrab2");
//     }
// 	LIKWID_MARKER_START("EscreveParciaisTrab2");
// 	calculaResultadoMatrizJacobianaOpt(n, resultadoJacobianaOpt, resultadoEquacoes, jacobianaOpt);
// 	LIKWID_MARKER_STOP("EscreveParciaisTrab2");
// 	return timestamp() - tempoExec;
// }

// Eliminação de Gauss
// Objetivo: Faz a eliminação de Gauss
// Variaveis:
// 			n: Tamanho do vetor
//			i: Iteração 1
// Não retorna nada
double eliminacaoGaussOpt(int n, int i, double *jacobianaOpt, double *resultadoEquacoes){
	double term;
	for(int k=i+1;k<n;k++){
		// Caso for 0 vai jogar para mensagem de erro
		if(jacobianaOpt[(i*n)+i] != 0){
			term= jacobianaOpt[(k*n)+i]/ jacobianaOpt[(i*n)+i];
			for(int j=0;j<n;j++){
				jacobianaOpt[(k*n)+j]=jacobianaOpt[(k*n)+j]-term*jacobianaOpt[(i*n)+j];
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
	for(int j=0;j<n-n%4;j+=4){                
		troca(&jacobianaOpt[(i*n)+j], &jacobianaOpt[(k*n)+j]);
		troca(&jacobianaOpt[(i*n)+j+1], &jacobianaOpt[(k*n)+j+1]);
		troca(&jacobianaOpt[(i*n)+j+2], &jacobianaOpt[(k*n)+j+2]);
		troca(&jacobianaOpt[(i*n)+j+3], &jacobianaOpt[(k*n)+j+3]);
	}
	for (int j=n-n%4; j < n; j++)
	{
		troca(&jacobianaOpt[(i*n)+j], &jacobianaOpt[(k*n)+j]);;
	}
	troca(&resultadoEquacoes[i], &resultadoEquacoes[k]);
}

// Calcula o resultado da matriz jacobianaOpt
// Objetivo: Calculcar o resultado a partir do pivoteamento de gauss
// Variaveis:
// 			n: Tamanho do vetor
// Não retorna nada
void calculaResultadoMatrizJacobianaOpt(int n){
	int j;
	for (int i = n-1; i >= n%4; i-=4)
	{
		resultadoJacobianaOpt[i] = resultadoEquacoes[i];
		resultadoJacobianaOpt[i-1] = resultadoEquacoes[i-1];
		resultadoJacobianaOpt[i-2] = resultadoEquacoes[i-2];
		resultadoJacobianaOpt[i-3] = resultadoEquacoes[i-3];
		for (j = i +1; j < n; j++)
		{
			resultadoJacobianaOpt[i] = resultadoJacobianaOpt[i] - jacobianaOpt[(i*n)+j] * resultadoJacobianaOpt[j];
			resultadoJacobianaOpt[i-1] = resultadoJacobianaOpt[i-1] - jacobianaOpt[((i-1)*n)+j] * resultadoJacobianaOpt[j];
			resultadoJacobianaOpt[i-2] = resultadoJacobianaOpt[i-1] - jacobianaOpt[((i-2)*n)+j] * resultadoJacobianaOpt[j];
			resultadoJacobianaOpt[i-3] = resultadoJacobianaOpt[i-1] - jacobianaOpt[((i-3)*n)+j] * resultadoJacobianaOpt[j];
		}
		resultadoJacobianaOpt[i] = resultadoJacobianaOpt[i] / jacobianaOpt[(i*n)+i];
		resultadoJacobianaOpt[i-1] = resultadoJacobianaOpt[i-1] / jacobianaOpt[((i-1)*n)+(i-1)];
		resultadoJacobianaOpt[i-2] = resultadoJacobianaOpt[i-2] / jacobianaOpt[((i-2)*n)+(i-2)];
		resultadoJacobianaOpt[i-3] = resultadoJacobianaOpt[i-3] / jacobianaOpt[((i-3)*n)+(i-3)];
	}
	for (int i = (n%4)-1; i >= 0; i --)
	{
		resultadoJacobianaOpt[i] = resultadoEquacoes[i];
		for (j = i +1; j < n; j++)
		{
			resultadoJacobianaOpt[i] = resultadoJacobianaOpt[i] - jacobianaOpt[(i*n)+j] * resultadoJacobianaOpt[j];
		}
		resultadoJacobianaOpt[i] = resultadoJacobianaOpt[i] / jacobianaOpt[(i*n)+i];
	}
}

// Método de Newton
// Objetivo: Devolver uma solução refinada para o Sistema linear passado
// Variaveis:
// 			n: Tamanho do vetor
//			tempos: Vetor que grava tempos por iteração de resposta
// Retorna o tempo de cálculo utilizando o método de newton
// double newtonMethodOpt(int n, double *tempos){
// 	double tempoExec = timestamp();
// 	int iter;

// 	//Loop epsilon < max resultadoJacobianaOpt, epsilon < max F(x) , iter < maxIter
// 	for (iter = 0; iter < maxIter; iter++)
// 	{
// 		impressaoResultadosOpt(n, output);

// 		LIKWID_MARKER_START("MatrizJacobianaTrab2");
// 		tempos[2] = jacobianaMetodoOpt(n);
// 		LIKWID_MARKER_STOP("MatrizJacobianaTrab2");

// 		//Verificação de parada
// 		if (maxOpt(resultadoEquacoes, n) < epsilon){
// 			return timestamp() - tempoExec;
// 		}
	
// 		LIKWID_MARKER_START("PivoteamentoTrab2");	fprintf(stderr, "\n");

// 		tempos[3] = gaussPivotearParcialOpt(n);
// 		LIKWID_MARKER_STOP("PivoteamentoTrab2");
// 		if(tempos[3] == -1){
// 			return -1;
// 		}

// 		calculoResultadoXOpt(n);

// 		// Verificação de parada
// 		if (maxOpt(resultadoJacobianaOpt, n) < epsilon){
// 			return timestamp() - tempoExec;
// 		}
				
// 	}
// 	return timestamp() - tempoExec;
// }

// Calcula resultado
// Objetivo: Função que calcula resultado à partir dos resultadosOpt do vetor de jacobiano
// Variaveis:
// 			n: Tamanho do vetor
// Não retorna nada
void calculoResultadoXOpt(int n){
	for(int i=0; i< n-n%4; i+=4){
		resultadosOpt[i] -= resultadoJacobianaOpt[i];
		resultadosOpt[i+1] -= resultadoJacobianaOpt[i+1];
		resultadosOpt[i+2] -= resultadoJacobianaOpt[i+2];
		resultadosOpt[i+3] -= resultadoJacobianaOpt[i+3];
	}
	for (int i = n-n%4; i < n; i++)
	{
		resultadosOpt[i] -= resultadoJacobianaOpt[i];
	}
}


// Impressão do vetor de resultadosOpt
// Objetivo: Impressão padronizada para os resultadosOpt de cada iteração
// Variaveis:
// 			n: Tamanho do vetor
// Não retorna nada
void impressaoResultadosOpt(int  n, FILE *output, double *resultadosOpt){
	fprintf(output,"#\n");
	for (int i = 0; i < n; i++)
	{
		fprintf(output, "x%d = %.6lf\n", i+1, resultadosOpt[i]);
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

int trabalho2(FILE *input, FILE *output){

	double maxEval, epsilon;
	
	int maxIter;

	LIKWID_MARKER_INIT;
  	LIKWID_MARKER_START("StartNewtonTrab2");

	// Declaração de variavel
	int dim, i, j, k, iter, term;
	void *eval;
	double tempos[4], tempoExec;

	while (fscanf(input, "%d\n", &dim) != EOF)
	{
		fprintf(output,"%d\n", dim);

		LIKWID_MARKER_START("AlocVariavelTrab2");
		//Alocação de memória
		alocacaoVariaveisOpt(dim);
		LIKWID_MARKER_STOP("AlocVariavelTrab2");

		LIKWID_MARKER_START("LeVariavelTrab2");
		leituraVariaveisOpt(dim, input, &epsilon, &maxIter);
		LIKWID_MARKER_STOP("LeVariavelTrab2");

		LIKWID_MARKER_START("EscreveParciaisTrab2");
		tempos[1] = escreveDerivadasParciaisOpt(dim, output);
		LIKWID_MARKER_STOP("EscreveParciaisTrab2");

		LIKWID_MARKER_START("MetodoNewtonTrab2");

		// Método de Newton
		// Objetivo: Devolver uma solução refinada para o Sistema linear passado
		tempoExec = timestamp();

		//Loop epsilon < max resultadoJacobianaOpt, epsilon < max F(x) , iter < maxIter
		for (iter = 0; iter < maxIter; iter++)
		{
			impressaoResultadosOpt(dim, output, resultadosOpt);

			LIKWID_MARKER_START("MatrizJacobianaTrab2");
			tempos[2] = timestamp();

			// Metodo de jacobi
			// Objetivo: Criar matriz jacobianaOpt para a resposta atual
			for (i = 0; i < dim; i++)
			{	
				for (int j = 0; j < dim-dim%4; j+=4)
				{
					jacobianaOpt[(i*dim)+j] = evaluator_evaluate(derivadas[(i*dim)+j], dim, (char **)variaveis, resultadosOpt);
					jacobianaOpt[(i*dim)+j+1] = evaluator_evaluate(derivadas[(i*dim)+j+1], dim, (char **)variaveis, resultadosOpt);
					jacobianaOpt[(i*dim)+j+2] = evaluator_evaluate(derivadas[(i*dim)+j+2], dim, (char **)variaveis, resultadosOpt);
					jacobianaOpt[(i*dim)+j+3] = evaluator_evaluate(derivadas[(i*dim)+j+3], dim, (char **)variaveis, resultadosOpt);
				}
				for (int j = dim-dim%4; j < dim; j++)
				{
					jacobianaOpt[(i*dim)+j] = evaluator_evaluate(derivadas[(i*dim)+j], dim, (char **)variaveis, resultadosOpt);
				}
				resultadoEquacoes[i] = evaluator_evaluate(equacoes[i], dim, (char **)variaveis, resultadosOpt);
			}

			// O tempo de cálculo utilizando o método de newton
			tempos[2] = timestamp() - tempos[2];
			
			LIKWID_MARKER_STOP("MatrizJacobianaTrab2");

			//Verificação de parada
			if (maxOpt(resultadoEquacoes, dim) < epsilon){
				tempos[0] = timestamp() - tempoExec;
			}
		
			LIKWID_MARKER_START("PivoteamentoTrab2");

			// Pivoteamento parcial de Gauss
			// Objetivo: Aplicar Gauss com Pivoteamento Parcial para refinar a resposta
			// Variaveis:
			// 			n: Tamanho do vetor
			// Retorna o tempo de cálculo utilizando o método de newton
			tempos[3] = timestamp();

			
			for(i=0;i<dim;i++){
				// Pivoteamento parcial
				for(k=i+1;k<dim;k++){
					// Caso o elemento da diagonal for menor que os termos abaixo
					if(fabs(jacobianaOpt[(i*dim)+i])<fabs(jacobianaOpt[(k*dim)+i])){
						//Troca as linhas
						LIKWID_MARKER_START("TrocaLinhasTrab2");
						trocaLinhasOpt(dim, i, k);
						LIKWID_MARKER_STOP("TrocaLinhasTrab2");
					}
				}
				//Realiza eliminação de gauss
				LIKWID_MARKER_START("EliminacaoGaussTrab2");
				// Eliminação de Gauss
				// Objetivo: Faz a eliminação de Gauss
				for(k=i+1;k<dim;k++){
					term= jacobianaOpt[(k*dim)+i]/ jacobianaOpt[(i*dim)+i];
					for( j=0;j<dim-dim%4;j+=4){
						jacobianaOpt[(k*dim)+j]=jacobianaOpt[(k*dim)+j]-term*jacobianaOpt[(i*dim)+j];
						jacobianaOpt[(k*dim)+j+1]=jacobianaOpt[(k*dim)+j+1]-term*jacobianaOpt[(i*dim)+j+1];
						jacobianaOpt[(k*dim)+j+2]=jacobianaOpt[(k*dim)+j+2]-term*jacobianaOpt[(i*dim)+j+2];
						jacobianaOpt[(k*dim)+j+3]=jacobianaOpt[(k*dim)+j+3]-term*jacobianaOpt[(i*dim)+j+3];
					}
					for (j = dim-dim%4; j < dim; j++)
					{
						jacobianaOpt[(k*dim)+j]=jacobianaOpt[(k*dim)+j]-term*jacobianaOpt[(i*dim)+j];
					}
					resultadoEquacoes[k] = resultadoEquacoes[k] - term * resultadoEquacoes[i];				
				}
				LIKWID_MARKER_STOP("EliminacaoGaussTrab2");
			}

			LIKWID_MARKER_START("ResultadoJacobianaTrab2");
			calculaResultadoMatrizJacobianaOpt(dim);
			LIKWID_MARKER_STOP("ResultadoJacobianaTrab2");
			
			tempos[3] = timestamp() - tempos[3];
			LIKWID_MARKER_STOP("PivoteamentoTrab2");
			if(tempos[3] == -1){
				return -1;
			}

			LIKWID_MARKER_START("ResultadoNewtonTrab2");
			calculoResultadoXOpt(dim);
			LIKWID_MARKER_STOP("ResultadoNewtonTrab2");

			// Verificação de parada
			if (maxOpt(resultadoJacobianaOpt,dim) < epsilon){
				return timestamp() - tempoExec;
			}					
		}
		// Finalização metodo de newton
		tempos[0] =  timestamp() - tempoExec;
		if(tempos[0] == -1){
			return -1;
		}
		LIKWID_MARKER_STOP("MetodoNewtonTrab2");

		LIKWID_MARKER_START("LiberaMemoriaTrab2");
		liberacaoMemoriaUsadaOpt(dim);
		LIKWID_MARKER_STOP("LiberaMemoriaTrab2");


		//Printar os tempos
		fprintf(output,"###########\n");
		fprintf(output,"# Tempo Total : %.6lf \n", tempos[0]);
		fprintf(output,"# Tempo Derivadas: %.6lf \n", tempos[1]);
		fprintf(output,"# Tempo jacobianaOpt: %.6lf \n", tempos[2]);
		fprintf(output,"# Tempo SL: %.6lf \n", tempos[3]);
		fprintf(output,"###########\n\n");
	}
	LIKWID_MARKER_STOP("StartNewtonTrab2");
	LIKWID_MARKER_CLOSE;
}