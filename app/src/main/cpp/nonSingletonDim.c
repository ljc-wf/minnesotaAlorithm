/*
 * nonSingletonDim.c
 *
 * Code generation for function 'nonSingletonDim'
 *
 */

/* Include files */
#include "nonSingletonDim.h"

/* Function Definitions */
int nonSingletonDim(const int x_size[1])
{
  int dim;
  dim = 2;
  if (x_size[0] != 1) {
    dim = 1;
  }

  return dim;
}

/* End of code generation (nonSingletonDim.c) */
