/*
 * sortIdx.c
 *
 * Code generation for function 'sortIdx'
 *
 */

/* Include files */
#include "rt_nonfinite.h"
#include "sortIdx.h"

/* Function Declarations */
static void b_merge(int idx_data[], double x_data[], int offset, int np, int nq,
                    int iwork_data[], double xwork_data[]);
static void b_merge_block(int idx_data[], double x_data[], int offset, int n,
  int preSortLevel, int iwork_data[], double xwork_data[]);
static void b_merge_pow2_block(int idx_data[], double x_data[], int offset);
static void merge(int idx_data[], double x_data[], int offset, int np, int nq,
                  int iwork_data[], double xwork_data[]);
static void merge_block(int idx_data[], double x_data[], int offset, int n, int
  preSortLevel, int iwork_data[], double xwork_data[]);
static void merge_pow2_block(int idx_data[], double x_data[], int offset);

/* Function Definitions */
static void b_merge(int idx_data[], double x_data[], int offset, int np, int nq,
                    int iwork_data[], double xwork_data[])
{
  int n;
  int qend;
  int p;
  int iout;
  int exitg1;
  if (nq != 0) {
    n = np + nq;
    for (qend = 0; qend + 1 <= n; qend++) {
      iwork_data[qend] = idx_data[offset + qend];
      xwork_data[qend] = x_data[offset + qend];
    }

    p = 0;
    n = np;
    qend = np + nq;
    iout = offset - 1;
    do {
      exitg1 = 0;
      iout++;
      if (xwork_data[p] <= xwork_data[n]) {
        idx_data[iout] = iwork_data[p];
        x_data[iout] = xwork_data[p];
        if (p + 1 < np) {
          p++;
        } else {
          exitg1 = 1;
        }
      } else {
        idx_data[iout] = iwork_data[n];
        x_data[iout] = xwork_data[n];
        if (n + 1 < qend) {
          n++;
        } else {
          n = (iout - p) + 1;
          while (p + 1 <= np) {
            idx_data[n + p] = iwork_data[p];
            x_data[n + p] = xwork_data[p];
            p++;
          }

          exitg1 = 1;
        }
      }
    } while (exitg1 == 0);
  }
}

static void b_merge_block(int idx_data[], double x_data[], int offset, int n,
  int preSortLevel, int iwork_data[], double xwork_data[])
{
  int nPairs;
  int bLen;
  int tailOffset;
  int nTail;
  nPairs = n >> preSortLevel;
  bLen = 1 << preSortLevel;
  while (nPairs > 1) {
    if ((nPairs & 1) != 0) {
      nPairs--;
      tailOffset = bLen * nPairs;
      nTail = n - tailOffset;
      if (nTail > bLen) {
        b_merge(idx_data, x_data, offset + tailOffset, bLen, nTail - bLen,
                iwork_data, xwork_data);
      }
    }

    tailOffset = bLen << 1;
    nPairs >>= 1;
    for (nTail = 1; nTail <= nPairs; nTail++) {
      b_merge(idx_data, x_data, offset + (nTail - 1) * tailOffset, bLen, bLen,
              iwork_data, xwork_data);
    }

    bLen = tailOffset;
  }

  if (n > bLen) {
    b_merge(idx_data, x_data, offset, bLen, n - bLen, iwork_data, xwork_data);
  }
}

