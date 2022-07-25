/*
 * xgemv.c
 *
 * Code generation for function 'xgemv'
 *
 */

/* Include files */
#include "dll1.h"
#include "xgemv.h"

/* Function Definitions */
void xgemv(int m, int n, const emxArray_real_T *A, int ia0, int lda, const
           emxArray_real_T *x, int ix0, double y[7])
{
  int iy;
  int i19;
  int iac;
  int ix;
  double c;
  int i20;
  int ia;
  if (n != 0) {
    for (iy = 1; iy <= n; iy++) {
      y[iy - 1] = 0.0;
    }

    iy = 0;
    i19 = ia0 + lda * (n - 1);
    iac = ia0;
    while ((lda > 0) && (iac <= i19)) {
      ix = ix0;
      c = 0.0;
      i20 = (iac + m) - 1;
      for (ia = iac; ia <= i20; ia++) {
        c += A->data[ia - 1] * x->data[ix - 1];
        ix++;
      }

      y[iy] += c;
      iy++;
      iac += lda;
    }
  }
}

/* End of code generation (xgemv.c) */
