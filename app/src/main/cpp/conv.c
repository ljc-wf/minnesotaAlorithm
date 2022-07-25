/*
 * conv.c
 *
 * Code generation for function 'conv'
 *
 */

/* Include files */
#include "conv.h"

/* Function Definitions */
void b_conv(const double A_data[], const int A_size[1], double C_data[], int
C_size[1]) {
    int nA;
    int jC;
    int jA2;
    double s;
    int k;
    nA = A_size[0];
    C_size[0] = (short) (A_size[0] + 22);
    for (jC = 1; jC <= A_size[0] + 22; jC++) {
        if (nA <= jC) {
            jA2 = nA;
        } else {
            jA2 = jC;
        }

        s = 0.0;
        if (23 < jC + 1) {
            k = jC - 23;
        } else {
            k = 0;
        }

        while (k + 1 <= jA2) {
            s += A_data[k] * 0.043478260869565216;
            k++;
        }

        C_data[jC - 1] = s;
    }
}

void conv(const double A_data[], const int A_size[1], double C_data[], int
C_size[1]) {
    int nA;
    int nC;
    int jC;
    int jA2;
    double s;
    int k;
    static const double dv2[5] = {-0.125, -0.25, 0.0, 0.25, 0.125};

    nA = A_size[0];
    if (A_size[0] == 0) {
        nC = 5;
    } else {
        nC = A_size[0] + 4;
    }

    C_size[0] = (short) nC;
    if (A_size[0] == 0) {
        jC = (short) nC;
        C_size[0] = (short) nC;
        for (nA = 0; nA < jC; nA++) {
            C_data[nA] = 0.0;
        }
    } else {
        for (jC = 1; jC <= nC; jC++) {
            if (nA <= jC) {
                jA2 = nA;
            } else {
                jA2 = jC;
            }

            s = 0.0;
            if (5 < jC + 1) {
                k = jC - 4;
            } else {
                k = 1;
            }

            while (k <= jA2) {
                s += A_data[k - 1] * dv2[jC - k];
                k++;
            }

            C_data[jC - 1] = s;
        }
    }
}

/* End of code generation (conv.c) */
