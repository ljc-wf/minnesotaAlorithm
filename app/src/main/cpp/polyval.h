/*
 * polyval.h
 *
 * Code generation for function 'polyval'
 *
 */

#ifndef POLYVAL_H
#define POLYVAL_H

/* Include files */
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rt_nonfinite.h"
#include "rtwtypes.h"
#include "dll1_types.h"

/* Function Declarations */
extern void polyval(const double p[7], double x_data[], int x_size[2], const
                    double mu[2], double y_data[], int y_size[2]);

#endif

/* End of code generation (polyval.h) */
