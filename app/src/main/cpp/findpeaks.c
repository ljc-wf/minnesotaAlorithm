/*
 * findpeaks.c
 *
 * Code generation for function 'findpeaks'
 *
 */

/* Include files */
#include "rt_nonfinite.h"
#include "findpeaks.h"
#include "sort1.h"
#include "eml_setop.h"
#include "diff.h"
#include "sign.h"

/* Function Declarations */
static void assignOutputs(const double y_data[], const double x_data[], const
  double iPk_data[], const int iPk_size[1], double YpkOut_data[], int
  YpkOut_size[2], double XpkOut_data[], int XpkOut_size[2]);
static void c_findPeaksSeparatedByMoreThanM(const double y_data[], const double
  x_data[], const double iPk_data[], const int iPk_size[1], double idx_data[],
  int idx_size[1]);
static void findLocalMaxima(double yTemp_data[], int yTemp_size[1], double
  iPk_data[], int iPk_size[1], double iInflect_data[], int iInflect_size[1]);
static void getAllPeaks(const double y_data[], const int y_size[1], double
  iPk_data[], int iPk_size[1], double iInf_data[], int iInf_size[1], double
  iInflect_data[], int iInflect_size[1]);
static void keepAtMostNpPeaks(double idx_data[], int idx_size[1], double Np);
static void parse_inputs(const double Yin_data[], const int Yin_size[1], double
  y_data[], int y_size[1], boolean_T *yIsRow, double x_data[], int x_size[1],
  boolean_T *xIsRow, double *NpOut);
static void removePeaksBelowMinPeakHeight(const double Y_data[], double
  iPk_data[], int iPk_size[1]);
static void removePeaksBelowThreshold(const double Y_data[], double iPk_data[],
  int iPk_size[1]);

/* Function Definitions */
static void assignOutputs(const double y_data[], const double x_data[], const
  double iPk_data[], const int iPk_size[1], double YpkOut_data[], int
  YpkOut_size[2], double XpkOut_data[], int XpkOut_size[2])
{
  int loop_ub;
  int i7;
  YpkOut_size[0] = iPk_size[0];
  YpkOut_size[1] = 1;
  loop_ub = iPk_size[0];
  for (i7 = 0; i7 < loop_ub; i7++) {
    YpkOut_data[i7] = y_data[(int)iPk_data[i7] - 1];
  }

  XpkOut_size[0] = iPk_size[0];
  XpkOut_size[1] = 1;
  loop_ub = iPk_size[0];
  for (i7 = 0; i7 < loop_ub; i7++) {
    XpkOut_data[i7] = x_data[(int)iPk_data[i7] - 1];
  }
}

static void c_findPeaksSeparatedByMoreThanM(const double y_data[], const double
  x_data[], const double iPk_data[], const int iPk_size[1], double idx_data[],
  int idx_size[1])
{
  int loop_ub;
  int i6;
  int locs_temp_size[1];
  static double locs_data[4464];
  static double locs_temp_data[4464];
  int iidx_data[4464];
  int iidx_size[1];
  int sortIdx_size_idx_0;
  int sortIdx_data[4464];
  int i;
  boolean_T idelete_data[4464];
  int trueCount;
  boolean_T tmp_data[4464];
  if (iPk_size[0] == 0) {
    idx_size[0] = 0;
  } else {
    loop_ub = iPk_size[0];
    for (i6 = 0; i6 < loop_ub; i6++) {
      locs_data[i6] = x_data[(int)iPk_data[i6] - 1];
    }

    locs_temp_size[0] = iPk_size[0];
    loop_ub = iPk_size[0];
    for (i6 = 0; i6 < loop_ub; i6++) {
      locs_temp_data[i6] = y_data[(int)iPk_data[i6] - 1];
    }

    sort(locs_temp_data, locs_temp_size, iidx_data, iidx_size);
    sortIdx_size_idx_0 = iidx_size[0];
    loop_ub = iidx_size[0];
    for (i6 = 0; i6 < loop_ub; i6++) {
      sortIdx_data[i6] = iidx_data[i6];
    }

    loop_ub = iidx_size[0];
    for (i6 = 0; i6 < loop_ub; i6++) {
      locs_temp_data[i6] = locs_data[sortIdx_data[i6] - 1];
    }

    loop_ub = (short)iidx_size[0];
    for (i6 = 0; i6 < loop_ub; i6++) {
      idelete_data[i6] = false;
    }

    for (i = 0; i < sortIdx_size_idx_0; i++) {
      if (!idelete_data[i]) {
        for (i6 = 0; i6 < sortIdx_size_idx_0; i6++) {
          tmp_data[i6] = ((locs_temp_data[i6] >= locs_data[sortIdx_data[i] - 1]
                           - 30.0) && (locs_temp_data[i6] <=
            locs_data[sortIdx_data[i] - 1] + 30.0));
        }

        loop_ub = (short)sortIdx_size_idx_0;
        for (i6 = 0; i6 < loop_ub; i6++) {
          idelete_data[i6] = (idelete_data[i6] || tmp_data[i6]);
        }

        idelete_data[i] = false;
      }
    }

    loop_ub = (short)iidx_size[0] - 1;
    trueCount = 0;
    for (i = 0; i <= loop_ub; i++) {
      if (!idelete_data[i]) {
        trueCount++;
      }
    }

    sortIdx_size_idx_0 = 0;
    for (i = 0; i <= loop_ub; i++) {
      if (!idelete_data[i]) {
        iidx_data[sortIdx_size_idx_0] = i + 1;
        sortIdx_size_idx_0++;
      }
    }

    idx_size[0] = trueCount;
    for (i6 = 0; i6 < trueCount; i6++) {
      idx_data[i6] = sortIdx_data[iidx_data[i6] - 1];
    }

    c_sort(idx_data, idx_size);
  }
}