static void b_merge_pow2_block(int idx_data[], double x_data[], int offset)
{
  int iwork[256];
  double xwork[256];
  int b;
  int bLen;
  int bLen2;
  int nPairs;
  int k;
  int blockOffset;
  int q;
  int p;
  int exitg1;
  for (b = 0; b < 6; b++) {
    bLen = 1 << (b + 2);
    bLen2 = bLen << 1;
    nPairs = 256 >> (b + 3);
    for (k = 1; k <= nPairs; k++) {
      blockOffset = (offset + (k - 1) * bLen2) - 1;
      for (q = 1; q <= bLen2; q++) {
        iwork[q - 1] = idx_data[blockOffset + q];
        xwork[q - 1] = x_data[blockOffset + q];
      }

      p = 0;
      q = bLen;
      do {
        exitg1 = 0;
        blockOffset++;
        if (xwork[p] <= xwork[q]) {
          idx_data[blockOffset] = iwork[p];
          x_data[blockOffset] = xwork[p];
          if (p + 1 < bLen) {
            p++;
          } else {
            exitg1 = 1;
          }
        } else {
          idx_data[blockOffset] = iwork[q];
          x_data[blockOffset] = xwork[q];
          if (q + 1 < bLen2) {
            q++;
          } else {
            q = blockOffset - p;
            while (p + 1 <= bLen) {
              idx_data[(q + p) + 1] = iwork[p];
              x_data[(q + p) + 1] = xwork[p];
              p++;
            }

            exitg1 = 1;
          }
        }
      } while (exitg1 == 0);
    }
  }
}

static void merge(int idx_data[], double x_data[], int offset, int np, int nq,
                  int iwork_data[], double xwork_data[])
{
  int n;
  int qend;
  int p;
  int iout;
  int exitg1;
  if (nq != 0) {
    n = np + nq;
    for (qend = 0; qend + 1 <= n; qend++) {
      iwork_data[qend] = idx_data[offset + qend];
      xwork_data[qend] = x_data[offset + qend];
    }

    p = 0;
    n = np;
    qend = np + nq;
    iout = offset - 1;
    do {
      exitg1 = 0;
      iout++;
      if (xwork_data[p] >= xwork_data[n]) {
        idx_data[iout] = iwork_data[p];
        x_data[iout] = xwork_data[p];
        if (p + 1 < np) {
          p++;
        } else {
          exitg1 = 1;
        }
      } else {
        idx_data[iout] = iwork_data[n];
        x_data[iout] = xwork_data[n];
        if (n + 1 < qend) {
          n++;
        } else {
          n = (iout - p) + 1;
          while (p + 1 <= np) {
            idx_data[n + p] = iwork_data[p];
            x_data[n + p] = xwork_data[p];
            p++;
          }

          exitg1 = 1;
        }
      }
    } while (exitg1 == 0);
  }
}

static void merge_block(int idx_data[], double x_data[], int offset, int n, int
  preSortLevel, int iwork_data[], double xwork_data[])
{
  int nPairs;
  int bLen;
  int tailOffset;
  int nTail;
  nPairs = n >> preSortLevel;
  bLen = 1 << preSortLevel;
  while (nPairs > 1) {
    if ((nPairs & 1) != 0) {
      nPairs--;
      tailOffset = bLen * nPairs;
      nTail = n - tailOffset;
      if (nTail > bLen) {
        merge(idx_data, x_data, offset + tailOffset, bLen, nTail - bLen,
              iwork_data, xwork_data);
      }
    }

    tailOffset = bLen << 1;
    nPairs >>= 1;
    for (nTail = 1; nTail <= nPairs; nTail++) {
      merge(idx_data, x_data, offset + (nTail - 1) * tailOffset, bLen, bLen,
            iwork_data, xwork_data);
    }

    bLen = tailOffset;
  }

  if (n > bLen) {
    merge(idx_data, x_data, offset, bLen, n - bLen, iwork_data, xwork_data);
  }
}

