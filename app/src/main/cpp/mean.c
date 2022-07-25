/*
 * mean.c
 *
 * Code generation for function 'mean'
 *
 */

/* Include files */
#include "mean.h"

/* Function Definitions */
double b_mean(const double x_data[], const int x_size[2])
{
  double y;
  int k;

  if (x_size[1] == 0) {
    y = 0.0;
  } else {
    y = x_data[0];
    for (k = 2; k <= x_size[1]; k++) {
      y += x_data[k - 1];
    }
  }

  y /= (double)x_size[1];
  return y;
}

double mean(const double x[300])
{
  double y;
  int k;
  y = x[0];
  for (k = 0; k < 299; k++) {
    y += x[k + 1];
  }

  y /= 300.0;
  return y;
}

/* End of code generation (mean.c) */
