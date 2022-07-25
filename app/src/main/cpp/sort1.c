/*
 * sort1.c
 *
 * Code generation for function 'sort1'
 *
 */

/* Include files */
#include "sort1.h"
#include "sortIdx.h"
#include "nonSingletonDim.h"

/* Type Definitions */
#ifndef struct_emxArray_int32_T_4464
#define struct_emxArray_int32_T_4464

struct emxArray_int32_T_4464
{
  int data[4464];
  int size[1];
};

#endif                                 /*struct_emxArray_int32_T_4464*/

#ifndef typedef_emxArray_int32_T_4464
#define typedef_emxArray_int32_T_4464

typedef struct emxArray_int32_T_4464 emxArray_int32_T_4464;

#endif                                 /*typedef_emxArray_int32_T_4464*/

/* Function Declarations */
static void b_sort(double x_data[], int x_size[1], int dim, int idx_data[], int
                   idx_size[1]);

/* Function Definitions */
static void b_sort(double x_data[], int x_size[1], int dim, int idx_data[], int
                   idx_size[1])
{
  int i15;
  static double vwork_data[4464];
  int vwork_size[1];
  int vstride;
  int k;
  int j;
  int iidx_data[4464];
  int iidx_size[1];
  if (dim <= 1) {
    i15 = x_size[0];
  } else {
    i15 = 1;
  }

  vwork_size[0] = (short)i15;
  idx_size[0] = (short)x_size[0];
  vstride = 1;
  k = 1;
  while (k <= dim - 1) {
    vstride *= x_size[0];
    k = 2;
  }

  for (j = 0; j + 1 <= vstride; j++) {
    for (k = 0; k + 1 <= i15; k++) {
      vwork_data[k] = x_data[j + k * vstride];
    }

    sortIdx(vwork_data, vwork_size, iidx_data, iidx_size);
    for (k = 0; k + 1 <= i15; k++) {
      x_data[j + k * vstride] = vwork_data[k];
      idx_data[j + k * vstride] = iidx_data[k];
    }
  }
}

void c_sort(double x_data[], int x_size[1])
{
  int dim;
  int i16;
  static double vwork_data[4464];
  int vwork_size[1];
  int vstride;
  int k;
  emxArray_int32_T_4464 b_vwork_data;
  dim = nonSingletonDim(x_size);
  if (dim <= 1) {
    i16 = x_size[0];
  } else {
    i16 = 1;
  }

  vwork_size[0] = (short)i16;
  vstride = 1;
  k = 1;
  while (k <= dim - 1) {
    vstride *= x_size[0];
    k = 2;
  }

  for (dim = 0; dim + 1 <= vstride; dim++) {
    for (k = 0; k + 1 <= i16; k++) {
      vwork_data[k] = x_data[dim + k * vstride];
    }

    b_sortIdx(vwork_data, vwork_size, b_vwork_data.data, b_vwork_data.size);
    for (k = 0; k + 1 <= i16; k++) {
      x_data[dim + k * vstride] = vwork_data[k];
    }
  }
}

void sort(double x_data[], int x_size[1], int idx_data[], int idx_size[1])
{
  int i14;
  i14 = nonSingletonDim(x_size);
  b_sort(x_data, x_size, i14, idx_data, idx_size);
}

/* End of code generation (sort1.c) */
