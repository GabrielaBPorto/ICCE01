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
// Variaveis usadas para Newton
double resultado_anterior_newton, resultado_newton, parada_newton;
// Variaveis usadas para Secante
double resultado_anteanterior_secante, resultado_anterior_secante, resultado_secante, parada_secante;
// Variaveis de erro
double erro_absoluto, erro_relativo, ulps;
//Variavéis de controle de iteração
int refino_newton = 0, refino_secante = 0;

//Variaveis para uso de biblioteca de tratamento de funções
void *f, *f_prim;

// Essa função tem como o objetivo de centralizar a leitura das variavéis globais
void leituraVariavel(){
	
	fgets(funcao, TAM_BUFFER, stdin);
	int length = strlen (funcao);
	if (length > 0 && funcao[length - 1] == '\n')
		funcao[length - 1] = '\0';
	scanf("%lg", &x0);
	scanf("%lg", &epsilon);
	scanf("%d", &maxIter);
	return;
}

//Função genérica de verificação de valor
double verifica(double result1, double result2){
	return fabs(result1 - result2);
}

//Função de verificação de proximo de zero usando epsilon
int verification_proximidade_zero(double divisor){
	if(fabs(divisor) < epsilon){
		return 1;
	}
	return 0;
}

//Função que faz o cálculo do método de newton
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

//Função que faz o cálculo do método da secante
void metodoSecante(){

	double dividendoInferior = (evaluator_evaluate_x(f, resultado_anterior_secante) - evaluator_evaluate_x(f, resultado_anteanterior_secante));
	double dividendoFinal = evaluator_evaluate_x(f, resultado_anterior_secante) / dividendoInferior;
	double subtracao = resultado_anterior_secante - resultado_anteanterior_secante;
	double multiplicacao = subtracao * dividendoFinal;
	double resultado = resultado_anterior_secante - multiplicacao;

	if(verification_proximidade_zero(dividendoInferior)){
		refino_secante = 1;
		return;
	}

	resultado_secante = resultado;

	parada_secante = verifica(resultado_secante, resultado_anterior_secante);
	if(parada_secante < epsilon){
		refino_secante = 1;
	}
}

int main(){

	leituraVariavel();

	// Printando iteração 0 com os valores iniciais dados pela leitura de variáveis
	printf("%d, %1.16e, %1.16e, %1.16e, %1.16e, %1.16e, %1.16e, %1.16e \n", 
		0, x0, x0, x0, x0, 0, 0, 0);


	// Passos necessários para o uso do Método iterativo Newton
	f = evaluator_create(funcao);
	assert (f);
	
	/* Create evaluator for function derivative and print textual
		representation of derivative.  */
	f_prim = evaluator_derivative_x (f);

	//Inicialização de variavéis
	resultado_anterior_newton = resultado_anteanterior_secante = x0;
	for(int i=1; i<= maxIter && (!refino_newton || !refino_secante); i++){
		metodoNewton();
		if(i>1){
			metodoSecante();
			resultado_anteanterior_secante = resultado_anterior_secante;
			resultado_anterior_secante = resultado_secante;
		}
		else if(i==1){
			// Inicialização de variavel
			resultado_anterior_secante = resultado_newton;
		}

		// Imprimindo resultados finais do estilo de csv.
		printf("%d, %1.16e, %1.16e, %1.16e, %1.16e, %1.16e, %1.16e, %1.16e \n", 
		i, resultado_newton, parada_newton, resultado_secante, parada_secante, erro_absoluto, erro_relativo, ulps);

		resultado_anterior_newton = resultado_newton;
	}

	return 0;
}