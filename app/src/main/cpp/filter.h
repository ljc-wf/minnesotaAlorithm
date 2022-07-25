/*
 * filter.h
 *
 * Code generation for function 'filter'
 *
 */

#ifndef FILTER_H
#define FILTER_H

/* Include files */
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rt_nonfinite.h"
#include "rtwtypes.h"
#include "dll1_types.h"

/* Function Declarations */
extern void filter(const double x_data[], const int x_size[1], const double zi[6],
                   double y_data[], int y_size[1]);
extern void b_filter(const double b[245], const double x[3264], const double zi
[244], double y[3264]);
extern void filter2(const double b[29], const double x[1968], const double zi[28],
                   double y[1968]);
#endif

/* End of code generation (filter.h) */
