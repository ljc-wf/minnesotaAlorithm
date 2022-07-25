/*
 * diff.h
 *
 * Code generation for function 'diff'
 *
 */

#ifndef DIFF_H
#define DIFF_H

/* Include files */
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rt_nonfinite.h"
#include "rtwtypes.h"
#include "dll1_types.h"

/* Function Declarations */
extern void b_diff(const emxArray_real_T *x, emxArray_real_T *y);
extern void diff(const double x_data[], const int x_size[1], double y_data[],
                 int y_size[1]);

#endif

/* End of code generation (diff.h) */
