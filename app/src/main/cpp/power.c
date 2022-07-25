/*
 * power.c
 *
 * Code generation for function 'power'
 *
 */

/* Include files */
#include "power.h"

/* Function Definitions */
void power(const double a_data[], const int a_size[1], double y_data[], int
           y_size[1])
{
  int k;
  y_size[0] = (short)a_size[0];
  for (k = 0; k + 1 <= a_size[0]; k++) {
    y_data[k] = a_data[k] * a_data[k];
  }
}

/* End of code generation (power.c) */