static void merge_pow2_block(int idx_data[], double x_data[], int offset)
{
  int iwork[256];
  double xwork[256];
  int b;
  int bLen;
  int bLen2;
  int nPairs;
  int k;
  int blockOffset;
  int q;
  int p;
  int exitg1;
  for (b = 0; b < 6; b++) {
    bLen = 1 << (b + 2);
    bLen2 = bLen << 1;
    nPairs = 256 >> (b + 3);
    for (k = 1; k <= nPairs; k++) {
      blockOffset = (offset + (k - 1) * bLen2) - 1;
      for (q = 1; q <= bLen2; q++) {
        iwork[q - 1] = idx_data[blockOffset + q];
        xwork[q - 1] = x_data[blockOffset + q];
      }

      p = 0;
      q = bLen;
      do {
        exitg1 = 0;
        blockOffset++;
        if (xwork[p] >= xwork[q]) {
          idx_data[blockOffset] = iwork[p];
          x_data[blockOffset] = xwork[p];
          if (p + 1 < bLen) {
            p++;
          } else {
            exitg1 = 1;
          }
        } else {
          idx_data[blockOffset] = iwork[q];
          x_data[blockOffset] = xwork[q];
          if (q + 1 < bLen2) {
            q++;
          } else {
            q = blockOffset - p;
            while (p + 1 <= bLen) {
              idx_data[(q + p) + 1] = iwork[p];
              x_data[(q + p) + 1] = xwork[p];
              p++;
            }

            exitg1 = 1;
          }
        }
      } while (exitg1 == 0);
    }
  }
}

