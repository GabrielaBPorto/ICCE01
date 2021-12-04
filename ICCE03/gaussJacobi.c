#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <likwid.h>

#include "utils.h"
#include "libGaussJacobi.h"

void gaussJacobi(FILE *f_in, FILE *f_out) {
  LIKWID_MARKER_INIT;
  LIKWID_MARKER_START("StartGaussJacobiNotOpt");
  int n;
  fscanf(f_in, "%d", &n);

  // -----------------------------------------  alocação:
  LIKWID_MARKER_START("AllocationStartJacobiNotOpt");
  double **A = (double **) malloc(sizeof(double *) * n);
  for (int i = 0; i < n; ++i)
    A[i] = (double *) malloc(sizeof(double) * n);
  double *b = (double *) malloc(sizeof(double) * n);
  double *x = (double *) malloc(sizeof(double) * n);
  LIKWID_MARKER_STOP("AllocationStopJacobiNotOpt");
  // --------------------------------------  leitura: matriz A, vetor b, vetor x
  LIKWID_MARKER_START("ReadingVectorStartJacobiNotOpt");
  for (int i = 0; i < n; ++i)
    for (int j = 0; j < n; ++j)
      fscanf(f_in, "%lf", &(A[i][j]));
  for (int i = 0; i < n; ++i)
    fscanf(f_in, "%lf", &(b[i]));
  for (int i = 0; i < n; ++i)
    fscanf(f_in, "%lf", &(x[i]));
  LIKWID_MARKER_STOP("ReadingVectorStopJacobiNotOpt");
  // --------------------------------------------------  Método:
  LIKWID_MARKER_START("MethodStartJacobiNotOpt");
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
  LIKWID_MARKER_STOP("MethodStopJacobiNotOpt");
  // --------------------------------------------------  Resultados e liberação:
  fprintf(f_out, "----------\nGauss-Jacobi\n");
  for (int i = 0; i < n; ++i)
    fprintf(f_out, "x%d = %1.15g\n", i+1, x[i]);
  LIKWID_MARKER_START("FreeStartJacobiNotOpt");
  for (int i = 0; i < n; ++i)
    free(A[i]);
  free(A);
  free(b);
  free(x);
  free(x1);
  LIKWID_MARKER_STOP("FreeStopJacobiNotOpt");
  LIKWID_MARKER_STOP("StopGaussJacobiNotOpt");
  LIKWID_MARKER_CLOSE;
}

