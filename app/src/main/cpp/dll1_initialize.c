/*
 * dll1_initialize.c
 *
 * Code generation for function 'dll1_initialize'
 *
 */

/* Include files */
#include "rt_nonfinite.h"
#include "dll1_initialize.h"
#include "pan_tompkin.h"

/* Function Definitions */
void dll1_initialize(void)
{
  rt_InitInfAndNaN(8U);
  pan_tompkin_init();
}

/* End of code generation (dll1_initialize.c) */