static void findLocalMaxima(double yTemp_data[], int yTemp_size[1], double
  iPk_data[], int iPk_size[1], double iInflect_data[], int iInflect_size[1])
{
  double tmp_data[2234];
  int nx;
  int loop_ub;
  int idx;
  double y_data[2234];
  double iTemp_data[2234];
  boolean_T yFinite_data[2234];
  boolean_T x_data[2233];
  int ii_data[2233];
  int ii_size_idx_0;
  int ii;
  boolean_T exitg3;
  boolean_T guard3 = false;
  int b_tmp_data[2234];
  int b_yTemp_size[1];
  double s_data[2233];
  int s_size[1];
  double c_tmp_data[2233];
  boolean_T exitg2;
  boolean_T guard2 = false;
  int b_ii_data[2233];
  boolean_T exitg1;
  boolean_T guard1 = false;
  nx = 2 + yTemp_size[0];
  tmp_data[0] = rtNaN;
  loop_ub = yTemp_size[0];
  for (idx = 0; idx < loop_ub; idx++) {
    tmp_data[idx + 1] = yTemp_data[idx];
  }

  tmp_data[1 + yTemp_size[0]] = rtNaN;
  yTemp_size[0] += 2;
  for (idx = 0; idx < nx; idx++) {
    yTemp_data[idx] = tmp_data[idx];
  }

  loop_ub = (int)((double)yTemp_size[0] - 1.0);
  for (idx = 0; idx <= loop_ub; idx++) {
    y_data[idx] = 1.0 + (double)idx;
  }

  loop_ub = (int)((double)nx - 1.0) + 1;
  for (idx = 0; idx < loop_ub; idx++) {
    iTemp_data[idx] = y_data[idx];
  }

  for (idx = 0; idx < nx; idx++) {
    yFinite_data[idx] = rtIsNaN(yTemp_data[idx]);
  }

  loop_ub = yTemp_size[0];
  for (idx = 0; idx < loop_ub; idx++) {
    yFinite_data[idx] = !yFinite_data[idx];
  }

  loop_ub = nx - 2;
  for (idx = 0; idx <= loop_ub; idx++) {
    x_data[idx] = ((yTemp_data[idx] != yTemp_data[1 + idx]) && (yFinite_data[idx]
      || yFinite_data[1 + idx]));
  }

  idx = 0;
  ii_size_idx_0 = loop_ub + 1;
  ii = 1;
  exitg3 = false;
  while ((!exitg3) && (ii <= loop_ub + 1)) {
    guard3 = false;
    if (x_data[ii - 1]) {
      idx++;
      ii_data[idx - 1] = ii;
      if (idx >= loop_ub + 1) {
        exitg3 = true;
      } else {
        guard3 = true;
      }
    } else {
      guard3 = true;
    }

    if (guard3) {
      ii++;
    }
  }

  if (loop_ub + 1 == 1) {
    if (idx == 0) {
      ii_size_idx_0 = 0;
    }
  } else if (1 > idx) {
    ii_size_idx_0 = 0;
  } else {
    ii_size_idx_0 = idx;
  }

  b_tmp_data[0] = 1;
  for (idx = 0; idx < ii_size_idx_0; idx++) {
    b_tmp_data[idx + 1] = ii_data[idx] + 1;
  }

  loop_ub = 1 + ii_size_idx_0;
  for (idx = 0; idx < loop_ub; idx++) {
    tmp_data[idx] = iTemp_data[b_tmp_data[idx] - 1];
  }

  loop_ub = 1 + ii_size_idx_0;
  for (idx = 0; idx < loop_ub; idx++) {
    iTemp_data[idx] = tmp_data[idx];
  }

  b_yTemp_size[0] = 1 + ii_size_idx_0;
  loop_ub = 1 + ii_size_idx_0;
  for (idx = 0; idx < loop_ub; idx++) {
    tmp_data[idx] = yTemp_data[(int)iTemp_data[idx] - 1];
  }

  diff(tmp_data, b_yTemp_size, s_data, s_size);
  b_sign(s_data, s_size);
  diff(s_data, s_size, c_tmp_data, b_yTemp_size);
  loop_ub = b_yTemp_size[0];
  for (idx = 0; idx < loop_ub; idx++) {
    x_data[idx] = (c_tmp_data[idx] < 0.0);
  }

  nx = b_yTemp_size[0];
  idx = 0;
  ii_size_idx_0 = b_yTemp_size[0];
  ii = 1;
  exitg2 = false;
  while ((!exitg2) && (ii <= nx)) {
    guard2 = false;
    if (x_data[ii - 1]) {
      idx++;
      ii_data[idx - 1] = ii;
      if (idx >= nx) {
        exitg2 = true;
      } else {
        guard2 = true;
      }
    } else {
      guard2 = true;
    }

    if (guard2) {
      ii++;
    }
  }

  if (b_yTemp_size[0] == 1) {
    if (idx == 0) {
      ii_size_idx_0 = 0;
    }
  } else if (1 > idx) {
    ii_size_idx_0 = 0;
  } else {
    ii_size_idx_0 = idx;
  }

  if (1 > s_size[0] - 1) {
    loop_ub = 0;
  } else {
    loop_ub = s_size[0] - 1;
  }

  idx = !(2 > s_size[0]);
  for (nx = 0; nx < loop_ub; nx++) {
    x_data[nx] = (s_data[nx] != s_data[idx + nx]);
  }

  idx = 0;
  ii = 1;
  exitg1 = false;
  while ((!exitg1) && (ii <= loop_ub)) {
    guard1 = false;
    if (x_data[ii - 1]) {
      idx++;
      b_ii_data[idx - 1] = ii;
      if (idx >= loop_ub) {
        exitg1 = true;
      } else {
        guard1 = true;
      }
    } else {
      guard1 = true;
    }

    if (guard1) {
      ii++;
    }
  }

  if (loop_ub == 1) {
    if (idx == 0) {
      loop_ub = 0;
    }
  } else if (1 > idx) {
    loop_ub = 0;
  } else {
    loop_ub = idx;
  }

  iInflect_size[0] = loop_ub;
  for (idx = 0; idx < loop_ub; idx++) {
    iInflect_data[idx] = iTemp_data[b_ii_data[idx]] - 1.0;
  }

  iPk_size[0] = ii_size_idx_0;
  for (idx = 0; idx < ii_size_idx_0; idx++) {
    iPk_data[idx] = iTemp_data[ii_data[idx]] - 1.0;
  }
}

