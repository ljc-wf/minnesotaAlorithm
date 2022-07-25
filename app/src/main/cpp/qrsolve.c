/*
 * qrsolve.c
 *
 * Code generation for function 'qrsolve'
 *
 */

/* Include files */
#include "dll1.h"
#include "qrsolve.h"
#include "dll1_emxutil.h"
#include "xgeqp3.h"

/* Function Declarations */
static void LSQFromQR(const emxArray_real_T *A, const double tau_data[], const
                      int jpvt[7], double B_data[], int rankA, double Y[7]);
static int b_rankFromQR(const emxArray_real_T *A);
static int rankFromQR(const double A_data[], const int A_size[1]);

/* Function Definitions */
static void LSQFromQR(const emxArray_real_T *A, const double tau_data[], const
                      int jpvt[7], double B_data[], int rankA, double Y[7])
{
  int i;
  int m;
  int mn;
  int j;
  double wj;
  for (i = 0; i < 7; i++) {
    Y[i] = 0.0;
  }

  m = A->size[0];
  mn = A->size[0];
  if (!(mn <= 7)) {
    mn = 7;
  }

  for (j = 0; j + 1 <= mn; j++) {
    if (tau_data[j] != 0.0) {
      wj = B_data[j];
      for (i = j + 1; i + 1 <= m; i++) {
        wj += A->data[i + A->size[0] * j] * B_data[i];
      }

      wj *= tau_data[j];
      if (wj != 0.0) {
        B_data[j] -= wj;
        for (i = j + 1; i + 1 <= m; i++) {
          B_data[i] -= A->data[i + A->size[0] * j] * wj;
        }
      }
    }
  }

  for (i = 0; i + 1 <= rankA; i++) {
    Y[jpvt[i] - 1] = B_data[i];
  }

  for (j = rankA - 1; j + 1 > 0; j--) {
    Y[jpvt[j] - 1] /= A->data[j + A->size[0] * j];
    for (i = 0; i + 1 <= j; i++) {
      Y[jpvt[i] - 1] -= Y[jpvt[j] - 1] * A->data[i + A->size[0] * j];
    }
  }
}

static int b_rankFromQR(const emxArray_real_T *A)
{
  int r;
  int minmn;
  int maxmn;
  double tol;
  r = 0;
  if (A->size[0] < 7) {
    minmn = A->size[0];
    maxmn = 7;
  } else {
    minmn = 7;
    maxmn = A->size[0];
  }

  if (minmn > 0) {
    tol = (double)maxmn * fabs(A->data[0]) * 2.2204460492503131E-16;
    while ((r < minmn) && (fabs(A->data[r + A->size[0] * r]) >= tol)) {
      r++;
    }
  }

  return r;
}

static int rankFromQR(const double A_data[], const int A_size[1])
{
  int r;
  int minmn;
  int maxmn;
  double tol;
  r = 0;
  if (A_size[0] < 1) {
    minmn = 0;
    maxmn = 1;
  } else {
    minmn = 1;
    maxmn = A_size[0];
  }

  if (minmn > 0) {
    tol = (double)maxmn * fabs(A_data[0]) * 2.2204460492503131E-16;
    while ((r < 1) && (fabs(A_data[0]) >= tol)) {
      r = 1;
    }
  }

  return r;
}

