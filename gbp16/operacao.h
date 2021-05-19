#ifndef _OPERACAO_H
#define _OPERACAO_H

#include <stdio.h>
#include "intervalo.h"

double max(int args, ...);
double min(int args, ...);

void sum(dadosIntervalar_t dados, int i, int variavelA, int variavelB);
void subtract(dadosIntervalar_t dados, int i, int variavelA, int variavelB);
void multiplication(dadosIntervalar_t dados, int i, int variavelA, int variavelB);
void divide(dadosIntervalar_t dados, int i, int variavelA, int variavelB);


//------------------------------------------------------------------------------
#endif
