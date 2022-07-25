/*
 * sign.c
 *
 * Code generation for function 'sign'
 *
 */

/* Include files */
#include "sign.h"

/* Function Definitions */
void b_sign(double x_data[], int x_size[1])
{
  int nx;
  int k;
  nx = x_size[0];
  for (k = 0; k + 1 <= nx; k++) {
    if (x_data[k] < 0.0) {
      x_data[k] = -1.0;
    } else if (x_data[k] > 0.0) {
      x_data[k] = 1.0;
    } else {
      if (x_data[k] == 0.0) {
        x_data[k] = 0.0;
      }
    }
  }
}

/* End of code generation (sign.c) */
