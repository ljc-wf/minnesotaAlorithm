/*
 * xgerc.h
 *
 * Code generation for function 'xgerc'
 *
 */

#ifndef XGERC_H
#define XGERC_H

/* Include files */
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rt_nonfinite.h"
#include "rtwtypes.h"
#include "dll1_types.h"

/* Function Declarations */
extern void xgerc(int m, int n, double alpha1, int ix0, const double y[7],
                  emxArray_real_T *A, int ia0, int lda);

#endif

/* End of code generation (xgerc.h) */