void b_sortIdx(double x_data[], int x_size[1], int idx_data[], int idx_size[1])
{
  short unnamed_idx_0;
  int x_size_idx_0;
  int i;
  int ib;
  static double b_x_data[4464];
  int n;
  double x4[4];
  short idx4[4];
  int iwork_data[4464];
  int nNaNs;
  static double xwork_data[4464];
  int k;
  int wOffset;
  signed char perm[4];
  int i3;
  int i4;
  unnamed_idx_0 = (short)x_size[0];
  x_size_idx_0 = x_size[0];
  i = x_size[0];
  for (ib = 0; ib < i; ib++) {
    b_x_data[ib] = x_data[ib];
  }

  idx_size[0] = unnamed_idx_0;
  i = unnamed_idx_0;
  for (ib = 0; ib < i; ib++) {
    idx_data[ib] = 0;
  }

  n = x_size[0];
  for (i = 0; i < 4; i++) {
    x4[i] = 0.0;
    idx4[i] = 0;
  }

  i = unnamed_idx_0;
  for (ib = 0; ib < i; ib++) {
    iwork_data[ib] = 0;
  }

  i = (short)x_size[0];
  for (ib = 0; ib < i; ib++) {
    xwork_data[ib] = 0.0;
  }

  nNaNs = 1;
  ib = 0;
  for (k = 0; k + 1 <= n; k++) {
    if (rtIsNaN(b_x_data[k])) {
      idx_data[n - nNaNs] = k + 1;
      xwork_data[n - nNaNs] = b_x_data[k];
      nNaNs++;
    } else {
      ib++;
      idx4[ib - 1] = (short)(k + 1);
      x4[ib - 1] = b_x_data[k];
      if (ib == 4) {
        i = k - nNaNs;
        if (x4[0] <= x4[1]) {
          ib = 1;
          wOffset = 2;
        } else {
          ib = 2;
          wOffset = 1;
        }

        if (x4[2] <= x4[3]) {
          i3 = 3;
          i4 = 4;
        } else {
          i3 = 4;
          i4 = 3;
        }

        if (x4[ib - 1] <= x4[i3 - 1]) {
          if (x4[wOffset - 1] <= x4[i3 - 1]) {
            perm[0] = (signed char)ib;
            perm[1] = (signed char)wOffset;
            perm[2] = (signed char)i3;
            perm[3] = (signed char)i4;
          } else if (x4[wOffset - 1] <= x4[i4 - 1]) {
            perm[0] = (signed char)ib;
            perm[1] = (signed char)i3;
            perm[2] = (signed char)wOffset;
            perm[3] = (signed char)i4;
          } else {
            perm[0] = (signed char)ib;
            perm[1] = (signed char)i3;
            perm[2] = (signed char)i4;
            perm[3] = (signed char)wOffset;
          }
        } else if (x4[ib - 1] <= x4[i4 - 1]) {
          if (x4[wOffset - 1] <= x4[i4 - 1]) {
            perm[0] = (signed char)i3;
            perm[1] = (signed char)ib;
            perm[2] = (signed char)wOffset;
            perm[3] = (signed char)i4;
          } else {
            perm[0] = (signed char)i3;
            perm[1] = (signed char)ib;
            perm[2] = (signed char)i4;
            perm[3] = (signed char)wOffset;
          }
        } else {
          perm[0] = (signed char)i3;
          perm[1] = (signed char)i4;
          perm[2] = (signed char)ib;
          perm[3] = (signed char)wOffset;
        }

        idx_data[i - 2] = idx4[perm[0] - 1];
        idx_data[i - 1] = idx4[perm[1] - 1];
        idx_data[i] = idx4[perm[2] - 1];
        idx_data[i + 1] = idx4[perm[3] - 1];
        b_x_data[i - 2] = x4[perm[0] - 1];
        b_x_data[i - 1] = x4[perm[1] - 1];
        b_x_data[i] = x4[perm[2] - 1];
        b_x_data[i + 1] = x4[perm[3] - 1];
        ib = 0;
      }
    }
  }

  wOffset = x_size[0] - nNaNs;
  if (ib > 0) {
    for (i = 0; i < 4; i++) {
      perm[i] = 0;
    }

    if (ib == 1) {
      perm[0] = 1;
    } else if (ib == 2) {
      if (x4[0] <= x4[1]) {
        perm[0] = 1;
        perm[1] = 2;
      } else {
        perm[0] = 2;
        perm[1] = 1;
      }
    } else if (x4[0] <= x4[1]) {
      if (x4[1] <= x4[2]) {
        perm[0] = 1;
        perm[1] = 2;
        perm[2] = 3;
      } else if (x4[0] <= x4[2]) {
        perm[0] = 1;
        perm[1] = 3;
        perm[2] = 2;
      } else {
        perm[0] = 3;
        perm[1] = 1;
        perm[2] = 2;
      }
    } else if (x4[0] <= x4[2]) {
      perm[0] = 2;
      perm[1] = 1;
      perm[2] = 3;
    } else if (x4[1] <= x4[2]) {
      perm[0] = 2;
      perm[1] = 3;
      perm[2] = 1;
    } else {
      perm[0] = 3;
      perm[1] = 2;
      perm[2] = 1;
    }

    for (k = 1; k <= ib; k++) {
      idx_data[(wOffset - ib) + k] = idx4[perm[k - 1] - 1];
      b_x_data[(wOffset - ib) + k] = x4[perm[k - 1] - 1];
    }
  }

  i = (nNaNs - 1) >> 1;
  for (k = 1; k <= i; k++) {
    ib = idx_data[wOffset + k];
    idx_data[wOffset + k] = idx_data[n - k];
    idx_data[n - k] = ib;
    b_x_data[wOffset + k] = xwork_data[n - k];
    b_x_data[n - k] = xwork_data[wOffset + k];
  }

  if (((nNaNs - 1) & 1) != 0) {
    b_x_data[(wOffset + i) + 1] = xwork_data[(wOffset + i) + 1];
  }

  wOffset = (x_size[0] - nNaNs) + 1;
  i = 2;
  if (wOffset > 1) {
    if (x_size[0] >= 256) {
      ib = wOffset >> 8;
      if (ib > 0) {
        for (i = 1; i <= ib; i++) {
          b_merge_pow2_block(idx_data, b_x_data, (i - 1) << 8);
        }

        i = ib << 8;
        ib = wOffset - i;
        if (ib > 0) {
          b_merge_block(idx_data, b_x_data, i, ib, 2, iwork_data, xwork_data);
        }

        i = 8;
      }
    }

    b_merge_block(idx_data, b_x_data, 0, wOffset, i, iwork_data, xwork_data);
  }

  x_size[0] = x_size_idx_0;
  for (ib = 0; ib < x_size_idx_0; ib++) {
    x_data[ib] = b_x_data[ib];
  }
}

