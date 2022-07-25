/*
 * std.c
 *
 * Code generation for function 'std'
 *
 */

/* Include files */
#include "dll1.h"
#include "std.h"

/* Function Definitions */
double b_std(const double varargin_1_data[], const int varargin_1_size[2])
{
  double y;
  int i8;
  int ix;
  double xbar;
  int k;
  double r;
  int b_varargin_1_size;
  i8 = varargin_1_size[1];
  if (varargin_1_size[1] == 0) {
    y = 0.0;
  } else {
    ix = 0;
    xbar = varargin_1_data[0];
    for (k = 2; k <= i8; k++) {
      ix++;
      xbar += varargin_1_data[ix];
    }

    xbar /= (double)varargin_1_size[1];
    ix = 0;
    r = varargin_1_data[0] - xbar;
    y = r * r;
    for (k = 2; k <= i8; k++) {
      ix++;
      r = varargin_1_data[ix] - xbar;
      y += r * r;
    }

    if (varargin_1_size[1] > 1) {
      b_varargin_1_size = varargin_1_size[1] - 1;
    } else {
      b_varargin_1_size = varargin_1_size[1];
    }

    y /= (double)b_varargin_1_size;
  }

  return sqrt(y);
}

/* End of code generation (std.c) */
