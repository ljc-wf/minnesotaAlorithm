/*
 * abs.c
 *
 * Code generation for function 'abs'
 *
 */

/* Include files */
#include "dll1.h"
#include "abs.h"
#include "dll1_emxutil.h"

/* Function Definitions */
void b_abs(const emxArray_real_T *x, emxArray_real_T *y)
{
  short iv1[2];
  int n;
  int k;
  for (n = 0; n < 2; n++) {
    iv1[n] = (short)x->size[n];
  }

  n = y->size[0] * y->size[1];
  y->size[0] = iv1[0];
  y->size[1] = iv1[1];
  emxEnsureCapacity((emxArray__common *)y, n, (int)sizeof(double));
  n = x->size[0] * x->size[1];
  for (k = 0; k + 1 <= n; k++) {
    y->data[k] = fabs(x->data[k]);
  }
}

/* End of code generation (abs.c) */
