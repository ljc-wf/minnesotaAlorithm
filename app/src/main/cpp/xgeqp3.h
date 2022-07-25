/*
 * xgeqp3.h
 *
 * Code generation for function 'xgeqp3'
 *
 */

#ifndef XGEQP3_H
#define XGEQP3_H

/* Include files */
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rt_nonfinite.h"
#include "rtwtypes.h"
#include "dll1_types.h"

/* Function Declarations */
extern void b_xgeqp3(emxArray_real_T *A, double tau_data[], int tau_size[1], int
                     jpvt[7]);
extern void xgeqp3(double A_data[], int A_size[1], double tau_data[], int
                   tau_size[1], int *jpvt);

#endif

/* End of code generation (xgeqp3.h) */
