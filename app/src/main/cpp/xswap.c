/*
 * xswap.c
 *
 * Code generation for function 'xswap'
 *
 */

/* Include files */
#include "dll1.h"
#include "xswap.h"

/* Function Definitions */
void xswap(int n, emxArray_real_T *x, int ix0, int iy0)
{
  int ix;
  int iy;
  int k;
  double temp;
  ix = ix0 - 1;
  iy = iy0 - 1;
  for (k = 1; k <= n; k++) {
    temp = x->data[ix];
    x->data[ix] = x->data[iy];
    x->data[iy] = temp;
    ix++;
    iy++;
  }
}

/* End of code generation (xswap.c) */
