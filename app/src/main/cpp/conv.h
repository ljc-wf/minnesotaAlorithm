/*
 * conv.h
 *
 * Code generation for function 'conv'
 *
 */

#ifndef CONV_H
#define CONV_H

/* Include files */
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rt_nonfinite.h"
#include "rtwtypes.h"
#include "dll1_types.h"

/* Function Declarations */
extern void b_conv(const double A_data[], const int A_size[1], double C_data[],
                   int C_size[1]);
extern void conv(const double A_data[], const int A_size[1], double C_data[],
                 int C_size[1]);

#endif

/* End of code generation (conv.h) */
