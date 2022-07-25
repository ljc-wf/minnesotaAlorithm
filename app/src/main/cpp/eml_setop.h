/*
 * eml_setop.h
 *
 * Code generation for function 'eml_setop'
 *
 */

#ifndef EML_SETOP_H
#define EML_SETOP_H

/* Include files */
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rt_nonfinite.h"
#include "rtwtypes.h"
#include "dll1_types.h"

/* Function Declarations */
extern void do_vectors(const double a_data[], const int a_size[1], const double
  b_data[], const int b_size[1], double c_data[], int c_size[1], int ia_data[],
  int ia_size[1], int ib_data[], int ib_size[1]);

#endif

/* End of code generation (eml_setop.h) */
