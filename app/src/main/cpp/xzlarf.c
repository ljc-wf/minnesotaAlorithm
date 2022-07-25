/*
 * xzlarf.c
 *
 * Code generation for function 'xzlarf'
 *
 */

/* Include files */
#include "rt_nonfinite.h"
#include "dll1.h"
#include "xzlarf.h"
#include "xgerc.h"
#include "xgemv.h"

/* Function Declarations */
static int ilazlc(int m, int n, const emxArray_real_T *A, int ia0, int lda);

/* Function Definitions */
static int ilazlc(int m, int n, const emxArray_real_T *A, int ia0, int lda)
{
  int j;
  boolean_T exitg2;
  int coltop;
  int ia;
  int exitg1;
  j = n;
  exitg2 = false;
  while ((!exitg2) && (j > 0)) {
    coltop = ia0 + (j - 1) * lda;
    ia = coltop;
    do {
      exitg1 = 0;
      if (ia <= (coltop + m) - 1) {
        if (A->data[ia - 1] != 0.0) {
          exitg1 = 1;
        } else {
          ia++;
        }
      } else {
        j--;
        exitg1 = 2;
      }
    } while (exitg1 == 0);

    if (exitg1 == 1) {
      exitg2 = true;
    }
  }

  return j;
}

void xzlarf(int m, int n, int iv0, double tau, emxArray_real_T *C, int ic0, int
            ldc, double work[7])
{
  int lastv;
  int lastc;
  if (tau != 0.0) {
    lastv = m;
    lastc = iv0 + m;
    while ((lastv > 0) && (C->data[lastc - 2] == 0.0)) {
      lastv--;
      lastc--;
    }

    lastc = ilazlc(lastv, n, C, ic0, ldc);
  } else {
    lastv = 0;
    lastc = 0;
  }

  if (lastv > 0) {
    xgemv(lastv, lastc, C, ic0, ldc, C, iv0, work);
    xgerc(lastv, lastc, -tau, iv0, work, C, ic0, ldc);
  }
}

/* End of code generation (xzlarf.c) */
