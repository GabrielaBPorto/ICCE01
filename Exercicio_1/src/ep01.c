#include <stdio.h>
#include <matheval.h>
#include <assert.h>
#include <string.h>

#define TAM_BUFFER 100
char funcao[TAM_BUFFER];
double x0;
double epsilon;
int maxIter;

void leituraVariavel(){
	fgets(funcao, TAM_BUFFER, stdin);
	int length = strlen (funcao);
	if (length > 0 && funcao[length - 1] == '\n')
		funcao[length - 1] = '\0';
	scanf("%lg", &x0);
	scanf("%lg", &epsilon);
	scanf("%d", &maxIter);
}


int main(){

	leituraVariavel();

	// validar passando o primeiro valor do x0
	// esta devolvendo alguma coisa que existe
	/* Create evaluator for function.  */

	void *f = evaluator_create(funcao);
	//printf("%s\n",evaluator_get_string(f));
	assert (f);
	
	/* Create evaluator for function derivative and print textual
		representation of derivative.  */
	void *f_prim = evaluator_derivative_x (f);


	printf("%s\n",funcao);
	printf("%lg\n",x0);
	printf("%lg\n",epsilon);
	printf("%d\n",maxIter);

	printf("----TERMINO INPUT----\n");

	printf("  f'(x) = %s\n", evaluator_get_string (f_prim));
	printf ("  f(%g) = %g\n", x0, evaluator_evaluate_x (f, x0));
	printf ("  f'(%g) = %g\n", x0, evaluator_evaluate_x (f_prim, x0));

	return 0;
}