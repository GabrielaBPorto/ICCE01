#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define MIN -9.9999
#define MAX 9.9999

#define ABS(x) ((x<0)?(-x):(x))

#define RANDOM() ((MIN)+(rand()/((double)RAND_MAX/((MAX)-(MIN)))))

int main (int argc, char **argv) {

  if (argc < 2) {
    fprintf(stderr, "\nForma de uso: %s dimensão_SL\n\n", argv[0]);
    return 1;
  }
  
  int n = atoi(argv[1]);

  double *A = (double *) malloc(sizeof(double) * n*n);
  double *b = (double *) malloc(sizeof(double) * n);
  double *x = (double *) malloc(sizeof(double) * n);

  srand(time(NULL));

  for (int i = 0; i < n; ++i) {
    double soma = 0.0;
    for (int j = 0; j < n; ++j) {
      A[i*n+j] = RANDOM();
      soma += ABS(A[i*n+j]);
    }
    int sign = pow(-1, rand());
    A[i*n+i] = sign*soma;
  }

  for (int i = 0; i < n; ++i)
    x[i] = RANDOM();

  for (int i = 0; i < n; ++i) {
    b[i] = 0.0;
    for (int j = 0; j < n; ++j)
      b[i] += A[i*n+j]*x[j];
  }

  printf("%d\n", n);
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j)
      printf("%g ", A[i*n+j]);
    printf("\n");
  }
  for (int i = 0; i < n; ++i)
    printf("%g ", b[i]);
  printf("\n");
  for (int i = 0; i < n; ++i)
    printf("%d ", (int) x[i]);

  free(A);
  free(b);
  free(x);

  return 0;
}

