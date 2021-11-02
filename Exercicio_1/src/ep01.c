#include <stdio.h>
#include <matheval.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <limits.h>
#include <float.h>
#include <stdint.h>


typedef union Double_t
{
    int64_t i;
    double f;
    struct
    {   // Bitfields for exploration.
        uint64_t mantissa : 53;
        uint64_t exponent : 11;
        uint64_t sign : 1;
    } parts;
} Double_t;

#define TAM_BUFFER 100
char funcao[TAM_BUFFER];
double x0;
double epsilon;
int maxIter;
int ulps;
// Variaveis usadas para Newton
double resultado_anterior_newton, resultado_newton, parada_newton;
// Variaveis usadas para Secante
double resultado_anteanterior_secante, resultado_anterior_secante, resultado_secante, parada_secante;
// Variaveis de erro
double erro_absoluto, erro_relativo;
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

int ulpsFunction(double A, double B)
{
	//Converte O double para o tipo Double_t
    Double_t double_newton, double_secante;
	double_secante.f = A;
	double_newton.f = B;
	int ulpsDiff;

	//Sinais diferentes não são proximos
    if (double_secante.parts.sign != double_newton.parts.sign)
    {
        // Checa +0==-0
        ulpsDiff = abs(abs(double_secante.i) - abs(double_newton.i));
    }
	else
	{
    	ulpsDiff = abs(double_secante.i - double_newton.i);
	}
    //Pega a diferença
    
    return ulpsDiff;
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
	printf("%d, %1.16e, %1.16e, %1.16e, %1.16e, %1.16e, %1.16e, %d \n", 
		0, x0, x0, x0, x0, 0.0, 0.0, 0);


	// Utilização da biblioteca para tratamento da função e trazer sua derivada
	f = evaluator_create(funcao);
	assert (f);
	f_prim = evaluator_derivative_x (f);

	//Inicialização de variavéis
	resultado_anterior_newton = resultado_anteanterior_secante = x0;
	for(int i=1; i<= maxIter && (!refino_newton || !refino_secante); i++){
		metodoNewton();
		printf("%d, %1.16e, %1.16e,", i, resultado_newton, parada_newton);
		if(i>1){
			metodoSecante();
			resultado_anteanterior_secante = resultado_anterior_secante;
			resultado_anterior_secante = resultado_secante;
			erro_absoluto = resultado_newton - resultado_secante;
			
			if(!verification_proximidade_zero(resultado_secante))
				erro_relativo = fabs(erro_absoluto) / fabs(resultado_newton);

			ulps = ulpsFunction(resultado_newton, resultado_secante);

			// Imprimindo resultados finais do estilo de csv.
			printf(" %1.16e, %1.16e, ", resultado_secante, parada_secante);
		}
		else if(i==1){
			// Inicialização de variavel
			resultado_anterior_secante = resultado_newton;
			printf(" %1.16e, %1.16e, ", resultado_newton, parada_newton);
		}
 
		printf("%1.16e, %1.16e, %d \n", erro_absoluto, erro_relativo, ulps);
		resultado_anterior_newton = resultado_newton;
	}

	return 0;
}