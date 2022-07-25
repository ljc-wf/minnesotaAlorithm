/*
 * filtfilt.h
 *
 * Code generation for function 'filtfilt'
 *
 */

#ifndef FILTFILT_H
#define FILTFILT_H

/* Include files */
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rt_nonfinite.h"
#include "rtwtypes.h"
#include "dll1_types.h"

/* Function Declarations */
extern void filtfilt(const double x_in_data[], const int x_in_size[1],
                     emxArray_real_T *y_out);

 //void b_filtfilt(const double x_in[], double y_out[]);
extern void b_filtfilt(const double x_in[], double y_out[]);
extern void filtfilt2(const double x_in[], double y_out[]);
extern void filterECG128Hz(double data[], double filtdata[]);

#endif

/* End of code generation (filtfilt.h) */