static void getAllPeaks(const double y_data[], const int y_size[1], double
  iPk_data[], int iPk_size[1], double iInf_data[], int iInf_size[1], double
  iInflect_data[], int iInflect_size[1])
{
  int nx;
  int idx;
  boolean_T tmp_data[2232];
  boolean_T x_data[2233];
  int ii_data[2233];
  int ii_size_idx_0;
  int ii;
  boolean_T exitg1;
  boolean_T guard1 = false;
  static double yTemp_data[2232];
  short b_tmp_data[2232];
  double b_yTemp_data[2232];
  int yTemp_size[1];
  nx = y_size[0];
  for (idx = 0; idx < nx; idx++) {
    tmp_data[idx] = rtIsInf(y_data[idx]);
  }

  nx = y_size[0];
  for (idx = 0; idx < nx; idx++) {
    x_data[idx] = (tmp_data[idx] && (y_data[idx] > 0.0));
  }

  nx = y_size[0];
  idx = 0;
  ii_size_idx_0 = y_size[0];
  ii = 1;
  exitg1 = false;
  while ((!exitg1) && (ii <= nx)) {
    guard1 = false;
    if (x_data[ii - 1]) {
      idx++;
      ii_data[idx - 1] = ii;
      if (idx >= nx) {
        exitg1 = true;
      } else {
        guard1 = true;
      }
    } else {
      guard1 = true;
    }

    if (guard1) {
      ii++;
    }
  }

  if (y_size[0] == 1) {
    if (idx == 0) {
      ii_size_idx_0 = 0;
    }
  } else if (1 > idx) {
    ii_size_idx_0 = 0;
  } else {
    ii_size_idx_0 = idx;
  }

  iInf_size[0] = ii_size_idx_0;
  for (idx = 0; idx < ii_size_idx_0; idx++) {
    iInf_data[idx] = ii_data[idx];
  }

  nx = y_size[0];
  for (idx = 0; idx < nx; idx++) {
    yTemp_data[idx] = y_data[idx];
  }

  for (idx = 0; idx < ii_size_idx_0; idx++) {
    b_tmp_data[idx] = (short)ii_data[idx];
  }

  for (idx = 0; idx < ii_size_idx_0; idx++) {
    yTemp_data[b_tmp_data[idx] - 1] = rtNaN;
  }

  yTemp_size[0] = y_size[0];
  nx = y_size[0];
  for (idx = 0; idx < nx; idx++) {
    b_yTemp_data[idx] = yTemp_data[idx];
  }

  findLocalMaxima(b_yTemp_data, yTemp_size, iPk_data, iPk_size, iInflect_data,
                  iInflect_size);
}

