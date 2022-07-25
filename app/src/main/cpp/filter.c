/*
 * filter.c
 *
 * Code generation for function 'filter'
 *
 */

/* Include files */
#include "rt_nonfinite.h"
#include "dll1.h"
#include "filter.h"

// start
void b_filter(const double b[245], const double x[3264], const double zi[244],
              double y[3264])
{
  int k;
  int j;
  memcpy(&y[0], &zi[0], 244U * sizeof(double));
  memset(&y[244], 0, 3020U * sizeof(double));
  for (k = 0; k < 245; k++) {
    for (j = k; j + 1 < 3265; j++) {
      y[j] += b[k] * x[j - k];
    }
  }
}

void filter2(const double b[29], const double x[1968], const double zi[28],
            double y[1968])
{
  int k;
  int j;
  memcpy(&y[0], &zi[0], 28U * sizeof(double));
  memset(&y[28], 0, 1940U * sizeof(double));
  for (k = 0; k < 29; k++) {
    for (j = k; j + 1 < 1969; j++) {
      y[j] += b[k] * x[j - k];
    }
  }
}

/* End of code generation (filter.c) */


// end


/* Function Definitions */
void filter(const double x_data[], const int x_size[1], const double zi[6],
            double y_data[], int y_size[1])
{
  int nx;
  int k;
  int naxpy;
  int j;
  static const double dv0[7] = { 0.00629697191242768, 0.0,
    -0.018890915737283038, 0.0, 0.018890915737283038, 0.0,
    -0.00629697191242768 };

  double as;
  static const double dv1[7] = { 1.0, -4.8264173453373385, 10.003725664742968,
    -11.400852147993284, 7.5388551329940228, -2.7437089158828396,
    0.42990882485073983 };

  y_size[0] = (short)x_size[0];
  nx = x_size[0];
  for (k = 0; k < 6; k++) {
    y_data[k] = zi[k];
  }

  for (k = 7; k <= nx; k++) {
    y_data[k - 1] = 0.0;
  }

  for (k = 1; k <= nx; k++) {
    naxpy = (nx - k) + 1;
    if (!(naxpy <= 7)) {
      naxpy = 7;
    }

    for (j = -1; j + 2 <= naxpy; j++) {
      y_data[k + j] += x_data[k - 1] * dv0[j + 1];
    }

    naxpy = nx - k;
    if (!(naxpy <= 6)) {
      naxpy = 6;
    }

    as = -y_data[k - 1];
    for (j = 1; j <= naxpy; j++) {
      y_data[(k + j) - 1] += as * dv1[j];
    }
  }
}

/* End of code generation (filter.c) */
