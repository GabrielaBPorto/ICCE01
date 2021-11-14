#include <stdio.h>
#include "utils.h"
#include "newton.h"
#include <string.h>
#include <matheval.h>
#include <assert.h>

int main(int argc, char *argv[]){

	// Declaração de variavel
	double epsilon;
	int dim;
	void *eval;
	double tempoDerivada;

	output = trataSaida(argc, argv);
	input = fopen("../input/sistemas.dat","r+");

	while (fscanf(input, "%d\n", &dim) != EOF)
	{
		fprintf(output,"%d\n", dim);

		alocacaoVariaveis(dim);

		leituraVariaveis(dim);

		tempoDerivada = escreveDerivadasParciais(dim);
		
		fprintf(output,"#\n");

		jacobiano(dim);
		//Fim Loop
		liberacaoMemoriaUsada(dim);
		//Printar os tempos

		fprintf(output, "\n");
	}
	
	
	return 0;
}