void b_qrsolve(const emxArray_real_T *A, const double B_data[], const int
               B_size[1], double Y[7], int *rankR, double R_data[], int R_size[2])
{
  int mn;
  emxArray_real_T *b_A;
  int i9;
  int loop_ub;
  double tau_data[7];
  int tau_size[1];
  int jpvt[7];
  double b_B_data[2205];
  int i;
  mn = A->size[0];
  if (!(mn <= 7)) {
    mn = 7;
  }

  emxInit_real_T(&b_A, 2);
  i9 = b_A->size[0] * b_A->size[1];
  b_A->size[0] = A->size[0];
  b_A->size[1] = 7;
  emxEnsureCapacity((emxArray__common *)b_A, i9, (int)sizeof(double));
  loop_ub = A->size[0] * A->size[1];
  for (i9 = 0; i9 < loop_ub; i9++) {
    b_A->data[i9] = A->data[i9];
  }

  b_xgeqp3(b_A, tau_data, tau_size, jpvt);
  *rankR = b_rankFromQR(b_A);
  loop_ub = B_size[0];
  for (i9 = 0; i9 < loop_ub; i9++) {
    b_B_data[i9] = B_data[i9];
  }

  LSQFromQR(b_A, tau_data, jpvt, b_B_data, mn, Y);
  R_size[0] = (signed char)mn;
  R_size[1] = 7;
  for (loop_ub = 0; loop_ub < 7; loop_ub++) {
    if (loop_ub + 1 <= mn) {
      i9 = loop_ub + 1;
    } else {
      i9 = mn;
    }

    for (i = 0; i + 1 <= i9; i++) {
      R_data[i + (signed char)mn * loop_ub] = b_A->data[i + b_A->size[0] *
        loop_ub];
    }

    for (i = loop_ub + 1; i + 1 <= mn; i++) {
      R_data[i + (signed char)mn * loop_ub] = 0.0;
    }
  }

  emxFree_real_T(&b_A);
}

void qrsolve(const double A_data[], const int A_size[1], const emxArray_real_T
             *B, double Y_data[], int Y_size[2])
{
  int b_A_size[1];
  int loop_ub;
  int m;
  double b_A_data[2205];
  double tau_data[1];
  int tau_size[1];
  int rankA;
  short unnamed_idx_1;
  emxArray_real_T *b_B;
  int mn;
  int k;
  double wj;
  b_A_size[0] = A_size[0];
  loop_ub = A_size[0];
  for (m = 0; m < loop_ub; m++) {
    b_A_data[m] = A_data[m];
  }

  xgeqp3(b_A_data, b_A_size, tau_data, tau_size, &loop_ub);
  rankA = rankFromQR(b_A_data, b_A_size);
  unnamed_idx_1 = (short)B->size[1];
  Y_size[0] = 1;
  Y_size[1] = unnamed_idx_1;
  loop_ub = unnamed_idx_1;
  for (m = 0; m < loop_ub; m++) {
    Y_data[m] = 0.0;
  }

  emxInit_real_T(&b_B, 2);
  m = b_B->size[0] * b_B->size[1];
  b_B->size[0] = B->size[0];
  b_B->size[1] = B->size[1];
  emxEnsureCapacity((emxArray__common *)b_B, m, (int)sizeof(double));
  loop_ub = B->size[0] * B->size[1];
  for (m = 0; m < loop_ub; m++) {
    b_B->data[m] = B->data[m];
  }

  m = b_A_size[0];
  mn = !(b_A_size[0] < 1);
  loop_ub = 1;
  while (loop_ub <= mn) {
    if (tau_data[0] != 0.0) {
      for (k = 0; k + 1 <= B->size[1]; k++) {
        wj = b_B->data[b_B->size[0] * k];
        for (loop_ub = 1; loop_ub + 1 <= m; loop_ub++) {
          wj += b_A_data[loop_ub] * b_B->data[loop_ub + b_B->size[0] * k];
        }

        wj *= tau_data[0];
        if (wj != 0.0) {
          b_B->data[b_B->size[0] * k] -= wj;
          for (loop_ub = 1; loop_ub + 1 <= m; loop_ub++) {
            b_B->data[loop_ub + b_B->size[0] * k] -= b_A_data[loop_ub] * wj;
          }
        }
      }
    }

    loop_ub = 2;
  }

  for (k = 0; k + 1 <= B->size[1]; k++) {
    loop_ub = 1;
    while (loop_ub <= rankA) {
      Y_data[k] = b_B->data[b_B->size[0] * k];
      loop_ub = 2;
    }

    loop_ub = rankA;
    while (loop_ub > 0) {
      Y_data[k] /= b_A_data[0];
      loop_ub = 0;
    }
  }

  emxFree_real_T(&b_B);
}

/* End of code generation (qrsolve.c) */
