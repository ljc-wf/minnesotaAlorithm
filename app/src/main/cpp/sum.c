/*
 * sum.c
 *
 * Code generation for function 'sum'
 *
 */

/* Include files */
#include "dll1.h"
#include "sum.h"

/* Function Definitions */
double sum(const emxArray_real_T *x)
{
  double y;
  int k;
  if (x->size[1] == 0) {
    y = 0.0;
  } else {
    y = x->data[0];
    for (k = 2; k <= x->size[1]; k++) {
      y += x->data[k - 1];
    }
  }

  return y;
}

/* End of code generation (sum.c) */
