/*
 * xgemv.h
 *
 * Code generation for function 'xgemv'
 *
 */

#ifndef XGEMV_H
#define XGEMV_H

/* Include files */
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rt_nonfinite.h"
#include "rtwtypes.h"
#include "dll1_types.h"

/* Function Declarations */
extern void xgemv(int m, int n, const emxArray_real_T *A, int ia0, int lda,
                  const emxArray_real_T *x, int ix0, double y[7]);

#endif

/* End of code generation (xgemv.h) */