static void keepAtMostNpPeaks(double idx_data[], int idx_size[1], double Np)
{
  double b_idx_data[4464];
  int loop_ub;
  int i17;
  if (idx_size[0] > Np) {
    loop_ub = (int)Np;
    for (i17 = 0; i17 < loop_ub; i17++) {
      b_idx_data[i17] = idx_data[i17];
    }

    idx_size[0] = (int)Np;
    loop_ub = (int)Np;
    for (i17 = 0; i17 < loop_ub; i17++) {
      idx_data[i17] = b_idx_data[i17];
    }
  }
}

static void parse_inputs(const double Yin_data[], const int Yin_size[1], double
  y_data[], int y_size[1], boolean_T *yIsRow, double x_data[], int x_size[1],
  boolean_T *xIsRow, double *NpOut)
{
  int loop_ub;
  int i5;
  int y_size_idx_1;
  unsigned int b_y_data[2234];
  *yIsRow = false;
  y_size[0] = Yin_size[0];
  loop_ub = Yin_size[0];
  for (i5 = 0; i5 < loop_ub; i5++) {
    y_data[i5] = Yin_data[i5];
  }

  *xIsRow = false;
  if (Yin_size[0] < 1) {
    y_size_idx_1 = 0;
  } else {
    y_size_idx_1 = Yin_size[0];
    loop_ub = Yin_size[0] - 1;
    for (i5 = 0; i5 <= loop_ub; i5++) {
      b_y_data[i5] = (unsigned int)(1 + i5);
    }
  }

  x_size[0] = y_size_idx_1;
  for (i5 = 0; i5 < y_size_idx_1; i5++) {
    x_data[i5] = b_y_data[i5];
  }

  *NpOut = Yin_size[0];
}

static void removePeaksBelowMinPeakHeight(const double Y_data[], double
  iPk_data[], int iPk_size[1])
{
  int end;
  int trueCount;
  int i;
  int partialTrueCount;
  if (!(iPk_size[0] == 0)) {
    end = iPk_size[0] - 1;
    trueCount = 0;
    for (i = 0; i <= end; i++) {
      if (Y_data[(int)iPk_data[i] - 1] > rtMinusInf) {
        trueCount++;
      }
    }

    partialTrueCount = 0;
    for (i = 0; i <= end; i++) {
      if (Y_data[(int)iPk_data[i] - 1] > rtMinusInf) {
        iPk_data[partialTrueCount] = iPk_data[i];
        partialTrueCount++;
      }
    }

    iPk_size[0] = trueCount;
  }
}

