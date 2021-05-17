/*
 * source: https://randomascii.wordpress.com/2012/01/11/tricks-with-the-floating-point-format/
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>

typedef union
{
    int32_t i;
    float f;
    struct
    {   // Bitfields for exploration.
        uint32_t mantissa : 23;
        uint32_t exponent : 8;
        uint32_t sign : 1;
    } parts;
} Float_t;

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


int AlmostEqualRelative(Float_t A, Float_t B)
{
    // Calculate the difference.
    Float_t diff, largest, relEpsilon;
    diff.f = fabs(A.f - B.f);
    A.f = fabs(A.f);
    B.f = fabs(B.f);
    // Find the largest
    largest.f = (B.f > A.f) ? B.f : A.f;
    relEpsilon.f = largest.f * FLT_EPSILON;

    printf("\tThe difference: ");
    printFloat_t(diff);
    
    printf("\trel. Epsilon:   ");
    printFloat_t(relEpsilon);
    
    if (diff.f <= relEpsilon.f)
        return 1;
    return 0;
}


int AlmostEqualUlps(Float_t A, Float_t B, int maxULPs)
{
    // Different signs means they do not match.
    if (A.parts.sign != B.parts.sign)
    {
        // Check for equality to make sure +0==-0
        if (A.f == B.f)
            return 1;
        return 0;
    }
 
    // Find the difference in ULPs.
    int ulpsDiff = abs(A.i - B.i);
    printf("\tULPs diff: %d\n", ulpsDiff);
    
    if (ulpsDiff < maxULPs)
        return 1;
    return 0;
}


int main()
{
   Float_t num, num2;
   num.f=1.0f;
   num2.f=1000.0f;
   
   /* ***********************************************
    * Mostra o valor de FLT_EPSILON e calcula o Epsilon relativo
    * a um numero dado.
    * Testar com:
    *   1.0: resultado FLT_EPSILON
    *   16777215: maior número impar
    *   16777216: epsilon = 2.0
    *************************************************/   
 /*   printf("\nEpsilon: %1.15f, Epsilon: %1.9e\n", FLT_EPSILON, FLT_EPSILON);
   
    printf("\nDigite um numero: ");
    scanf("%f", &num.f);
    printf("\n Epsilon: ");
    printFloat_t( calculaEpsilonRelativo(num) );
    /*************************************************/


   /******************************************************
    * Lê um número e imprime o próximo e a diferença entre eles
    * Testar com:
    *   1.0: resultado FLT_EPSILON
    *   16777215: maior número impar
    * *****************************************************/
 /*   printf("\nDigite um numero: ");
    scanf("%f", &num.f);
    for (int i=0; i<5; i++)
    {
        Float_t next, diff;
        next.i = num.i+1;
        diff.f = fabs(next.f - num.f);
        printf("\n");
        printFloat_t( num );
        printFloat_t( next );
        printFloat_t( diff );
      
        num = next;
    }
    /*******************************************************/


    /**************************************************
     * Compara dois numeros usando tres técnicas diferentes
     * Testar com:
     *    1.99999988 e 2.0 (numeros consecutivos)
     *    16777215 e 16777216 (diferença grande mas é a menor possível)
     *    0.000001 e 0.0  (nunca serao iguais em relativo e ULP)
     * 
     * ************************************************/
    printf("\nDigite dois numeros reais: ");
    while (scanf("%f %f", &(num.f), &(num2.f)) == 2)
    {
        printf("\nEpsilon: %1.15f, Epsilon: %1.9e\n", FLT_EPSILON, FLT_EPSILON);
        
        printf("\nOs numeros :\n\t");
        printFloat_t(num); printf("\t");
        printFloat_t(num2);
        
        Float_t diff;
        diff.f = fabs(num.f - num2.f);
        printf("\n\tSubtraidos: "); 
        printFloat_t(diff);
        
        printf("\n\n\tComparacao Epsilon Relativo: \n");
        AlmostEqualRelative(num, num2);
            

        printf("\n\n\tComparacao ULPs: \n");
        AlmostEqualUlps(num, num2, 10);

        printf("\nDigite dois numeros reais: ");
    }
    /*******************************************************/


   return 0;
}
