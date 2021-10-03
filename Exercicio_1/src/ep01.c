#include <stdio.h>
#include <matheval.h>
#include <assert.h>
#include <string.h>
#include "floatType.h"
#include <math.h>

#define TAM_BUFFER 100
char funcao[TAM_BUFFER];
double x0;
double epsilon;
int maxIter;
double resultado_anterior_newton, resultado_newton, parada_newton, resultado_anterior_secante,
resultado_secante, parada_secante, erro_absoluto, erro_relativo, ulps;
int refino_newton = 0, refino_secante = 0;

void *f, *f_prim;

void leituraVariavel(){
	
	fgets(funcao, TAM_BUFFER, stdin);
	int length = strlen (funcao);
	if (length > 0 && funcao[length - 1] == '\n')
		funcao[length - 1] = '\0';
	scanf("%lg", &x0);
	scanf("%lg", &epsilon);
	scanf("%d", &maxIter);

}

int verification_newton(){
	parada_newton = fabs(resultado_newton - resultado_anterior_newton);
	return parada_newton < epsilon;
}

void metodoNewton(){
	double divisao = (evaluator_evaluate_x(f,resultado_anterior_newton) / evaluator_evaluate_x(f_prim,resultado_anterior_newton));
	resultado_newton = resultado_anterior_newton - divisao;
	if(verification_newton()){
		refino_newton = 1;
	}
}

int main(){

	leituraVariavel();

	// validar passando o primeiro valor do x0
	// esta devolvendo alguma coisa que existe
	/* Create evaluator for function.  */

	f = evaluator_create(funcao);
	//printf("%s\n",evaluator_get_string(f));
	assert (f);
	
	/* Create evaluator for function derivative and print textual
		representation of derivative.  */
	f_prim = evaluator_derivative_x (f);

	resultado_anterior_newton = resultado_anterior_secante = x0;
	for(int i=0; i< maxIter && (!refino_newton); i++){
		metodoNewton();
		printf("%d, %1.16e, %1.16e \n", i, resultado_newton, parada_newton);
		// printf("%d, %1.16e, %1.16e, %1.16e, %1.16e, %1.16e, %1.16e, %1.16e ", i, resultado_newton,
		// 	parada_newton, resultado_secante, parada_secante, erro_absoluto, erro_relativo, ulps);
		resultado_anterior_newton = resultado_newton;
	}

	// printf("----TERMINO INPUT----\n");

	// printf("  f'(x) = %s\n", evaluator_get_string (f_prim));
	// printf ("  f(%g) = %g\n", x0, evaluator_evaluate_x (f, x0));
	// printf ("  f'(%g) = %g\n", x0, evaluator_evaluate_x (f_prim, x0));

	return 0;
}