static void removePeaksBelowThreshold(const double Y_data[], double iPk_data[],
  int iPk_size[1])
{
  short csz_idx_0;
  double base_data[2232];
  int k;
  int trueCount;
  int i;
  int partialTrueCount;
  csz_idx_0 = (short)iPk_size[0];
  for (k = 0; k + 1 <= csz_idx_0; k++) {
    if ((Y_data[(int)(iPk_data[k] - 1.0) - 1] >= Y_data[(int)(iPk_data[k] + 1.0)
         - 1]) || rtIsNaN(Y_data[(int)(iPk_data[k] + 1.0) - 1])) {
      base_data[k] = Y_data[(int)(iPk_data[k] - 1.0) - 1];
    } else {
      base_data[k] = Y_data[(int)(iPk_data[k] + 1.0) - 1];
    }
  }

  k = iPk_size[0] - 1;
  trueCount = 0;
  for (i = 0; i <= k; i++) {
    if (Y_data[(int)iPk_data[i] - 1] - base_data[i] >= 0.0) {
      trueCount++;
    }
  }

  partialTrueCount = 0;
  for (i = 0; i <= k; i++) {
    if (Y_data[(int)iPk_data[i] - 1] - base_data[i] >= 0.0) {
      iPk_data[partialTrueCount] = iPk_data[i];
      partialTrueCount++;
    }
  }

  iPk_size[0] = trueCount;
}

void findpeaks(const double Yin_data[], const int Yin_size[1], double Ypk_data[],
               int Ypk_size[2], double Xpk_data[], int Xpk_size[2])
{
  static double y_data[2232];
  int y_size[1];
  boolean_T yIsRow;
  static double x_data[2232];
  int x_size[1];
  boolean_T xIsRow;
  double maxN;
  static double iFinite_data[2232];
  static double iInf_data[2232];
  int iInf_size[1];
  static double iInflect_data[2232];
  int iInflect_size[1];
  int iPk_size[1];
  int loop_ub;
  int i4;
  static double iPk_data[4464];
  static double b_iPk_data[4464];
  int ia_data[2232];
  int ib_data[2232];
  static double c_iPk_data[4464];
  int b_iPk_size[1];
  parse_inputs(Yin_data, Yin_size, y_data, y_size, &yIsRow, x_data, x_size,
               &xIsRow, &maxN);
  getAllPeaks(y_data, y_size, iFinite_data, x_size, iInf_data, iInf_size,
              iInflect_data, iInflect_size);
  removePeaksBelowMinPeakHeight(y_data, iFinite_data, x_size);
  iPk_size[0] = x_size[0];
  loop_ub = x_size[0];
  for (i4 = 0; i4 < loop_ub; i4++) {
    iPk_data[i4] = iFinite_data[i4];
  }

  loop_ub = iPk_size[0];
  for (i4 = 0; i4 < loop_ub; i4++) {
    iFinite_data[i4] = iPk_data[i4];
  }

  removePeaksBelowThreshold(y_data, iFinite_data, x_size);
  iPk_size[0] = x_size[0];
  loop_ub = x_size[0];
  for (i4 = 0; i4 < loop_ub; i4++) {
    iPk_data[i4] = iFinite_data[i4];
  }

  do_vectors(iPk_data, iPk_size, iInf_data, iInf_size, b_iPk_data, x_size,
             ia_data, iInflect_size, ib_data, y_size);
  c_findPeaksSeparatedByMoreThanM(y_data, x_data, b_iPk_data, x_size, iPk_data,
    iPk_size);
  keepAtMostNpPeaks(iPk_data, iPk_size, maxN);
  b_iPk_size[0] = iPk_size[0];
  loop_ub = iPk_size[0];
  for (i4 = 0; i4 < loop_ub; i4++) {
    c_iPk_data[i4] = b_iPk_data[(int)iPk_data[i4] - 1];
  }

  assignOutputs(y_data, x_data, c_iPk_data, b_iPk_size, Ypk_data, Ypk_size,
                Xpk_data, Xpk_size);
}

/* End of code generation (findpeaks.c) */
