/*
 * norm.c
 *
 * Code generation for function 'norm'
 *
 */

/* Include files */
#include "dll1.h"
#include "norm.h"

/* Function Definitions */
double norm(const double x_data[], const int x_size[1])
{
  double y;
  double scale;
  int k;
  double absxk;
  double t;
  if (x_size[0] == 0) {
    y = 0.0;
  } else {
    y = 0.0;
    if (x_size[0] == 1) {
      y = fabs(x_data[0]);
    } else {
      scale = 2.2250738585072014E-308;
      for (k = 1; k <= x_size[0]; k++) {
        absxk = fabs(x_data[k - 1]);
        if (absxk > scale) {
          t = scale / absxk;
          y = 1.0 + y * t * t;
          scale = absxk;
        } else {
          t = absxk / scale;
          y += t * t;
        }
      }

      y = scale * sqrt(y);
    }
  }

  return y;
}

/* End of code generation (norm.c) */
