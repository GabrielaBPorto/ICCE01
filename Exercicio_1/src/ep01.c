#include <stdio.h>
#include <matheval.h>
#include <assert.h>
#include <string.h>
#include <math.h>

#define TAM_BUFFER 100
char funcao[TAM_BUFFER];
double x0;
double epsilon;
int maxIter;
double resultado_anterior_newton, resultado_newton, parada_newton, resultado_anterior_secante,
resultado_secante, parada_secante, erro_absoluto, erro_relativo, ulps, resultado_anteanterior_secante;
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

double verifica(double result1, double result2){
	return fabs(result1 - result2);
}

int verification_proximidade_zero(double divisor){
	if(fabs(divisor) < epsilon){
		return 1;
	}
	return 0;
}

void metodoNewton(){
	double dividendo = (evaluator_evaluate_x(f,resultado_anterior_newton));
	double divisor = evaluator_evaluate_x(f_prim,resultado_anterior_newton);
	if(verification_proximidade_zero(divisor)){
		refino_newton = 1;
		return;
	}

	double resultadoDivisao = dividendo / divisor;
	resultado_newton = resultado_anterior_newton - resultadoDivisao;

	parada_newton = verifica(resultado_newton, resultado_anterior_newton);
	if(parada_newton < epsilon){
		refino_newton = 1;
	}
}

void metodoSecante(){
	double dividendo1 = resultado_anterior_secante - ( resultado_anterior_secante - resultado_anteanterior_secante);
	double dividendo2 = evaluator_evaluate_x(f,resultado_anterior_secante);
	double divisor = evaluator_evaluate_x(f,resultado_anterior_secante) * evaluator_evaluate_x(f,resultado_anteanterior_secante);
	if(verification_proximidade_zero(divisor)){
		refino_secante = 1;
		return;
	}

	double dividendo = dividendo1 * dividendo2;
	double resultado = dividendo / divisor;
	resultado_secante = resultado;

	parada_secante = verifica(resultado_secante, resultado_anterior_secante);
	if(parada_secante < epsilon){
		refino_secante = 1;
	}
}

int main(){

	leituraVariavel();

	// Passos necessários para o uso do Método iterativo Newton
	f = evaluator_create(funcao);
	assert (f);
	
	/* Create evaluator for function derivative and print textual
		representation of derivative.  */
	f_prim = evaluator_derivative_x (f);

	resultado_anterior_newton = resultado_anteanterior_secante = x0;
	for(int i=1; i<= maxIter && (!refino_newton || !refino_secante); i++){
		metodoNewton();
		if(i>1){
			metodoSecante();
			resultado_anteanterior_secante = resultado_anterior_secante;
			resultado_anterior_secante = resultado_secante;
		}
		else if(i==1){
			resultado_anterior_secante = resultado_newton;
		}
		printf("%d, %1.16e, %1.16e, %1.16e, %1.16e \n", i, resultado_newton, parada_newton, resultado_secante, parada_secante);
		// printf("%d, %1.16e, %1.16e, %1.16e, %1.16e, %1.16e, %1.16e, %1.16e ", i, resultado_newton,
		// 	parada_newton, resultado_secante, parada_secante, erro_absoluto, erro_relativo, ulps);
		resultado_anterior_newton = resultado_newton;
	}

	return 0;
}