/*
 * mrdivide.c
 *
 * Code generation for function 'mrdivide'
 *
 */

/* Include files */
#include "dll1.h"
#include "mrdivide.h"
#include "dll1_emxutil.h"
#include "qrsolve.h"

/* Function Definitions */
void mrdivide(const emxArray_real_T *A, const double B_data[], const int B_size
              [2], double y_data[], int y_size[1])
{
  emxArray_real_T *b_A;
  short unnamed_idx_0;
  double b_B_data[2205];
  int nb;
  int b_B_size[1];
  int loop_ub;
  int i3;
  double temp;
  int b_loop_ub;
  double tmp_data[2205];
  int tmp_size[2];
  emxInit_real_T(&b_A, 2);
  if ((A->size[0] == 0) || (A->size[1] == 0) || (B_size[1] == 0)) {
    unnamed_idx_0 = (short)A->size[0];
    y_size[0] = unnamed_idx_0;
    loop_ub = unnamed_idx_0;
    for (i3 = 0; i3 < loop_ub; i3++) {
      y_data[i3] = 0.0;
    }
  } else if (1 == B_size[1]) {
    nb = A->size[0];
    y_size[0] = A->size[0];
    loop_ub = A->size[0];
    for (i3 = 0; i3 < loop_ub; i3++) {
      y_data[i3] = A->data[i3];
    }

    temp = 1.0 / B_data[0];
    for (b_loop_ub = 0; b_loop_ub + 1 <= nb; b_loop_ub++) {
      y_data[b_loop_ub] *= temp;
    }
  } else {
    b_B_size[0] = B_size[1];
    loop_ub = B_size[1];
    for (i3 = 0; i3 < loop_ub; i3++) {
      b_B_data[i3] = B_data[B_size[0] * i3];
    }

    i3 = b_A->size[0] * b_A->size[1];
    b_A->size[0] = A->size[1];
    b_A->size[1] = A->size[0];
    emxEnsureCapacity((emxArray__common *)b_A, i3, (int)sizeof(double));
    loop_ub = A->size[0];
    for (i3 = 0; i3 < loop_ub; i3++) {
      b_loop_ub = A->size[1];
      for (nb = 0; nb < b_loop_ub; nb++) {
        b_A->data[nb + b_A->size[0] * i3] = A->data[i3 + A->size[0] * nb];
      }
    }

    qrsolve(b_B_data, b_B_size, b_A, tmp_data, tmp_size);
    y_size[0] = tmp_size[1];
    loop_ub = tmp_size[1];
    for (i3 = 0; i3 < loop_ub; i3++) {
      y_data[i3] = tmp_data[tmp_size[0] * i3];
    }
  }

  emxFree_real_T(&b_A);
}

/* End of code generation (mrdivide.c) */
