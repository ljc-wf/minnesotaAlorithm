/*
 * vander.c
 *
 * Code generation for function 'vander'
 *
 */

/* Include files */
#include "dll1.h"
#include "vander.h"
#include "dll1_emxutil.h"

/* Function Definitions */
void vander(const double v_data[], const int v_size[2], emxArray_real_T *A)
{
  int n;
  int k;
  int j;
  n = v_size[1] - 1;
  k = A->size[0] * A->size[1];
  A->size[0] = v_size[1];
  A->size[1] = 7;
  emxEnsureCapacity((emxArray__common *)A, k, (int)sizeof(double));
  if (A->size[0] != 0) {
    for (k = 0; k <= n; k++) {
      A->data[k + A->size[0] * 6] = 1.0;
    }

    for (k = 0; k <= n; k++) {
      A->data[k + A->size[0] * 5] = v_data[k];
    }

    for (j = 0; j < 5; j++) {
      for (k = 0; k <= n; k++) {
        A->data[k + A->size[0] * (4 - j)] = v_data[k] * A->data[k + A->size[0] *
          (5 - j)];
      }
    }
  }
}

/* End of code generation (vander.c) */
