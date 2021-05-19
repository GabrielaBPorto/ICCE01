#ifndef _FLOATTYPE_H
#define _FLOATTYPE_H

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

void printFloat_t( Float_t num );
Float_t calculaEpsilonRelativo( Float_t num );
int AlmostEqualRelative(Float_t A, Float_t B);
int AlmostEqualUlps(Float_t A, Float_t B, int maxULPs);



//------------------------------------------------------------------------------
#endif
