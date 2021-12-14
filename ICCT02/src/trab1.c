#include <stdio.h>
#include "utils.h"
#include "newton.h"
#include <string.h>
#include <matheval.h>
#include <assert.h>
#include <likwid.h>

int main(int argc, char *argv[]){

	LIKWID_MARKER_INIT;
  	LIKWID_MARKER_START("StartNewtonTrab1");

	// Declaração de variavel
	int dim;
	void *eval;
	double tempos[4];

	output = stdout;
	// trataSaida(argc, argv);
	input = stdin;

	printf("Tratou saida\n");

	while (fscanf(input, "%d\n", &dim) != EOF)
	{
		fprintf(output,"%d\n", dim);

		alocacaoVariaveis(dim);

		leituraVariaveis(dim);

		tempos[1] = escreveDerivadasParciais(dim);

		tempos[0] = newtonMethod(dim, tempos);
		if(tempos[0] == -1){
			return -1;
		}

		liberacaoMemoriaUsada(dim);

		//Printar os tempos
		fprintf(output,"###########\n");
		fprintf(output,"# Tempo Total : %.6lf \n", tempos[0]);
		fprintf(output,"# Tempo Derivadas: %.6lf \n", tempos[1]);
		fprintf(output,"# Tempo Jacobiana: %.6lf \n", tempos[2]);
		fprintf(output,"# Tempo SL: %.6lf \n", tempos[3]);
		fprintf(output,"###########\n\n");
	}
	LIKWID_MARKER_STOP("StartNewtonTrab1");
	LIKWID_MARKER_CLOSE;

	return 0;
}