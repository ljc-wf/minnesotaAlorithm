/*
 * pan_tompkin.c
 *
 * Code generation for function 'pan_tompkin'
 *
 */

/* Include files */
#include "rt_nonfinite.h"
#include "dll1.h"
#include "pan_tompkin.h"
#include "dll1_emxutil.h"
#include "mean.h"
#include "findpeaks.h"
#include "conv.h"
#include "power.h"
#include "mrdivide.h"
#include "abs.h"
#include "filtfilt.h"

/* Variable Definitions */
static double y_i;
static double x_i;


/* Function Definitions */
void b_pan_tompkin(const double ecg_data[], const int ecg_size[2],
                   emxArray_real_T *qrs_amp_raw, emxArray_real_T *qrs_i_raw,
                   emxArray_real_T *NOISL_buf, double locs_data[], int
                   locs_size[2]) {
    emxArray_real_T *ecg_h;
    emxArray_real_T *varargin_2;
    int i11;
    int ser_back;
    int ecg[1];
    static double maxval_data[2205];
    int maxval_size[2];
    int n;
    int i;
    int ix;
    static double ecg_h_data[2205];
    int ecg_h_size[1];
    int ixstart;
    int b_n;
    double THR_SIG1;
    static double ecg_d_data[2210];
    boolean_T exitg7;
    boolean_T exitg6;
    static double ecg_s_data[2210];
    int ecg_s_size[1];
    static double ecg_m_data[2232];
    static double pks_data[4464];
    boolean_T exitg5;
    double THR_SIG;
    static const short iv2[300] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
                                   15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
                                   31, 32, 33,
                                   34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
                                   50, 51, 52,
                                   53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68,
                                   69, 70, 71,
                                   72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87,
                                   88, 89, 90,
                                   91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105,
                                   106, 107,
                                   108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120,
                                   121, 122,
                                   123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135,
                                   136, 137,
                                   138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150,
                                   151, 152,
                                   153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165,
                                   166, 167,
                                   168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180,
                                   181, 182,
                                   183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195,
                                   196, 197,
                                   198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210,
                                   211, 212,
                                   213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225,
                                   226, 227,
                                   228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240,
                                   241, 242,
                                   243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255,
                                   256, 257,
                                   258, 259, 260, 261, 262, 263, 264, 265, 266, 267, 268, 269, 270,
                                   271, 272,
                                   273, 274, 275, 276, 277, 278, 279, 280, 281, 282, 283, 284, 285,
                                   286, 287,
                                   288, 289, 290, 291, 292, 293, 294, 295, 296, 297, 298, 299, 300};

    double THR_NOISE;
    double SIG_LEV;
    double NOISE_LEV;
    boolean_T exitg4;
    double SIG_LEV1;
    double NOISE_LEV1;
    int i12;
    static double varargin_1_data[2205];
    int itmp;
    boolean_T exitg2;
    boolean_T exitg3;
    boolean_T exitg1;
    emxInit_real_T(&ecg_h, 2);
    emxInit_real_T(&varargin_2, 2);

    i11 = qrs_i_raw->size[0] * qrs_i_raw->size[1];
    qrs_i_raw->size[0] = 1;
    qrs_i_raw->size[1] = 0;
    emxEnsureCapacity((emxArray__common *) qrs_i_raw, i11, (int) sizeof(double));
    i11 = qrs_amp_raw->size[0] * qrs_amp_raw->size[1];
    qrs_amp_raw->size[0] = 1;
    qrs_amp_raw->size[1] = 0;
    emxEnsureCapacity((emxArray__common *) qrs_amp_raw, i11, (int) sizeof(double));
    ser_back = 0;
    i11 = NOISL_buf->size[0] * NOISL_buf->size[1];
    NOISL_buf->size[0] = 1;
    NOISL_buf->size[1] = 0;
    emxEnsureCapacity((emxArray__common *) NOISL_buf, i11, (int) sizeof(double));

    ecg[0] = ecg_size[1];
    filtfilt(ecg_data, ecg, ecg_h);
    b_abs(ecg_h, varargin_2);
    maxval_size[0] = 1;
    maxval_size[1] = (short) varargin_2->size[1];
    n = varargin_2->size[0];
    for (i = 0; i + 1 <= varargin_2->size[1]; i++) {
        ix = i * n;
        ixstart = i * n + 1;
        b_n = ix + n;
        THR_SIG1 = varargin_2->data[ix];
        if (n > 1) {
            if (rtIsNaN(varargin_2->data[ix])) {
                ix = ixstart;
                exitg7 = false;
                while ((!exitg7) && (ix + 1 <= b_n)) {
                    ixstart = ix + 1;
                    if (!rtIsNaN(varargin_2->data[ix])) {
                        THR_SIG1 = varargin_2->data[ix];
                        exitg7 = true;
                    } else {
                        ix++;
                    }
                }
            }

            if (ixstart < b_n) {
                while (ixstart + 1 <= b_n) {
                    if (varargin_2->data[ixstart] > THR_SIG1) {
                        THR_SIG1 = varargin_2->data[ixstart];
                    }

                    ixstart++;
                }
            }
        }

        maxval_data[i] = THR_SIG1;
    }

    emxFree_real_T(&varargin_2);
    mrdivide(ecg_h, maxval_data, maxval_size, ecg_h_data, ecg_h_size);

    /* ax(3)=subplot(323);plot(ecg_h);axis tight;title('Band Pass Filtered'); */
    /*  derivative filter H(z) = (1/8T)(-z^(-2) - 2z^(-1) + 2z + z^(2)) */
    /* 1/8*fs */
    conv(ecg_h_data, ecg_h_size, ecg_d_data, ecg);
    ixstart = 1;
    n = ecg[0];
    THR_SIG1 = ecg_d_data[0];
    emxFree_real_T(&ecg_h);
    if (ecg[0] > 1) {
        if (rtIsNaN(ecg_d_data[0])) {
            ix = 2;
            exitg6 = false;
            while ((!exitg6) && (ix <= n)) {
                ixstart = ix;
                if (!rtIsNaN(ecg_d_data[ix - 1])) {
                    THR_SIG1 = ecg_d_data[ix - 1];
                    exitg6 = true;
                } else {
                    ix++;
                }
            }
        }

        if (ixstart < ecg[0]) {
            while (ixstart + 1 <= n) {
                if (ecg_d_data[ixstart] > THR_SIG1) {
                    THR_SIG1 = ecg_d_data[ixstart];
                }

                ixstart++;
            }
        }
    }

    b_n = ecg[0];
    for (i11 = 0; i11 < b_n; i11++) {
        ecg_d_data[i11] /= THR_SIG1;
    }

    /*  delay of derivative filter 2 samples */
    /* ax(4)=subplot(324);plot(ecg_d);axis tight;title('Filtered with the derivative filter'); */
    /*  Squaring nonlinearly enhance the dominant peaks */
    power(ecg_d_data, ecg, ecg_s_data, ecg_s_size);

    /* ax(5)=subplot(325);plot(ecg_s);axis tight;title('Squared'); */
    /*  Moving average Y(nt) = (1/N)[x(nT-(N - 1)T)+ x(nT - (N - 2)T)+...+x(nT)] */
    b_conv(ecg_s_data, ecg_s_size, ecg_m_data, ecg);

    /* { */
    /* if gr */
    /* ax(6)=subplot(326);plot(ecg_m);axis tight;title('Averaged with 30 samples length,Black noise,Green Adaptive Threshold,RED Sig Level,Red circles QRS adaptive threshold'); */
    /* axis tight; */
    /* end */
    /*  */
    /* } */
    /*  Fiducial Mark  */
    /*  Note : a minimum distance of 40 samples is considered between each R wave */
    /*  since in physiological point of view no RR wave can occur in less than */
    /*  200 msec distance */
    findpeaks(ecg_m_data, ecg, pks_data, maxval_size, locs_data, locs_size);

    /*  initialize the training phase (2 seconds of the signal) to determine the THR_SIG and THR_NOISE */
    ixstart = 1;
    THR_SIG1 = ecg_m_data[0];
    if (rtIsNaN(ecg_m_data[0])) {
        ix = 2;
        exitg5 = false;
        while ((!exitg5) && (ix < 301)) {
            ixstart = ix;
            if (!rtIsNaN(ecg_m_data[ix - 1])) {
                THR_SIG1 = ecg_m_data[ix - 1];
                exitg5 = true;
            } else {
                ix++;
            }
        }
    }

    if (ixstart < 300) {
        while (ixstart + 1 < 301) {
            if (ecg_m_data[iv2[ixstart] - 1] > THR_SIG1) {
                THR_SIG1 = ecg_m_data[(short) (1 + (short) ixstart) - 1];
            }

            ixstart++;
        }
    }

    THR_SIG = THR_SIG1 / 3.0;

    /*  0.25 of the max amplitude  */
    THR_NOISE = mean(*(double (*)[300]) &ecg_m_data[0]) / 2.0;

    /*  0.5 of the mean signal is considered to be noise */
    SIG_LEV = THR_SIG;
    NOISE_LEV = THR_NOISE;

    /*  Initialize bandpath filter threshold(2 seconds of the bandpass signal) */
    ixstart = 1;
    THR_SIG1 = ecg_h_data[0];
    if (rtIsNaN(ecg_h_data[0])) {
        ix = 2;
        exitg4 = false;
        while ((!exitg4) && (ix < 301)) {
            ixstart = ix;
            if (!rtIsNaN(ecg_h_data[ix - 1])) {
                THR_SIG1 = ecg_h_data[ix - 1];
                exitg4 = true;
            } else {
                ix++;
            }
        }
    }

    if (ixstart < 300) {
        while (ixstart + 1 < 301) {
            if (ecg_h_data[iv2[ixstart] - 1] > THR_SIG1) {
                THR_SIG1 = ecg_h_data[(short) (1 + (short) ixstart) - 1];
            }

            ixstart++;
        }
    }

    THR_SIG1 /= 3.0;

    /*  0.25 of the max amplitude  */
    /*  */
    SIG_LEV1 = THR_SIG1;

    /*  Signal level in Bandpassed filter */
    NOISE_LEV1 = mean(*(double (*)[300]) &ecg_h_data[0]) / 2.0;

    /*  Noise level in Bandpassed filter */
    /*  Thresholding and online desicion rule */
    b_n = maxval_size[0];
    if (!(b_n >= 1)) {
        b_n = 1;
    }

    if (maxval_size[0] == 0) {
        n = 0;
    } else {
        n = b_n;
    }

    for (i = 0; i < n; i++) {
        /*    %% locate the corresponding peak in the filtered signal  */
        if ((locs_data[i] - 23.0 >= 1.0) && (locs_data[i] <= ecg_h_size[0])) {
            if (locs_data[i] - 23.0 > locs_data[i]) {
                i11 = 1;
                i12 = 1;
            } else {
                i11 = (int) (locs_data[i] - 23.0);
                i12 = (int) locs_data[i] + 1;
            }

            b_n = i12 - i11;
            for (itmp = 0; itmp < b_n; itmp++) {
                varargin_1_data[itmp] = ecg_h_data[(i11 + itmp) - 1];
            }

            ixstart = 1;
            b_n = i12 - i11;
            y_i = varargin_1_data[0];
            itmp = 1;
            if (i12 - i11 > 1) {
                if (rtIsNaN(varargin_1_data[0])) {
                    ix = 2;
                    exitg3 = false;
                    while ((!exitg3) && (ix <= b_n)) {
                        ixstart = ix;
                        if (!rtIsNaN(varargin_1_data[ix - 1])) {
                            y_i = varargin_1_data[ix - 1];
                            itmp = ix;
                            exitg3 = true;
                        } else {
                            ix++;
                        }
                    }
                }

                if (ixstart < i12 - i11) {
                    while (ixstart + 1 <= b_n) {
                        if (varargin_1_data[ixstart] > y_i) {
                            y_i = varargin_1_data[ixstart];
                            itmp = ixstart + 1;
                        }

                        ixstart++;
                    }
                }
            }

            x_i = itmp;
        } else if (1 + i == 1) {
            b_n = (int) locs_data[0];
            for (i11 = 0; i11 < b_n; i11++) {
                varargin_1_data[i11] = ecg_h_data[i11];
            }

            ixstart = 1;
            b_n = (int) locs_data[0];
            y_i = varargin_1_data[0];
            itmp = 1;
            i11 = (int) locs_data[0];
            if (i11 > 1) {
                if (rtIsNaN(varargin_1_data[0])) {
                    ix = 2;
                    exitg2 = false;
                    while ((!exitg2) && (ix <= b_n)) {
                        ixstart = ix;
                        if (!rtIsNaN(varargin_1_data[ix - 1])) {
                            y_i = varargin_1_data[ix - 1];
                            itmp = ix;
                            exitg2 = true;
                        } else {
                            ix++;
                        }
                    }
                }

                i11 = (int) locs_data[0];
                if (ixstart < i11) {
                    while (ixstart + 1 <= b_n) {
                        if (varargin_1_data[ixstart] > y_i) {
                            y_i = varargin_1_data[ixstart];
                            itmp = ixstart + 1;
                        }

                        ixstart++;
                    }
                }
            }

            x_i = itmp;
            ser_back = 1;
        } else {
            if (locs_data[i] >= ecg_h_size[0]) {
                if (locs_data[i] - 23.0 > ecg_h_size[0]) {
                    i11 = 1;
                    i12 = 1;
                } else {
                    i11 = (int) (locs_data[i] - 23.0);
                    i12 = ecg_h_size[0] + 1;
                }

                b_n = i12 - i11;
                for (itmp = 0; itmp < b_n; itmp++) {
                    varargin_1_data[itmp] = ecg_h_data[(i11 + itmp) - 1];
                }

                ixstart = 1;
                b_n = i12 - i11;
                y_i = varargin_1_data[0];
                itmp = 1;
                if (i12 - i11 > 1) {
                    if (rtIsNaN(varargin_1_data[0])) {
                        ix = 2;
                        exitg1 = false;
                        while ((!exitg1) && (ix <= b_n)) {
                            ixstart = ix;
                            if (!rtIsNaN(varargin_1_data[ix - 1])) {
                                y_i = varargin_1_data[ix - 1];
                                itmp = ix;
                                exitg1 = true;
                            } else {
                                ix++;
                            }
                        }
                    }

                    if (ixstart < i12 - i11) {
                        while (ixstart + 1 <= b_n) {
                            if (varargin_1_data[ixstart] > y_i) {
                                y_i = varargin_1_data[ixstart];
                                itmp = ixstart + 1;
                            }

                            ixstart++;
                        }
                    }
                }

                x_i = itmp;
            }
        }

        /*   %% update the heart_rate (Two heart rate means one the moste recent and the other selected) */
        /*       %% calculate the mean of the last 8 R waves to make sure that QRS is not */
        /*  missing(If no R detected , trigger a search back) 1.66*mean */
        /*     %%  find noise and QRS peaks */
        if (pks_data[i] >= THR_SIG) {
            /*  if a QRS candidate occurs within 360ms of the previous QRS */
            /*  ,the algorithm determines if its T wave or QRS */
            /*  skip is 1 when a T wave is detected        */
            /*   qrs_c = [qrs_c pks(i)]; */
            /*   qrs_i = [qrs_i locs(i)]; */
            /*  bandpass filter check threshold */
            if (y_i >= THR_SIG1) {
                if (ser_back != 0) {
                    b_n = qrs_i_raw->size[1];
                    i11 = qrs_i_raw->size[0] * qrs_i_raw->size[1];
                    qrs_i_raw->size[1] = b_n + 1;
                    emxEnsureCapacity((emxArray__common *) qrs_i_raw, i11, (int) sizeof
                            (double));
                    qrs_i_raw->data[b_n] = x_i;

                    /*  save index of bandpass  */
                } else {
                    b_n = qrs_i_raw->size[1];
                    i11 = qrs_i_raw->size[0] * qrs_i_raw->size[1];
                    qrs_i_raw->size[1] = b_n + 1;
                    emxEnsureCapacity((emxArray__common *) qrs_i_raw, i11, (int) sizeof
                            (double));
                    qrs_i_raw->data[b_n] = (locs_data[i] - 23.0) + (x_i - 1.0);

                    /*  save index of bandpass  */
                }

                b_n = qrs_amp_raw->size[1];
                i11 = qrs_amp_raw->size[0] * qrs_amp_raw->size[1];
                qrs_amp_raw->size[1] = b_n + 1;
                emxEnsureCapacity((emxArray__common *) qrs_amp_raw, i11, (int) sizeof
                        (double));
                qrs_amp_raw->data[b_n] = y_i;

                /*  save amplitude of bandpass  */
                SIG_LEV1 = 0.125 * y_i + 0.875 * SIG_LEV1;

                /*  adjust threshold for bandpass filtered sig */
            }

            /*  rahul 1 */
            /*  adjust Signal level */
            SIG_LEV = 0.125 * pks_data[i] + 0.875 * SIG_LEV;
        } else if ((THR_NOISE <= pks_data[i]) && (pks_data[i] < THR_SIG)) {
            /* adjust Noise level in filtered sig */
            NOISE_LEV1 = 0.125 * y_i + 0.875 * NOISE_LEV1;

            /* adjust Noise level in MVI */
            NOISE_LEV = 0.125 * pks_data[i] + 0.875 * NOISE_LEV;
        } else {
            if (pks_data[i] < THR_NOISE) {
                /*  noise level in filtered signal */
                NOISE_LEV1 = 0.125 * y_i + 0.875 * NOISE_LEV1;

                /* end */
                /* adjust Noise level in MVI */
                NOISE_LEV = 0.125 * pks_data[i] + 0.875 * NOISE_LEV;
            }
        }

        /*     %% adjust the threshold with SNR */
        if ((NOISE_LEV != 0.0) || (SIG_LEV != 0.0)) {
            THR_SIG = NOISE_LEV + 0.25 * fabs(SIG_LEV - NOISE_LEV);
            THR_NOISE = 0.5 * THR_SIG;
        }

        /*  adjust the threshold with SNR for bandpassed signal */
        if ((NOISE_LEV1 != 0.0) || (SIG_LEV1 != 0.0)) {
            THR_SIG1 = NOISE_LEV1 + 0.25 * fabs(SIG_LEV1 - NOISE_LEV1);
        }

        /*  take a track of thresholds of smoothed signal */
        b_n = NOISL_buf->size[1];
        i11 = NOISL_buf->size[0] * NOISL_buf->size[1];
        NOISL_buf->size[1] = b_n + 1;
        emxEnsureCapacity((emxArray__common *) NOISL_buf, i11, (int) sizeof(double));
        NOISL_buf->data[b_n] = NOISE_LEV;

        /*  take a track of thresholds of filtered signal */
        /* reset parameters */
        /* reset parameters */
        ser_back = 0;

        /* reset bandpass param    */
    }

    /* hold on,scatter(qrs_i,qrs_c,'m'); */
    /* hold on,plot(locs,NOISL_buf,'--k','LineWidth',2); */
    /* hold on,plot(locs,SIGL_buf,'--r','LineWidth',2); */
    /* hold on,plot(locs,THRS_buf,'--g','LineWidth',2); */
    /* { */
    /* if ax(:) */
    /* linkaxes(ax,'x'); */
    /* zoom on; */
    /* end */
    /* } */
    /* { */
    /* for x=0:length(qrs_i_raw) */
    /*     ecg2 =  -1 * ecg(qrs_i_raw(x,0)); */
    /*     disp(ecg2); */
    /* end */
    /* } */
    /* { */
    /*  overlay on the signals */
    /* if gr */
    /* figure,az(1)=subplot(311);plot(ecg_h);title('QRS on Filtered Signal');axis tight; */
    /*   */
    /* hold on,scatter(qrs_i_raw,qrs_amp_raw,'m'); */
    /* hold on,plot(locs,NOISL_buf1,'LineWidth',2,'Linestyle','--','color','k'); */
    /* hold on,plot(locs,SIGL_buf1,'LineWidth',2,'Linestyle','-.','color','r'); */
    /* hold on,plot(locs,THRS_buf1,'LineWidth',2,'Linestyle','-.','color','g'); */
    /* az(2)=subplot(312);plot(ecg_m);title('QRS on MVI signal and Noise level(black),Signal Level (red) and Adaptive Threshold(green)');axis tight; */
    /* hold on,scatter(qrs_i,qrs_c,'m'); */
    /* hold on,plot(locs,NOISL_buf,'LineWidth',2,'Linestyle','--','color','k'); */
    /* hold on,plot(locs,SIGL_buf,'LineWidth',2,'Linestyle','-.','color','r'); */
    /* hold on,plot(locs,THRS_buf,'LineWidth',2,'Linestyle','-.','color','g'); */
    /* az(3)=subplot(313);plot(ecg-mean(ecg));title('Pulse train of the found QRS on ECG signal');axis tight; */
    /* line(repmat(qrs_i_raw,[2 1]),repmat([min(ecg-mean(ecg))/2; max(ecg-mean(ecg))/2],size(qrs_i_raw)),'LineWidth',2.5,'LineStyle','-.','Color','r'); */
    /* linkaxes(az,'x'); */
    /* zoom on; */
    /* end */
    /* } */
}

