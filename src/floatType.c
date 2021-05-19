/*
 * source: https://randomascii.wordpress.com/2012/01/11/tricks-with-the-floating-point-format/
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>
#include "floatType.h"



/* imprime a union Float_t como ponto flutuante, hexadecimal e 
 * suas partes na forma de inteiros */
void printFloat_t( Float_t num )
{
   printf("f:%1.9e, ix:0x%08X, s:%d, e:%d, mx:0x%06X\n",
            num.f, num.i,
            num.parts.sign, num.parts.exponent, num.parts.mantissa); 
} 


/* calcula o epsilon relativo a um numero NUM. Ou seja, o valor tal
 * que NUM + epsilon seja > NUM */
Float_t calculaEpsilonRelativo( Float_t num )
{
    // Float_t epsilon = num;
    // epsilon.parts.mantissa += 1;

    Float_t epsilon;
    epsilon.f = num.f / 2.0f;
    while (num.f + epsilon.f / 2.0f > num.f)
        epsilon.f /= 2.0f;
    
    return epsilon;
}


int AlmostEqualRelative(double A, double B)
{
    // Calculate the difference.
    double diff, largest, relEpsilon;
    diff = fabs(A - B);
    A = fabs(A);
    B = fabs(B);
    // Find the largest
    largest = (B > A) ? B : A;
    relEpsilon = largest * FLT_EPSILON;

    printf("\tThe difference: ");
    printFloat_t(diff);
    
    printf("\trel. Epsilon:   ");
    printFloat_t(relEpsilon);
    
    if (diff <= relEpsilon)
        return 1;
    return 0;
}


int AlmostEqualUlps(Float_t A, Float_t B, int maxULPs)
{
 
    // Find the difference in ULPs.
    int ulpsDiff = abs(A.i - B.i);
    printf("\tULPs diff: %d\n", ulpsDiff);
    
    if (ulpsDiff < maxULPs)
        return 1;
    return 0;
}