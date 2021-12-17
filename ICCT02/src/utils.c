#include "utils.h"
#include <stdio.h>
#include <likwid.h>
#include <string.h>
#include <math.h>
/*  Retorna tempo em milisegundos

    Forma de uso:
 
    double tempo;
    tempo = timestamp();
    <trecho de programa do qual se deseja medir tempo>
    tempo = timestamp() - tempo;
*/

double timestamp(void)
{
  struct timeval tp;
  gettimeofday(&tp, NULL);
  return((double)(tp.tv_sec*1000.0 + tp.tv_usec/1000.0));
}

string_t markerName(string_t baseName, int n)
{
  string_t mark = (string_t) malloc( (strlen(baseName)+1) + (log10(n)+1) + 1 );

  sprintf(mark, "%s_%u", baseName,n);

  return mark;
}

int isPot2(int n)
{
  int k;
  return (k = log2(n)) == log2(n) ;
}

// Troca
// Objetivo: Fazer a troca de itens 
// Variaveis:
// 			a: Item 1 a ser trocado
// 			b: Item 2 a ser trocado
// Não retorna nada
void troca(double *a, double *b){
	double temp = *a;
	*a = *b;
	*b = temp;
}