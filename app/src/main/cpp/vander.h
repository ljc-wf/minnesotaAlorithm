/*
 * vander.h
 *
 * Code generation for function 'vander'
 *
 */

#ifndef VANDER_H
#define VANDER_H

/* Include files */
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rt_nonfinite.h"
#include "rtwtypes.h"
#include "dll1_types.h"

/* Function Declarations */
extern void vander(const double v_data[], const int v_size[2], emxArray_real_T
                   *A);

#endif

/* End of code generation (vander.h) */