void pan_tompkin(double ecg_data[], int ecg_size[1], emxArray_real_T
*qrs_amp_raw, emxArray_real_T *qrs_i_raw, emxArray_real_T
                 *NOISL_buf, emxArray_real_T *SIG_buf, double locs_data[], int locs_size[2]) {
    emxArray_real_T *ecg_h;
    emxArray_real_T *varargin_2;
    int i0;
    int ser_back;
    static double maxval_data[1800];
    int maxval_size[2];
    int n;
    int i;
    int ix;
    static double ecg_h_data[1800];
    int ecg_h_size[1];
    int ixstart;
    int b_n;
    double THR_SIG1;
    static double ecg_d_data[1800];
    int ecg_d_size[1];
    boolean_T exitg7;
    boolean_T exitg6;
    static double ecg_s_data[1800];
    int ecg_s_size[1];
    static double ecg_m_data[1800];
    static double pks_data[59];
    boolean_T exitg5;
    double THR_SIG;
    static const short iv0[300] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
                                   15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
                                   31, 32, 33,
                                   34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
                                   50, 51, 52,
                                   53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68,
                                   69, 70, 71,
                                   72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87,
                                   88, 89, 90,
                                   91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105,
                                   106, 107,
                                   108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120,
                                   121, 122,
                                   123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135,
                                   136, 137,
                                   138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150,
                                   151, 152,
                                   153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165,
                                   166, 167,
                                   168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180,
                                   181, 182,
                                   183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195,
                                   196, 197,
                                   198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210,
                                   211, 212,
                                   213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225,
                                   226, 227,
                                   228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240,
                                   241, 242,
                                   243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255,
                                   256, 257,
                                   258, 259, 260, 261, 262, 263, 264, 265, 266, 267, 268, 269, 270,
                                   271, 272,
                                   273, 274, 275, 276, 277, 278, 279, 280, 281, 282, 283, 284, 285,
                                   286, 287,
                                   288, 289, 290, 291, 292, 293, 294, 295, 296, 297, 298, 299, 300};

    double THR_NOISE;
    double SIG_LEV;
    double NOISE_LEV;
    boolean_T exitg4;
    double SIG_LEV1;
    double NOISE_LEV1;
    int i1;
    static double varargin_1_data[1800];
    int itmp;
    boolean_T exitg2;
    boolean_T exitg3;
    boolean_T exitg1;

    emxInit_real_T(&ecg_h, 2);
    emxInit_real_T(&varargin_2, 2);

    /*  vectorize */
    /*  Initialize */
    /* amplitude of R */
    /* index */
    /*  becomes one when a T wave is detected */
    /*  it is not noise when not_nois = 1 */
    /*  Selected RR intervals */

    i0 = qrs_i_raw->size[0] * qrs_i_raw->size[1];
    qrs_i_raw->size[0] = 1;
    qrs_i_raw->size[1] = 0;
    emxEnsureCapacity((emxArray__common *) qrs_i_raw, i0, (int) sizeof(double));
    i0 = qrs_amp_raw->size[0] * qrs_amp_raw->size[1];
    qrs_amp_raw->size[0] = 1;
    qrs_amp_raw->size[1] = 0;
    emxEnsureCapacity((emxArray__common *) qrs_amp_raw, i0, (int) sizeof(double));
    ser_back = 0;
    i0 = NOISL_buf->size[0] * NOISL_buf->size[1];
    NOISL_buf->size[0] = 1;
    NOISL_buf->size[1] = 0;
    SIG_buf->size[0] = 1;
    SIG_buf->size[1] = 0;
    emxEnsureCapacity((emxArray__common *) NOISL_buf, i0, (int) sizeof(double));
    emxEnsureCapacity((emxArray__common *) SIG_buf, i0, (int) sizeof(double));
    /* { */
    /*  Plot differently based on filtering settings */
    /* if gr */
    /*  if fs == 200 */
    /*   figure,  ax(1)=subplot(321);plot(ecg);axis tight;title('Raw ECG Signal'); */
    /*  else */
    /*   figure,  ax(1)=subplot(3,2,[1 2]);plot(ecg);axis tight;title('Raw ECG Signal'); */
    /*  end */
    /* end    */
    /* } */
    /*  Noise cancelation(Filtering) % Filters (Filter in between 5-15 Hz) */
    /*  bandpass filter for Noise cancelation of other sampling frequencies(Filtering) */
    /* cuttoff low frequency to get rid of baseline wander */
    /* cuttoff frequency to discard high frequency noise */
    /*  cutt off based on fs */
    /*  order of 3 less processing */
    /* bandpass filtering */
    filtfilt(ecg_data, ecg_size, ecg_h);
    b_abs(ecg_h, varargin_2);
    maxval_size[0] = 1;
    maxval_size[1] = (short) varargin_2->size[1];
    n = varargin_2->size[0];
    for (i = 0; i + 1 <= varargin_2->size[1]; i++) {
        ix = i * n;
        ixstart = i * n + 1;
        b_n = ix + n;
        THR_SIG1 = varargin_2->data[ix];
        if (n > 1) {
            if (rtIsNaN(varargin_2->data[ix])) {
                ix = ixstart;
                exitg7 = false;
                while ((!exitg7) && (ix + 1 <= b_n)) {
                    ixstart = ix + 1;
                    if (!rtIsNaN(varargin_2->data[ix])) {
                        THR_SIG1 = varargin_2->data[ix];
                        exitg7 = true;
                    } else {
                        ix++;
                    }
                }
            }

            if (ixstart < b_n) {
                while (ixstart + 1 <= b_n) {
                    if (varargin_2->data[ixstart] > THR_SIG1) {
                        THR_SIG1 = varargin_2->data[ixstart];
                    }

                    ixstart++;
                }
            }
        }

        maxval_data[i] = THR_SIG1;
    }

    emxFree_real_T(&varargin_2);
    mrdivide(ecg_h, maxval_data, maxval_size, ecg_h_data, ecg_h_size);

    /* ax(3)=subplot(323);plot(ecg_h);axis tight;title('Band Pass Filtered'); */
    /*  derivative filter H(z) = (1/8T)(-z^(-2) - 2z^(-1) + 2z + z^(2)) */
    /* 1/8*fs */
    conv(ecg_h_data, ecg_h_size, ecg_d_data, ecg_d_size);
    ixstart = 1;
    n = ecg_d_size[0];
    THR_SIG1 = ecg_d_data[0];
    emxFree_real_T(&ecg_h);
    if (ecg_d_size[0] > 1) {
        if (rtIsNaN(ecg_d_data[0])) {
            ix = 2;
            exitg6 = false;
            while ((!exitg6) && (ix <= n)) {
                ixstart = ix;
                if (!rtIsNaN(ecg_d_data[ix - 1])) {
                    THR_SIG1 = ecg_d_data[ix - 1];
                    exitg6 = true;
                } else {
                    ix++;
                }
            }
        }

        if (ixstart < ecg_d_size[0]) {
            while (ixstart + 1 <= n) {
                if (ecg_d_data[ixstart] > THR_SIG1) {
                    THR_SIG1 = ecg_d_data[ixstart];
                }

                ixstart++;
            }
        }
    }

    b_n = ecg_d_size[0];
    for (i0 = 0; i0 < b_n; i0++) {
        ecg_d_data[i0] /= THR_SIG1;
    }

    /*  delay of derivative filter 2 samples */
    /* ax(4)=subplot(324);plot(ecg_d);axis tight;title('Filtered with the derivative filter'); */
    /*  Squaring nonlinearly enhance the dominant peaks */
    power(ecg_d_data, ecg_d_size, ecg_s_data, ecg_s_size);

    /* ax(5)=subplot(325);plot(ecg_s);axis tight;title('Squared'); */
    /*  Moving average Y(nt) = (1/N)[x(nT-(N - 1)T)+ x(nT - (N - 2)T)+...+x(nT)] */
    b_conv(ecg_s_data, ecg_s_size, ecg_m_data, ecg_d_size);

    /* { */
    /* if gr */
    /* ax(6)=subplot(326);plot(ecg_m);axis tight;title('Averaged with 30 samples length,Black noise,Green Adaptive Threshold,RED Sig Level,Red circles QRS adaptive threshold'); */
    /* axis tight; */
    /* end */
    /*  */
    /* } */
    /*  Fiducial Mark  */
    /*  Note : a minimum distance of 40 samples is considered between each R wave */
    /*  since in physiological point of view no RR wave can occur in less than */
    /*  200 msec distance */
    findpeaks(ecg_m_data, ecg_d_size, pks_data, maxval_size, locs_data, locs_size);

    /*  initialize the training phase (2 seconds of the signal) to determine the THR_SIG and THR_NOISE */
    ixstart = 1;
    THR_SIG1 = ecg_m_data[0];
    if (rtIsNaN(ecg_m_data[0])) {
        ix = 2;
        exitg5 = false;
        while ((!exitg5) && (ix < 301)) {
            ixstart = ix;
            if (!rtIsNaN(ecg_m_data[ix - 1])) {
                THR_SIG1 = ecg_m_data[ix - 1];
                exitg5 = true;
            } else {
                ix++;
            }
        }
    }

    if (ixstart < 300) {
        while (ixstart + 1 < 301) {
            if (ecg_m_data[iv0[ixstart] - 1] > THR_SIG1) {
                THR_SIG1 = ecg_m_data[(short) (1 + (short) ixstart) - 1];
            }

            ixstart++;
        }
    }

    THR_SIG = THR_SIG1 / 3.0;

    /*  0.25 of the max amplitude  */
    THR_NOISE = mean(*(double (*)[300]) &ecg_m_data[0]) / 2.0;

    /*  0.5 of the mean signal is considered to be noise */
    SIG_LEV = THR_SIG;
    NOISE_LEV = THR_NOISE;

    /*  Initialize bandpath filter threshold(2 seconds of the bandpass signal) */
    ixstart = 1;
    THR_SIG1 = ecg_h_data[0];
    if (rtIsNaN(ecg_h_data[0])) {
        ix = 2;
        exitg4 = false;
        while ((!exitg4) && (ix < 301)) {
            ixstart = ix;
            if (!rtIsNaN(ecg_h_data[ix - 1])) {
                THR_SIG1 = ecg_h_data[ix - 1];
                exitg4 = true;
            } else {
                ix++;
            }
        }
    }

    if (ixstart < 300) {
        while (ixstart + 1 < 301) {
            if (ecg_h_data[iv0[ixstart] - 1] > THR_SIG1) {
                THR_SIG1 = ecg_h_data[(short) (1 + (short) ixstart) - 1];
            }

            ixstart++;
        }
    }

    THR_SIG1 /= 3.0;

    /*  0.25 of the max amplitude  */
    /*  */
    SIG_LEV1 = THR_SIG1;

    /*  Signal level in Bandpassed filter */
    NOISE_LEV1 = mean(*(double (*)[300]) &ecg_h_data[0]) / 2.0;

    /*  Noise level in Bandpassed filter */
    /*  Thresholding and online desicion rule */
    b_n = maxval_size[0];
    if (!(b_n >= 1)) {
        b_n = 1;
    }

    if (maxval_size[0] == 0) {
        n = 0;
    } else {
        n = b_n;
    }

    for (i = 0; i < n; i++) {
        /*    %% locate the corresponding peak in the filtered signal  */
        if ((locs_data[i] - 23.0 >= 1.0) && (locs_data[i] <= ecg_h_size[0])) {
            if (locs_data[i] - 23.0 > locs_data[i]) {
                i0 = 1;
                i1 = 1;
            } else {
                i0 = (int) (locs_data[i] - 23.0);
                i1 = (int) locs_data[i] + 1;
            }

            b_n = i1 - i0;
            for (itmp = 0; itmp < b_n; itmp++) {
                varargin_1_data[itmp] = ecg_h_data[(i0 + itmp) - 1];
            }

            ixstart = 1;
            b_n = i1 - i0;
            y_i = varargin_1_data[0];
            itmp = 1;
            if (i1 - i0 > 1) {
                if (rtIsNaN(varargin_1_data[0])) {
                    ix = 2;
                    exitg3 = false;
                    while ((!exitg3) && (ix <= b_n)) {
                        ixstart = ix;
                        if (!rtIsNaN(varargin_1_data[ix - 1])) {
                            y_i = varargin_1_data[ix - 1];
                            itmp = ix;
                            exitg3 = true;
                        } else {
                            ix++;
                        }
                    }
                }

                if (ixstart < i1 - i0) {
                    while (ixstart + 1 <= b_n) {
                        if (varargin_1_data[ixstart] > y_i) {
                            y_i = varargin_1_data[ixstart];
                            itmp = ixstart + 1;
                        }

                        ixstart++;
                    }
                }
            }

            x_i = itmp;
        } else if (1 + i == 1) {
            b_n = (int) locs_data[0];
            for (i0 = 0; i0 < b_n; i0++) {
                varargin_1_data[i0] = ecg_h_data[i0];
            }

            ixstart = 1;
            b_n = (int) locs_data[0];
            y_i = varargin_1_data[0];
            itmp = 1;
            i0 = (int) locs_data[0];
            if (i0 > 1) {
                if (rtIsNaN(varargin_1_data[0])) {
                    ix = 2;
                    exitg2 = false;
                    while ((!exitg2) && (ix <= b_n)) {
                        ixstart = ix;
                        if (!rtIsNaN(varargin_1_data[ix - 1])) {
                            y_i = varargin_1_data[ix - 1];
                            itmp = ix;
                            exitg2 = true;
                        } else {
                            ix++;
                        }
                    }
                }

                i0 = (int) locs_data[0];
                if (ixstart < i0) {
                    while (ixstart + 1 <= b_n) {
                        if (varargin_1_data[ixstart] > y_i) {
                            y_i = varargin_1_data[ixstart];
                            itmp = ixstart + 1;
                        }

                        ixstart++;
                    }
                }
            }

            x_i = itmp;
            ser_back = 1;
        } else {
            if (locs_data[i] >= ecg_h_size[0]) {
                if (locs_data[i] - 23.0 > ecg_h_size[0]) {
                    i0 = 1;
                    i1 = 1;
                } else {
                    i0 = (int) (locs_data[i] - 23.0);
                    i1 = ecg_h_size[0] + 1;
                }

                b_n = i1 - i0;
                for (itmp = 0; itmp < b_n; itmp++) {
                    varargin_1_data[itmp] = ecg_h_data[(i0 + itmp) - 1];
                }

                ixstart = 1;
                b_n = i1 - i0;
                y_i = varargin_1_data[0];
                itmp = 1;
                if (i1 - i0 > 1) {
                    if (rtIsNaN(varargin_1_data[0])) {
                        ix = 2;
                        exitg1 = false;
                        while ((!exitg1) && (ix <= b_n)) {
                            ixstart = ix;
                            if (!rtIsNaN(varargin_1_data[ix - 1])) {
                                y_i = varargin_1_data[ix - 1];
                                itmp = ix;
                                exitg1 = true;
                            } else {
                                ix++;
                            }
                        }
                    }

                    if (ixstart < i1 - i0) {
                        while (ixstart + 1 <= b_n) {
                            if (varargin_1_data[ixstart] > y_i) {
                                y_i = varargin_1_data[ixstart];
                                itmp = ixstart + 1;
                            }

                            ixstart++;
                        }
                    }
                }

                x_i = itmp;
            }
        }

        /*   %% update the heart_rate (Two heart rate means one the moste recent and the other selected) */
        /*       %% calculate the mean of the last 8 R waves to make sure that QRS is not */
        /*  missing(If no R detected , trigger a search back) 1.66*mean */
        /*     %%  find noise and QRS peaks */
        if (pks_data[i] >= THR_SIG) {
            /*  if a QRS candidate occurs within 360ms of the previous QRS */
            /*  ,the algorithm determines if its T wave or QRS */
            /*  skip is 1 when a T wave is detected        */
            /*   qrs_c = [qrs_c pks(i)]; */
            /*   qrs_i = [qrs_i locs(i)]; */
            /*  bandpass filter check threshold */
            if (y_i >= THR_SIG1) {
                if (ser_back != 0) {
                    b_n = qrs_i_raw->size[1];
                    i0 = qrs_i_raw->size[0] * qrs_i_raw->size[1];
                    qrs_i_raw->size[1] = b_n + 1;
                    emxEnsureCapacity((emxArray__common *) qrs_i_raw, i0, (int) sizeof
                            (double));
                    qrs_i_raw->data[b_n] = x_i;

                    /*  save index of bandpass  */
                } else {
                    b_n = qrs_i_raw->size[1];
                    i0 = qrs_i_raw->size[0] * qrs_i_raw->size[1];
                    qrs_i_raw->size[1] = b_n + 1;
                    emxEnsureCapacity((emxArray__common *) qrs_i_raw, i0, (int) sizeof
                            (double));
                    qrs_i_raw->data[b_n] = (locs_data[i] - 23.0) + (x_i - 1.0);

                    /*  save index of bandpass  */
                }

                b_n = qrs_amp_raw->size[1];
                i0 = qrs_amp_raw->size[0] * qrs_amp_raw->size[1];
                qrs_amp_raw->size[1] = b_n + 1;
                emxEnsureCapacity((emxArray__common *) qrs_amp_raw, i0, (int) sizeof
                        (double));
                qrs_amp_raw->data[b_n] = y_i;

                /*  save amplitude of bandpass  */
                SIG_LEV1 = 0.125 * y_i + 0.875 * SIG_LEV1;

                /*  adjust threshold for bandpass filtered sig */
            }

            /*  rahul 1 */
            /*  adjust Signal level */
            SIG_LEV = 0.125 * pks_data[i] + 0.875 * SIG_LEV;
        } else if ((THR_NOISE <= pks_data[i]) && (pks_data[i] < THR_SIG)) {
            /* adjust Noise level in filtered sig */
            NOISE_LEV1 = 0.125 * y_i + 0.875 * NOISE_LEV1;

            /* adjust Noise level in MVI */
            NOISE_LEV = 0.125 * pks_data[i] + 0.875 * NOISE_LEV;
        } else {
            if (pks_data[i] < THR_NOISE) {
                /*  noise level in filtered signal */
                NOISE_LEV1 = 0.125 * y_i + 0.875 * NOISE_LEV1;

                /* end */
                /* adjust Noise level in MVI */
                NOISE_LEV = 0.125 * pks_data[i] + 0.875 * NOISE_LEV;
            }
        }

        /*     %% adjust the threshold with SNR */
        if ((NOISE_LEV != 0.0) || (SIG_LEV != 0.0)) {
            THR_SIG = NOISE_LEV + 0.25 * fabs(SIG_LEV - NOISE_LEV);
            THR_NOISE = 0.5 * THR_SIG;
        }

        /*  adjust the threshold with SNR for bandpassed signal */
        if ((NOISE_LEV1 != 0.0) || (SIG_LEV1 != 0.0)) {
            THR_SIG1 = NOISE_LEV1 + 0.25 * fabs(SIG_LEV1 - NOISE_LEV1);
        }

        /*  take a track of thresholds of smoothed signal */
        b_n = NOISL_buf->size[1];
        i0 = NOISL_buf->size[0] * NOISL_buf->size[1];
        NOISL_buf->size[1] = b_n + 1;
        SIG_buf->size[1] = b_n + 1;
        emxEnsureCapacity((emxArray__common *) NOISL_buf, i0, (int) sizeof(double));
        emxEnsureCapacity((emxArray__common *) SIG_buf, i0, (int) sizeof(double));
        NOISL_buf->data[b_n] = NOISE_LEV;

        /*  take a track of thresholds of filtered signal */
        /* reset parameters */
        /* reset parameters */
        SIG_buf->data[b_n] = SIG_LEV1;

        /* reset bandpass param    */
    }

    /* hold on,scatter(qrs_i,qrs_c,'m'); */
    /* hold on,plot(locs,NOISL_buf,'--k','LineWidth',2); */
    /* hold on,plot(locs,SIGL_buf,'--r','LineWidth',2); */
    /* hold on,plot(locs,THRS_buf,'--g','LineWidth',2); */
    /* { */
    /* if ax(:) */
    /* linkaxes(ax,'x'); */
    /* zoom on; */
    /* end */
    /* } */
    /* { */
    /* for x=0:length(qrs_i_raw) */
    /*     ecg2 =  -1 * ecg(qrs_i_raw(x,0)); */
    /*     disp(ecg2); */
    /* end */
    /* } */
    /* { */
    /*  overlay on the signals */
    /* if gr */
    /* figure,az(1)=subplot(311);plot(ecg_h);title('QRS on Filtered Signal');axis tight; */
    /*   */
    /* hold on,scatter(qrs_i_raw,qrs_amp_raw,'m'); */
    /* hold on,plot(locs,NOISL_buf1,'LineWidth',2,'Linestyle','--','color','k'); */
    /* hold on,plot(locs,SIGL_buf1,'LineWidth',2,'Linestyle','-.','color','r'); */
    /* hold on,plot(locs,THRS_buf1,'LineWidth',2,'Linestyle','-.','color','g'); */
    /* az(2)=subplot(312);plot(ecg_m);title('QRS on MVI signal and Noise level(black),Signal Level (red) and Adaptive Threshold(green)');axis tight; */
    /* hold on,scatter(qrs_i,qrs_c,'m'); */
    /* hold on,plot(locs,NOISL_buf,'LineWidth',2,'Linestyle','--','color','k'); */
    /* hold on,plot(locs,SIGL_buf,'LineWidth',2,'Linestyle','-.','color','r'); */
    /* hold on,plot(locs,THRS_buf,'LineWidth',2,'Linestyle','-.','color','g'); */
    /* az(3)=subplot(313);plot(ecg-mean(ecg));title('Pulse train of the found QRS on ECG signal');axis tight; */
    /* line(repmat(qrs_i_raw,[2 1]),repmat([min(ecg-mean(ecg))/2; max(ecg-mean(ecg))/2],size(qrs_i_raw)),'LineWidth',2.5,'LineStyle','-.','Color','r'); */
    /* linkaxes(az,'x'); */
    /* zoom on; */
    /* end */
    /* } */
}

void pan_tompkin_init(void) {
    /*  Initialization */
    y_i = 0.0;
    x_i = 0.0;
}

/* End of code generation (pan_tompkin.c) */
