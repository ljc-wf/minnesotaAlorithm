/*
 * diff.c
 *
 * Code generation for function 'diff'
 *
 */

/* Include files */
#include "dll1.h"
#include "diff.h"
#include "dll1_emxutil.h"

/* Function Definitions */
void b_diff(const emxArray_real_T *x, emxArray_real_T *y)
{
  int iyLead;
  int orderForDim;
  double work_data_idx_0;
  int m;
  double tmp1;
  double tmp2;
  if (x->size[1] == 0) {
    iyLead = y->size[0] * y->size[1];
    y->size[0] = 1;
    y->size[1] = 0;
    emxEnsureCapacity((emxArray__common *)y, iyLead, (int)sizeof(double));
  } else {
    if (x->size[1] - 1 <= 1) {
      orderForDim = x->size[1] - 1;
    } else {
      orderForDim = 1;
    }

    if (orderForDim < 1) {
      iyLead = y->size[0] * y->size[1];
      y->size[0] = 1;
      y->size[1] = 0;
      emxEnsureCapacity((emxArray__common *)y, iyLead, (int)sizeof(double));
    } else {
      orderForDim = x->size[1] - 1;
      iyLead = y->size[0] * y->size[1];
      y->size[0] = 1;
      y->size[1] = orderForDim;
      emxEnsureCapacity((emxArray__common *)y, iyLead, (int)sizeof(double));
      if (!(y->size[1] == 0)) {
        orderForDim = 1;
        iyLead = 0;
        work_data_idx_0 = x->data[0];
        for (m = 2; m <= x->size[1]; m++) {
          tmp1 = x->data[orderForDim];
          tmp2 = work_data_idx_0;
          work_data_idx_0 = tmp1;
          tmp1 -= tmp2;
          orderForDim++;
          y->data[iyLead] = tmp1;
          iyLead++;
        }
      }
    }
  }
}

void diff(const double x_data[], const int x_size[1], double y_data[], int
          y_size[1])
{
  int b_x_size;
  int ixLead;
  int iyLead;
  double work_data_idx_0;
  int m;
  double tmp2;
  if (x_size[0] == 0) {
    y_size[0] = 0;
  } else {
    if (x_size[0] - 1 <= 1) {
      b_x_size = x_size[0] - 1;
    } else {
      b_x_size = 1;
    }

    if (b_x_size < 1) {
      y_size[0] = 0;
    } else {
      y_size[0] = (short)(x_size[0] - 1);
      if (!((short)(x_size[0] - 1) == 0)) {
        ixLead = 1;
        iyLead = 0;
        work_data_idx_0 = x_data[0];
        for (m = 2; m <= x_size[0]; m++) {
          tmp2 = work_data_idx_0;
          work_data_idx_0 = x_data[ixLead];
          tmp2 = x_data[ixLead] - tmp2;
          ixLead++;
          y_data[iyLead] = tmp2;
          iyLead++;
        }
      }
    }
  }
}

/* End of code generation (diff.c) */
