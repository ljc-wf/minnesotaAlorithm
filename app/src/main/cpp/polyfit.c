/*
 * polyfit.c
 *
 * Code generation for function 'polyfit'
 *
 */

/* Include files */
#include "rt_nonfinite.h"
#include "dll1.h"
#include "polyfit.h"
#include "norm.h"
#include "qrsolve.h"
#include "dll1_emxutil.h"
#include "vander.h"
#include "std.h"
#include "mean.h"

/* Function Definitions */
void b_polyfit(const double x_data[], const int x_size[2], const double y_data[],
               double p[7], double S_R_data[], int S_R_size[2], double *S_df,
               double *S_normr, double mu[2])
{
  double d4;
  double d5;
  double b_x_data[2205];
  int b_x_size[2];
  int ar;
  int ib;
  emxArray_real_T *r0;
  int V_size[2];
  double V_data[7];
  emxArray_real_T b_V_data;
  static int iv3[1] = { 1 };

  double p1[7];
  double R_data[49];
  signed char V_idx_0;
  int m;
  double b_y_data[2205];
  int br;
  int y_size[1];
  double c_y_data[1];
  d4 = b_mean(x_data, x_size);
  d5 = b_std(x_data, x_size);
  mu[0] = d4;
  mu[1] = d5;
  b_x_size[0] = 1;
  b_x_size[1] = x_size[1];
  ar = x_size[0] * x_size[1];
  for (ib = 0; ib < ar; ib++) {
    b_x_data[ib] = (x_data[ib] - d4) / d5;
  }

  emxInit_real_T(&r0, 2);
  vander(b_x_data, b_x_size, r0);
  V_size[0] = r0->size[0];
  V_size[1] = 7;
  ar = r0->size[0] * r0->size[1];
  for (ib = 0; ib < ar; ib++) {
    V_data[ib] = r0->data[ib];
  }

  emxFree_real_T(&r0);
  b_V_data.data = (double *)&V_data;
  b_V_data.size = (int *)&V_size;
  b_V_data.allocatedSize = 7;
  b_V_data.numDimensions = 2;
  b_V_data.canFreeData = false;
  b_qrsolve(&b_V_data, y_data, iv3, p1, &ar, R_data, b_x_size);
  V_idx_0 = (signed char)V_size[0];
  m = V_size[0];
  ar = V_idx_0;
  for (ib = 0; ib < ar; ib++) {
    b_y_data[ib] = 0.0;
  }

  if (V_size[0] != 0) {
    ar = 0;
    while ((m > 0) && (ar <= 0)) {
      b_y_data[0] = 0.0;
      ar = 1;
    }

    br = 0;
    ar = 0;
    while ((m > 0) && (ar <= 0)) {
      ar = -1;
      for (ib = br; ib + 1 <= br + 7; ib++) {
        if (p1[ib] != 0.0) {
          b_y_data[0] += p1[ib] * V_data[ar + 1];
        }

        ar++;
      }

      br += 7;
      ar = 1;
    }
  }

  S_R_size[0] = b_x_size[0];
  S_R_size[1] = 7;
  ar = b_x_size[0] * b_x_size[1];
  for (ib = 0; ib < ar; ib++) {
    S_R_data[ib] = R_data[ib];
  }

  *S_df = 0.0;
  y_size[0] = 1;
  c_y_data[0] = y_data[0] - b_y_data[0];
  *S_normr = norm(c_y_data, y_size);
  for (ib = 0; ib < 7; ib++) {
    p[ib] = p1[ib];
  }
}

void polyfit(const double x_data[], const int x_size[2], const double y_data[],
             const int y_size[2], double p[7], double S_R_data[], int S_R_size[2],
             double *S_df, double *S_normr, double mu[2])
{
  double d2;
  double d3;
  double b_x_data[2205];
  int b_x_size[2];
  int ar;
  int ib;
  emxArray_real_T *V;
  int y[1];
  double p1[7];
  double R_data[49];
  short V_idx_0;
  int m;
  double C_data[2205];
  int br;
  int ic;
  int b_y_size[1];
  int ia;
  d2 = b_mean(x_data, x_size);
  d3 = b_std(x_data, x_size);
  mu[0] = d2;
  mu[1] = d3;
  b_x_size[0] = 1;
  b_x_size[1] = x_size[1];
  ar = x_size[0] * x_size[1];
  for (ib = 0; ib < ar; ib++) {
    b_x_data[ib] = (x_data[ib] - d2) / d3;
  }

  emxInit_real_T(&V, 2);
  vander(b_x_data, b_x_size, V);
  y[0] = y_size[1];
  b_qrsolve(V, y_data, y, p1, &ar, R_data, b_x_size);
  V_idx_0 = (short)V->size[0];
  m = V->size[0];
  ar = V_idx_0;
  for (ib = 0; ib < ar; ib++) {
    C_data[ib] = 0.0;
  }

  if (V->size[0] != 0) {
    ar = 0;
    while ((m > 0) && (ar <= 0)) {
      for (ic = 1; ic <= m; ic++) {
        C_data[ic - 1] = 0.0;
      }

      ar = m;
    }

    br = 0;
    ar = 0;
    while ((m > 0) && (ar <= 0)) {
      ar = -1;
      for (ib = br; ib + 1 <= br + 7; ib++) {
        if (p1[ib] != 0.0) {
          ia = ar;
          for (ic = 0; ic + 1 <= m; ic++) {
            ia++;
            C_data[ic] += p1[ib] * V->data[ia];
          }
        }

        ar += m;
      }

      br += 7;
      ar = m;
    }
  }

  emxFree_real_T(&V);
  S_R_size[0] = b_x_size[0];
  S_R_size[1] = 7;
  ar = b_x_size[0] * b_x_size[1];
  for (ib = 0; ib < ar; ib++) {
    S_R_data[ib] = R_data[ib];
  }

  if (0 >= y_size[1] - 7) {
    *S_df = 0.0;
  } else {
    *S_df = y_size[1] - 7;
  }

  b_y_size[0] = y_size[1];
  ar = y_size[1];
  for (ib = 0; ib < ar; ib++) {
    b_x_data[ib] = y_data[ib] - C_data[ib];
  }

  *S_normr = norm(b_x_data, b_y_size);
  for (ib = 0; ib < 7; ib++) {
    p[ib] = p1[ib];
  }
}

/* End of code generation (polyfit.c) */
