/*
 * eml_setop.c
 *
 * Code generation for function 'eml_setop'
 *
 */

/* Include files */
#include "rt_nonfinite.h"
#include "dll1.h"
#include "eml_setop.h"

/* Function Declarations */
static double skip_to_last_equal_value(int *k, const double x_data[], const int
  x_size[1]);

/* Function Definitions */
static double skip_to_last_equal_value(int *k, const double x_data[], const int
  x_size[1])
{
  double xk;
  boolean_T exitg1;
  double absxk;
  int exponent;
  boolean_T p;
  xk = x_data[*k - 1];
  exitg1 = false;
  while ((!exitg1) && (*k < x_size[0])) {
    absxk = fabs(xk / 2.0);
    if ((!rtIsInf(absxk)) && (!rtIsNaN(absxk))) {
      if (absxk <= 2.2250738585072014E-308) {
        absxk = 4.94065645841247E-324;
      } else {
        frexp(absxk, &exponent);
        absxk = ldexp(1.0, exponent - 53);
      }
    } else {
      absxk = rtNaN;
    }

    if ((fabs(xk - x_data[*k]) < absxk) || (rtIsInf(x_data[*k]) && rtIsInf(xk)))
    {
      p = true;
    } else {
      p = false;
    }

    if (p) {
      (*k)++;
    } else {
      exitg1 = true;
    }
  }

  return xk;
}

void do_vectors(const double a_data[], const int a_size[1], const double b_data[],
                const int b_size[1], double c_data[], int c_size[1], int
                ia_data[], int ia_size[1], int ib_data[], int ib_size[1])
{
  int na;
  int nb;
  int ncmax;
  int nc;
  int nia;
  int nib;
  int iafirst;
  int ialast;
  int ibfirst;
  int iblast;
  int b_ialast;
  double ak;
  int b_iblast;
  double bk;
  double absxk;
  int b_ia_data[2232];
  int exponent;
  boolean_T p;
  double b_c_data[4464];
  na = a_size[0];
  nb = b_size[0];
  ncmax = a_size[0] + b_size[0];
  c_size[0] = (short)ncmax;
  ia_size[0] = a_size[0];
  ib_size[0] = b_size[0];
  nc = -1;
  nia = -1;
  nib = 0;
  iafirst = 1;
  ialast = 1;
  ibfirst = 0;
  iblast = 1;
  while ((ialast <= na) && (iblast <= nb)) {
    b_ialast = ialast;
    ak = skip_to_last_equal_value(&b_ialast, a_data, a_size);
    ialast = b_ialast;
    b_iblast = iblast;
    bk = skip_to_last_equal_value(&b_iblast, b_data, b_size);
    iblast = b_iblast;
    absxk = fabs(bk / 2.0);
    if ((!rtIsInf(absxk)) && (!rtIsNaN(absxk))) {
      if (absxk <= 2.2250738585072014E-308) {
        absxk = 4.94065645841247E-324;
      } else {
        frexp(absxk, &exponent);
        absxk = ldexp(1.0, exponent - 53);
      }
    } else {
      absxk = rtNaN;
    }

    if ((fabs(bk - ak) < absxk) || (rtIsInf(ak) && rtIsInf(bk))) {
      p = true;
    } else {
      p = false;
    }

    if (p) {
      nc++;
      c_data[nc] = ak;
      nia++;
      ia_data[nia] = iafirst;
      ialast = b_ialast + 1;
      iafirst = b_ialast + 1;
      iblast = b_iblast + 1;
      ibfirst = b_iblast;
    } else {
      if ((ak < bk) || rtIsNaN(bk)) {
        p = true;
      } else {
        p = false;
      }

      if (p) {
        nc++;
        nia++;
        c_data[nc] = ak;
        ia_data[nia] = iafirst;
        ialast = b_ialast + 1;
        iafirst = b_ialast + 1;
      } else {
        nc++;
        nib++;
        c_data[nc] = bk;
        ib_data[nib - 1] = ibfirst + 1;
        iblast = b_iblast + 1;
        ibfirst = b_iblast;
      }
    }
  }

  while (ialast <= na) {
    iafirst = ialast;
    ak = skip_to_last_equal_value(&iafirst, a_data, a_size);
    nc++;
    nia++;
    c_data[nc] = ak;
    ia_data[nia] = ialast;
    ialast = iafirst + 1;
  }

  while (iblast <= nb) {
    iafirst = iblast;
    bk = skip_to_last_equal_value(&iafirst, b_data, b_size);
    nc++;
    nib++;
    c_data[nc] = bk;
    ib_data[nib - 1] = iblast;
    iblast = iafirst + 1;
  }

  if (a_size[0] > 0) {
    if (1 > nia + 1) {
      iafirst = -1;
    } else {
      iafirst = nia;
    }

    for (ibfirst = 0; ibfirst <= iafirst; ibfirst++) {
      b_ia_data[ibfirst] = ia_data[ibfirst];
    }

    ia_size[0] = iafirst + 1;
    iafirst++;
    for (ibfirst = 0; ibfirst < iafirst; ibfirst++) {
      ia_data[ibfirst] = b_ia_data[ibfirst];
    }
  }

  if (b_size[0] > 0) {
    if (1 > nib) {
      iafirst = 0;
    } else {
      iafirst = nib;
    }

    for (ibfirst = 0; ibfirst < iafirst; ibfirst++) {
      b_ia_data[ibfirst] = ib_data[ibfirst];
    }

    ib_size[0] = iafirst;
    for (ibfirst = 0; ibfirst < iafirst; ibfirst++) {
      ib_data[ibfirst] = b_ia_data[ibfirst];
    }
  }

  if (ncmax > 0) {
    if (1 > nc + 1) {
      iafirst = -1;
    } else {
      iafirst = nc;
    }

    for (ibfirst = 0; ibfirst <= iafirst; ibfirst++) {
      b_c_data[ibfirst] = c_data[ibfirst];
    }

    c_size[0] = iafirst + 1;
    iafirst++;
    for (ibfirst = 0; ibfirst < iafirst; ibfirst++) {
      c_data[ibfirst] = b_c_data[ibfirst];
    }
  }
}

/* End of code generation (eml_setop.c) */
