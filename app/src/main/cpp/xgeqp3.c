/*
 * xgeqp3.c
 *
 * Code generation for function 'xgeqp3'
 *
 */

/* Include files */
#include "dll1.h"
#include "xgeqp3.h"
#include "xzlarfg.h"
#include "xnrm2.h"
#include "xzlarf.h"
#include "xswap.h"
#include "ixamax.h"

/* Function Definitions */
void b_xgeqp3(emxArray_real_T *A, double tau_data[], int tau_size[1], int jpvt[7])
{
  int m;
  int mn;
  int k;
  double vn1[7];
  double vn2[7];
  double work[7];
  int j;
  int i;
  double temp1;
  int mmi;
  int itemp;
  double temp2;
  m = A->size[0];
  if (A->size[0] <= 7) {
    mn = A->size[0];
  } else {
    mn = 7;
  }

  tau_size[0] = (short)mn;
  for (k = 0; k < 7; k++) {
    jpvt[k] = 1 + k;
  }

  if (A->size[0] != 0) {
    k = 1;
    for (j = 0; j < 7; j++) {
      work[j] = 0.0;
      temp1 = b_xnrm2(m, A, k);
      vn2[j] = temp1;
      k += m;
      vn1[j] = temp1;
    }

    for (i = 1; i <= mn; i++) {
      k = (i + (i - 1) * m) - 1;
      mmi = m - i;
      j = (i + ixamax(8 - i, vn1, i)) - 2;
      if (j + 1 != i) {
        xswap(m, A, 1 + m * j, 1 + m * (i - 1));
        itemp = jpvt[j];
        jpvt[j] = jpvt[i - 1];
        jpvt[i - 1] = itemp;
        vn1[j] = vn1[i - 1];
        vn2[j] = vn2[i - 1];
      }

      if (i < m) {
        temp1 = A->data[k];
        tau_data[i - 1] = b_xzlarfg(mmi + 1, &temp1, A, k + 2);
        A->data[k] = temp1;
      } else {
        tau_data[i - 1] = 0.0;
      }

      if (i < 7) {
        temp1 = A->data[k];
        A->data[k] = 1.0;
        xzlarf(mmi + 1, 7 - i, k + 1, tau_data[i - 1], A, i + i * m, m, work);
        A->data[k] = temp1;
      }

      for (j = i; j + 1 < 8; j++) {
        if (vn1[j] != 0.0) {
          temp1 = fabs(A->data[(i + A->size[0] * j) - 1]) / vn1[j];
          temp1 = 1.0 - temp1 * temp1;
          if (temp1 < 0.0) {
            temp1 = 0.0;
          }

          temp2 = vn1[j] / vn2[j];
          temp2 = temp1 * (temp2 * temp2);
          if (temp2 <= 1.4901161193847656E-8) {
            if (i < m) {
              vn1[j] = d_xnrm2(mmi, A, (i + m * j) + 1);
              vn2[j] = vn1[j];
            } else {
              vn1[j] = 0.0;
              vn2[j] = 0.0;
            }
          } else {
            vn1[j] *= sqrt(temp1);
          }
        }
      }
    }
  }
}

void xgeqp3(double A_data[], int A_size[1], double tau_data[], int tau_size[1],
            int *jpvt)
{
  int m;
  int mn;
  int i;
  double atmp;
  double d6;
  m = A_size[0];
  if (A_size[0] <= 1) {
    mn = A_size[0];
  } else {
    mn = 1;
  }

  tau_size[0] = (short)mn;
  if (A_size[0] != 0) {
    i = 1;
    while (i <= mn) {
      if (1 < m) {
        atmp = A_data[0];
        d6 = xzlarfg(m, &atmp, A_data);
        tau_data[0] = d6;
        A_data[0] = atmp;
      } else {
        tau_data[0] = 0.0;
      }

      i = 2;
    }
  }

  *jpvt = 1;
}

/* End of code generation (xgeqp3.c) */
