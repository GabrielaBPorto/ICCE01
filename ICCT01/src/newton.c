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

FILE* trataSaida(int argc, char *argv[]){
	char outputName[TAM_BUFFER];
	FILE *output;
	for(int i=0;i<argc;i++){
		if(strcmp(argv[i],"-o")){
			if(i+1<argc){
				strcpy(outputName, argv[i+1]);
			}
		}
	}
	fprintf(stdout,"%s\n",outputName);
	if(outputName){
		output = fopen(outputName, "w+");
	}

	if(output == NULL && outputName == NULL){
		output = stdout;
	}

	return output;
}

// 
// Objetivo: Centralizar leitura de variavéis usadas ao longo do algoritmo
// Parâmetros: N para tam, K 
void leituraVariavel(FILE *input, int *n, int *k, double *epsilon, int *maxIter){
	
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

