/*
 * polyval.c
 *
 * Code generation for function 'polyval'
 *
 */

/* Include files */
#include "polyval.h"

/* Function Definitions */
void polyval(const double p[7], double x_data[], int x_size[2], const double mu
             [2], double y_data[], int y_size[2])
{
  int loop_ub;
  int i10;
  int k;
  loop_ub = x_size[1];
  for (i10 = 0; i10 < loop_ub; i10++) {
    x_data[i10] = (x_data[i10] - mu[0]) / mu[1];
  }

  y_size[0] = 1;
  y_size[1] = (short)x_size[1];
  if (!((short)x_size[1] == 0)) {
    y_size[0] = 1;
    loop_ub = (short)x_size[1];
    for (i10 = 0; i10 < loop_ub; i10++) {
      y_data[i10] = p[0];
    }

    for (k = 0; k < 6; k++) {
      y_size[0] = 1;
      y_size[1] = x_size[1];
      loop_ub = x_size[1];
      for (i10 = 0; i10 < loop_ub; i10++) {
        y_data[i10] = x_data[i10] * y_data[i10] + p[k + 1];
      }
    }
  }
}

/* End of code generation (polyval.c) */
