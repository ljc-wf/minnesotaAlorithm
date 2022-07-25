/*
 * polyfit.h
 *
 * Code generation for function 'polyfit'
 *
 */

#ifndef POLYFIT_H
#define POLYFIT_H

/* Include files */
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rt_nonfinite.h"
#include "rtwtypes.h"
#include "dll1_types.h"

/* Function Declarations */
extern void b_polyfit(const double x_data[], const int x_size[2], const double
                      y_data[], double p[7], double S_R_data[], int S_R_size[2],
                      double *S_df, double *S_normr, double mu[2]);
extern void polyfit(const double x_data[], const int x_size[2], const double
                    y_data[], const int y_size[2], double p[7], double S_R_data[],
                    int S_R_size[2], double *S_df, double *S_normr, double mu[2]);

#endif

/* End of code generation (polyfit.h) */
