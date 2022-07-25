/*
 * pan_tompkin.h
 *
 * Code generation for function 'pan_tompkin'
 *
 */

#ifndef PAN_TOMPKIN_H
#define PAN_TOMPKIN_H

/* Include files */
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rt_nonfinite.h"
#include "rtwtypes.h"
#include "dll1_types.h"

/* Function Declarations */
extern void b_pan_tompkin(const double ecg_data[], const int ecg_size[2],
  emxArray_real_T *qrs_amp_raw, emxArray_real_T *qrs_i_raw, emxArray_real_T
  *NOISL_buf, double locs_data[], int locs_size[2]);
extern void pan_tompkin(double ecg_data[], int ecg_size[1], emxArray_real_T
  *qrs_amp_raw, emxArray_real_T *qrs_i_raw, emxArray_real_T *NOISL_buf, emxArray_real_T *SIG_buf, double
  locs_data[], int locs_size[2]);
extern void pan_tompkin_init(void);

extern double sig_strength[];

#endif

/* End of code generation (pan_tompkin.h) */
