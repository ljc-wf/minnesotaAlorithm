/*
 * qrsolve.h
 *
 * Code generation for function 'qrsolve'
 *
 */

#ifndef QRSOLVE_H
#define QRSOLVE_H

/* Include files */
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rt_nonfinite.h"
#include "rtwtypes.h"
#include "dll1_types.h"

/* Function Declarations */
extern void b_qrsolve(const emxArray_real_T *A, const double B_data[], const int
                      B_size[1], double Y[7], int *rankR, double R_data[], int
                      R_size[2]);
extern void qrsolve(const double A_data[], const int A_size[1], const
                    emxArray_real_T *B, double Y_data[], int Y_size[2]);

#endif

/* End of code generation (qrsolve.h) */
