/*
 * dll1_emxutil.h
 *
 * Code generation for function 'dll1_emxutil'
 *
 */

#ifndef DLL1_EMXUTIL_H
#define DLL1_EMXUTIL_H

/* Include files */
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rt_nonfinite.h"
#include "rtwtypes.h"
#include "dll1_types.h"

/* Function Declarations */
extern void emxEnsureCapacity(emxArray__common *emxArray, int oldNumel, int
  elementSize);
extern void emxFree_real_T(emxArray_real_T **pEmxArray);
extern void emxInit_real_T(emxArray_real_T **pEmxArray, int numDimensions);

#endif

/* End of code generation (dll1_emxutil.h) */
