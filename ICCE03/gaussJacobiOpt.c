#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "utils.h"
#include "libGaussJacobi.h"

#define PAD(n) (isPot2(n)?(n+1):(n))

void gaussJacobiOpt(FILE *f_in, FILE *f_out) {

  int n;
  fscanf(f_in, "%d", &n);

  // -----------------------------------------  alocação:
  double **A = (double **) malloc(sizeof(double *) * n);
  A[0] = (double *) malloc(sizeof(double) * n*PAD(n));
  for (int i = 1; i < n; ++i)
    A[i] = A[0] + i*PAD(n);
  double *b = (double *) malloc(sizeof(double) * PAD(n));
  double *x = (double *) malloc(sizeof(double) * PAD(n));

  // --------------------------------------  leitura: matriz A, vetor b, vetor x
  for (int i = 0; i < n; ++i)
    for (int j = 0; j < n; ++j)
      fscanf(f_in, "%lf", &(A[i][j]));
  for (int i = 0; i < n; ++i)
    fscanf(f_in, "%lf", &(b[i]));
  for (int i = 0; i < n; ++i)
    fscanf(f_in, "%lf", &(x[i]));

  // --------------------------------------------------  Método:
  double *x1 = (double *) malloc(sizeof(double) * PAD(n));
  double *x_atual = x;
  double *x_prox  = x1;

  for (int k = 0; k < MAXIT; ++k) {
    for (int i = 0; i < n-n%4; i+=4) {
      double soma[4] = {0.0, 0.0, 0.0, 0.0};
      for (int j = 0; j < n; ++j) {
        soma[0] += A[i][j]   * x_atual[j];
        soma[1] += A[i+1][j] * x_atual[j];
        soma[2] += A[i+2][j] * x_atual[j];
        soma[3] += A[i+3][j] * x_atual[j];
      }
      x_prox[i]   = (b[i]   - soma[0] + A[i][i]*x_atual[i])       / A[i][i];
      x_prox[i+1] = (b[i+1] - soma[1] + A[i+1][i+1]*x_atual[i+1]) / A[i+1][i+1];
      x_prox[i+2] = (b[i+2] - soma[2] + A[i+2][i+2]*x_atual[i+2]) / A[i+2][i+2];
      x_prox[i+3] = (b[i+3] - soma[3] + A[i+3][i+3]*x_atual[i+3]) / A[i+3][i+3];
    }
    for (int i = n-n%4; i < n; ++i) {
      double soma = 0.0;
      for (int j = 0; j < n; ++j) {
        soma += A[i][j] * x_atual[j];
      }
      x_prox[i] = (b[i] - soma + A[i][i]*x_atual[i]) / A[i][i];
    }
    double *aux = x_atual;
    x_atual = x_prox;
    x_prox = aux;
  }
  if (x_atual != x)
    memcpy(x, x_atual, sizeof(double) * PAD(n));

  // --------------------------------------------------  Resultados e liberação:
  fprintf(f_out, "----------\nGauss-Jacobi Otimizado\n");
  for (int i = 0; i < n; ++i)
    fprintf(f_out, "x%d = %1.15g\n", i+1, x[i]);
  free(A[0]);
  free(A);
  free(b);
  free(x);
  free(x1);
}

