/*
 * xgerc.c
 *
 * Code generation for function 'xgerc'
 *
 */

/* Include files */
#include "dll1.h"
#include "xgerc.h"

/* Function Definitions */
void xgerc(int m, int n, double alpha1, int ix0, const double y[7],
           emxArray_real_T *A, int ia0, int lda)
{
  int jA;
  int jy;
  int j;
  double temp;
  int ix;
  int i21;
  int ijA;
  if (!(alpha1 == 0.0)) {
    jA = ia0 - 1;
    jy = 0;
    for (j = 1; j <= n; j++) {
      if (y[jy] != 0.0) {
        temp = y[jy] * alpha1;
        ix = ix0;
        i21 = m + jA;
        for (ijA = jA; ijA + 1 <= i21; ijA++) {
          A->data[ijA] += A->data[ix - 1] * temp;
          ix++;
        }
      }

      jy++;
      jA += lda;
    }
  }
}

/* End of code generation (xgerc.c) */
