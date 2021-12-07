#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <likwid.h>

#include "utils.h"
#include "libGaussJacobi.h"

void gaussJacobi(FILE *f_in, FILE *f_out) {
  LIKWID_MARKER_INIT;
  LIKWID_MARKER_START("StartNotOpt");
  int n;
  fscanf(f_in, "%d", &n);

  // -----------------------------------------  alocação:
  LIKWID_MARKER_START("AllocNotOpt");
  double **A = (double **) malloc(sizeof(double *) * n);
  for (int i = 0; i < n; ++i)
    A[i] = (double *) malloc(sizeof(double) * n);
  double *b = (double *) malloc(sizeof(double) * n);
  double *x = (double *) malloc(sizeof(double) * n);
  LIKWID_MARKER_STOP("AllocNotOpt");
  // --------------------------------------  leitura: matriz A, vetor b, vetor x
  LIKWID_MARKER_START("ReadNotOpt");
  for (int i = 0; i < n; ++i)
    for (int j = 0; j < n; ++j)
      fscanf(f_in, "%lf", &(A[i][j]));
  for (int i = 0; i < n; ++i)
    fscanf(f_in, "%lf", &(b[i]));
  for (int i = 0; i < n; ++i)
    fscanf(f_in, "%lf", &(x[i]));
  LIKWID_MARKER_STOP("ReadNotOpt");
  // --------------------------------------------------  Método:
  LIKWID_MARKER_START("JacobiNotOpt");
  double *x1 = (double *) malloc(sizeof(double) * n);

  for (int k = 0; k < MAXIT; ++k) {
    for (int i = 0; i < n; ++i) {
      double soma = 0.0;
      for (int j = 0; j < n; ++j)
        if (j != i)
          soma += A[i][j] * x[j];
      x1[i] = (b[i] - soma) / A[i][i];
    }
    for (int i = 0; i < n; ++i)
      x[i] = x1[i];
  }
  LIKWID_MARKER_STOP("JacobiNotOpt");
  // --------------------------------------------------  Resultados e liberação:
  fprintf(f_out, "----------\nGauss-Jacobi\n");
  for (int i = 0; i < n; ++i)
    fprintf(f_out, "x%d = %1.15g\n", i+1, x[i]);
  LIKWID_MARKER_START("FreeNotOpt");
  for (int i = 0; i < n; ++i)
    free(A[i]);
  free(A);
  free(b);
  free(x);
  free(x1);
  LIKWID_MARKER_STOP("FreeNotOpt");
  LIKWID_MARKER_STOP("StartNotOpt");
  LIKWID_MARKER_CLOSE;
}

