/*
 * flipud.c
 *
 * Code generation for function 'flipud'
 *
 */

/* Include files */
#include "flipud.h"

/* Function Definitions */
void flipud(double x_data[], int x_size[1])
{
  int m;
  int md2;
  int i;
  double xtmp;
  m = x_size[0];
  md2 = x_size[0] >> 1;
  for (i = 1; i <= md2; i++) {
    xtmp = x_data[i - 1];
    x_data[i - 1] = x_data[m - i];
    x_data[m - i] = xtmp;
  }
}

void b_flipud(double x[3264])
{
  int i;
  double xtmp;
  for (i = 0; i < 1632; i++) {
    xtmp = x[i];
    x[i] = x[3263 - i];
    x[3263 - i] = xtmp;
  }
}

void flipud2(double x[1968])
{
  int i;
  double xtmp;
  for (i = 0; i < 984; i++) {
    xtmp = x[i];
    x[i] = x[1967 - i];
    x[1967 - i] = xtmp;
  }
}

/* End of code generation (flipud.c) */
