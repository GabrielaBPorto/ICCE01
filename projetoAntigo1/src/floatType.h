#ifndef _FLOATTYPE_H
#define _FLOATTYPE_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>

typedef union
{
    int64_t i;
    float f;
    struct
    {   // Bitfields for exploration.
        uint64_t mantissa : 53;
        uint64_t exponent : 11;
        uint64_t sign : 1;
    } parts;
} double_t;

void printFloat_t( double_t num );
double_t calculaEpsilonRelativo( double_t num );
int AlmostEqualRelative(double_t A, double_t B);
int AlmostEqualUlps(double_t A, double_t B, int maxULPs);



//------------------------------------------------------------------------------
#endif