void sortIdx(double x_data[], int x_size[1], int idx_data[], int idx_size[1])
{
  short unnamed_idx_0;
  int x_size_idx_0;
  int i;
  int ib;
  static double b_x_data[4464];
  int n;
  double x4[4];
  short idx4[4];
  int iwork_data[4464];
  int nNaNs;
  static double xwork_data[4464];
  int k;
  int wOffset;
  signed char perm[4];
  int i3;
  int i4;
  unnamed_idx_0 = (short)x_size[0];
  x_size_idx_0 = x_size[0];
  i = x_size[0];
  for (ib = 0; ib < i; ib++) {
    b_x_data[ib] = x_data[ib];
  }

  idx_size[0] = unnamed_idx_0;
  i = unnamed_idx_0;
  for (ib = 0; ib < i; ib++) {
    idx_data[ib] = 0;
  }

  n = x_size[0];
  for (i = 0; i < 4; i++) {
    x4[i] = 0.0;
    idx4[i] = 0;
  }

  i = unnamed_idx_0;
  for (ib = 0; ib < i; ib++) {
    iwork_data[ib] = 0;
  }

  i = (short)x_size[0];
  for (ib = 0; ib < i; ib++) {
    xwork_data[ib] = 0.0;
  }

  nNaNs = 0;
  ib = 0;
  for (k = 0; k + 1 <= n; k++) {
    if (rtIsNaN(b_x_data[k])) {
      idx_data[(n - nNaNs) - 1] = k + 1;
      xwork_data[(n - nNaNs) - 1] = b_x_data[k];
      nNaNs++;
    } else {
      ib++;
      idx4[ib - 1] = (short)(k + 1);
      x4[ib - 1] = b_x_data[k];
      if (ib == 4) {
        i = k - nNaNs;
        if (x4[0] >= x4[1]) {
          ib = 1;
          wOffset = 2;
        } else {
          ib = 2;
          wOffset = 1;
        }

        if (x4[2] >= x4[3]) {
          i3 = 3;
          i4 = 4;
        } else {
          i3 = 4;
          i4 = 3;
        }

        if (x4[ib - 1] >= x4[i3 - 1]) {
          if (x4[wOffset - 1] >= x4[i3 - 1]) {
            perm[0] = (signed char)ib;
            perm[1] = (signed char)wOffset;
            perm[2] = (signed char)i3;
            perm[3] = (signed char)i4;
          } else if (x4[wOffset - 1] >= x4[i4 - 1]) {
            perm[0] = (signed char)ib;
            perm[1] = (signed char)i3;
            perm[2] = (signed char)wOffset;
            perm[3] = (signed char)i4;
          } else {
            perm[0] = (signed char)ib;
            perm[1] = (signed char)i3;
            perm[2] = (signed char)i4;
            perm[3] = (signed char)wOffset;
          }
        } else if (x4[ib - 1] >= x4[i4 - 1]) {
          if (x4[wOffset - 1] >= x4[i4 - 1]) {
            perm[0] = (signed char)i3;
            perm[1] = (signed char)ib;
            perm[2] = (signed char)wOffset;
            perm[3] = (signed char)i4;
          } else {
            perm[0] = (signed char)i3;
            perm[1] = (signed char)ib;
            perm[2] = (signed char)i4;
            perm[3] = (signed char)wOffset;
          }
        } else {
          perm[0] = (signed char)i3;
          perm[1] = (signed char)i4;
          perm[2] = (signed char)ib;
          perm[3] = (signed char)wOffset;
        }

        idx_data[i - 3] = idx4[perm[0] - 1];
        idx_data[i - 2] = idx4[perm[1] - 1];
        idx_data[i - 1] = idx4[perm[2] - 1];
        idx_data[i] = idx4[perm[3] - 1];
        b_x_data[i - 3] = x4[perm[0] - 1];
        b_x_data[i - 2] = x4[perm[1] - 1];
        b_x_data[i - 1] = x4[perm[2] - 1];
        b_x_data[i] = x4[perm[3] - 1];
        ib = 0;
      }
    }
  }

  wOffset = (x_size[0] - nNaNs) - 1;
  if (ib > 0) {
    for (i = 0; i < 4; i++) {
      perm[i] = 0;
    }

    if (ib == 1) {
      perm[0] = 1;
    } else if (ib == 2) {
      if (x4[0] >= x4[1]) {
        perm[0] = 1;
        perm[1] = 2;
      } else {
        perm[0] = 2;
        perm[1] = 1;
      }
    } else if (x4[0] >= x4[1]) {
      if (x4[1] >= x4[2]) {
        perm[0] = 1;
        perm[1] = 2;
        perm[2] = 3;
      } else if (x4[0] >= x4[2]) {
        perm[0] = 1;
        perm[1] = 3;
        perm[2] = 2;
      } else {
        perm[0] = 3;
        perm[1] = 1;
        perm[2] = 2;
      }
    } else if (x4[0] >= x4[2]) {
      perm[0] = 2;
      perm[1] = 1;
      perm[2] = 3;
    } else if (x4[1] >= x4[2]) {
      perm[0] = 2;
      perm[1] = 3;
      perm[2] = 1;
    } else {
      perm[0] = 3;
      perm[1] = 2;
      perm[2] = 1;
    }

    for (k = 1; k <= ib; k++) {
      idx_data[(wOffset - ib) + k] = idx4[perm[k - 1] - 1];
      b_x_data[(wOffset - ib) + k] = x4[perm[k - 1] - 1];
    }
  }

  i = nNaNs >> 1;
  for (k = 1; k <= i; k++) {
    ib = idx_data[wOffset + k];
    idx_data[wOffset + k] = idx_data[n - k];
    idx_data[n - k] = ib;
    b_x_data[wOffset + k] = xwork_data[n - k];
    b_x_data[n - k] = xwork_data[wOffset + k];
  }

  if ((nNaNs & 1) != 0) {
    b_x_data[(wOffset + i) + 1] = xwork_data[(wOffset + i) + 1];
  }

  wOffset = x_size[0] - nNaNs;
  i = 2;
  if (wOffset > 1) {
    if (x_size[0] >= 256) {
      ib = wOffset >> 8;
      if (ib > 0) {
        for (i = 1; i <= ib; i++) {
          merge_pow2_block(idx_data, b_x_data, (i - 1) << 8);
        }

        i = ib << 8;
        ib = wOffset - i;
        if (ib > 0) {
          merge_block(idx_data, b_x_data, i, ib, 2, iwork_data, xwork_data);
        }

        i = 8;
      }
    }

    merge_block(idx_data, b_x_data, 0, wOffset, i, iwork_data, xwork_data);
  }

  if ((nNaNs > 0) && (wOffset > 0)) {
    for (k = 0; k + 1 <= nNaNs; k++) {
      xwork_data[k] = b_x_data[wOffset + k];
      iwork_data[k] = idx_data[wOffset + k];
    }

    for (k = wOffset - 1; k + 1 > 0; k--) {
      b_x_data[nNaNs + k] = b_x_data[k];
      idx_data[nNaNs + k] = idx_data[k];
    }

    for (k = 0; k + 1 <= nNaNs; k++) {
      b_x_data[k] = xwork_data[k];
      idx_data[k] = iwork_data[k];
    }
  }

  x_size[0] = x_size_idx_0;
  for (ib = 0; ib < x_size_idx_0; ib++) {
    x_data[ib] = b_x_data[ib];
  }
}

/* End of code generation (sortIdx.c) */
