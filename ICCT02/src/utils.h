#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdlib.h>
#include <sys/time.h>
#include <likwid.h>

/*  Retorna tempo em milisegundos

    Forma de uso:
 
    double tempo;
    tempo = timestamp();
    <trecho de programa do qual se deseja medir tempo>
    tempo = timestamp() - tempo;
*/

double timestamp(void);

typedef char * string_t;

string_t markerName(string_t baseName, int n);
int isPot2(int n);
void troca(double *a, double *b);

#endif // __UTILS_H__
