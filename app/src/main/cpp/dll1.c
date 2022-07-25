/*
 * dll1.c
 *
 * Code generation for function 'dll1'
 *
 */

/* Include files */
#include "rt_nonfinite.h"
#include "bdac.h"
#include "dll1.h"
#include "pan_tompkin.h"
#include "dll1_emxutil.h"
#include "diff.h"
#include <jni.h>
#include "filtfilt.h"

//#include "interface/_coder_dll1_api.h"
#include <android/log.h>
//#include "interface/_coder_dll1_api.h"

#define APPNAME "MyApp"

int    AvgPRVal;
int    AvgQTVal;
int    AvgQTcVal;
int    AvgQRSVal;

emxArray_real_T *g_qrs_i_raw;
emxArray_real_T *g_qrs_amp_raw;
emxArray_real_T *g_noise;
emxArray_real_T *g_sig_value;
double ECG_new_data[1500];

int flagpeak1 =0,flagpeak2 =0,flagpeak3 =0;

int grepeat = 0;
boolean_T exitg1;
double s1 = 0;
//double ver = 21.0;
double rrinterval_array[50];
static double g_rrinterval_data[50];

double *g_t1;
double *g_t2;
double *g_t3;
int g_rrinterval1[15];
double g_rrvalue[15];
emxArray_real_T *g_rrinterval2;
emxArray_real_T *g_rrinterval3;
static double g_l4s_data[500];
static double g1_l4s_data[500];
static double g_l4s2_data[500];
static double g_l4s3_data[500];
static double l_avr[500];
static double l_avf[500];
static double l_avl[500];
double sig_val1 = 0;

int gNoiseValue = 0;
static int c2 =0;

static const int ecg_len1 = 1800;// was 799
static const int ecg_len2 = 400;// was 799
static const int ecg_total_len = 1800;

#define NOTREADY 33
#define WINDOW_LENGTH1 100
static int length1  =0;

double ECG[WINDOW_LENGTH1];

double b_x[5];
int i;
double xtmp;
int j;
double k;
double y[100];
double F;
double W;

jdouble  b_ecg_data[401],b_ecg2_data[401], testarray, b_ecg_data_new[1800], b_ecg2_data_new[401], b_ecg3_data[401],b_ecg_data_single[1800];
jdouble b_ecg_data_new_single[1800];
//
//#define WINDOW_LENGTH 400
#define WINDOW_LENGTH 400
#define WINDOW_LENGTH12 1500

#define WINDOW_LENGTH_R 100
#define WINDOW_LENGTH_QS 10
#define MAX_AVG        10
#define NOTREADY 33
static int    length  =0;
static int    counter =0;

static int    ecglength  =0;

// static int    length1  =0;
static int    counter1 =0;
int    RRPeakVal=0;
double PQRSSampl1[WINDOW_LENGTH];
double ECGSAMPLE[1505];
double ECG2[WINDOW_LENGTH1];
double finalECG[WINDOW_LENGTH1];
double PQRSSampl2[WINDOW_LENGTH];
double PQRSLeft[WINDOW_LENGTH_R];
double PQRSRight[WINDOW_LENGTH_R];
double RRPeakValInSecs;
double IntervalInSecs;
double QRS;
double PR;
double QT;
double QTc;
double Ps;
double hr;
double finaly;

double n_sqrt=0.0001;
int    QRSVal;
int    PRVal;
int    PVal;
int    QTVal;
int    QTcVal;
int    POS_QV;
int    POS_TV;


int    iter=1;
int    doubleAvgRRPeakVal;
int    doubleAvgPRVal;
int    doubleAvgQTVal;
int    doubleAvgQTcVal;
int    doubleAvgQRSVal;
int    AvgRRPeakVal;

int pos1Value=0;
int pos2Value=0;
int pos3Value=0;
int sVal;
int g_withtompkin = false;

int peaks[6];

/*
 *  Shruti code start
 */
#include<stdio.h>
#include<stdlib.h>
#include<math.h>

void peakfinder(double ecg[]) {
    double Ist_Maxima = -10000, IInd_Maxima = -10000, IIIrd_Maxima=-10000, Ist_Minima = 10000, slopeS = 50, IInd_Minima = 10000, IIIrd_Minima=10000, ecg1[100], ecg2[100], ecg3[100], slope=170;
    int ctr1 = 0, ctr2 = 0, pos1 = 0, pos2 = 0, pos3=-0, posS1 = 0, posS2 = 0, posS3=0, d, i;
    for (int i = 0; i < 100; ++i) {
        ecg1[i] = ecg[i];
        ecg2[i] = ecg[i];
        ecg3[i] = ecg[i];
    }

//    if(device==sanket3)
//    {
//        slope=170;
//        slopeS = 50;
//    }
//    else
//    {
//        slope=10;
//        slopeS=5;
//    }

    jmp1:
    ctr1++;
    for (d = 0; d < 100; d++) {
        if (Ist_Maxima < ecg1[d] && ecg1[d] != 10000) {
            Ist_Maxima = ecg1[d];
            pos1 = d;
        }
    }

    if (fabs((ecg2[pos1 - 3] - ecg2[pos1]) / 2) < slope || (fabs((ecg2[pos1] - ecg2[pos1 + 3]) / 2) < slope)) {
        ecg1[(pos1)] = 10000;
        for (i = 1; i <= 10; i++) {
            if ((pos1 + i) < 100)
                ecg1[(pos1 + i)] = 10000;
            if ((pos1 - i) >= 0)
                ecg1[(pos1 - i)] = 10000;
        }
        if (ctr1 > 5)
            goto SMax;
        goto jmp1;
    }

    for (i = 1; i <= 10; i++) {
        if ((pos1 + i) < 100)
            ecg1[(pos1 + i)] = 10000;
        if ((pos1 - i) >= 0)
            ecg1[(pos1 - i)] = 10000;
    }
    SMax:
    // condition to check device
    //  if (device==sanket3)
    {
        IInd_Maxima = -10000;
        ctr2++;
        for (d = 0; d < 100; d++) {
            if (IInd_Maxima < ecg1[d] && d != pos1 && ecg1[d] != 10000) {
                IInd_Maxima = ecg1[d];
                pos2 = d;
            }
        }

        if (fabs((ecg2[pos2 - 3] - ecg2[pos2]) / 2) < slope || (fabs((ecg2[pos2] - ecg2[pos2 + 3]) / 2) < slope)) {
            ecg1[(pos2)] = 10000;
            for (i = 1; i <= 10; i++) {
                if ((pos2 + i) < 100)
                    ecg1[(pos2 + i)] = 10000;
                if ((pos2 - i) >= 0)
                    ecg1[(pos2 - i)] = 10000;
            }
            if (ctr2 > 5)
                goto it;
            goto SMax;
        }
        ecg1[pos2] = 10000;
        for (i = 1; i <= 10; i++) {
            if ((pos2 + i) < 100)
                ecg1[(pos2 + i)] = 10000;
            if ((pos2 - i) >= 0)
                ecg1[(pos2 - i)] = 10000;
        }
        if (abs(pos1 - pos2) < 20) {
            goto SMax;
        }

        if (pos2!=0)
        {
            IIIrd_Maxima = -10000;
            for (d = 0; d < 100; d++) {
                if (IIIrd_Maxima < ecg1[d] && d != pos1 && d != pos2 && ecg1[d] != 10000) {
                    IIIrd_Maxima = ecg1[d];
                    pos3 = d;
                }
            }

            if (fabs((ecg2[pos3 - 3] - ecg2[pos3]) / 2) < slope || (fabs((ecg2[pos3] - ecg2[pos3 + 3]) / 2) < slope)) {
                pos3 =0;
            }
        }
    }//else {pos2 =0;pos3=0;}

    it:

    if (pos1!=0&&pos2!=0)
    {
        if (ecg[pos1]<ecg[pos2]/1.25)
        {
            if (fabs((ecg2[pos1 - 3] - ecg2[pos1]) / 2) > slope && (fabs((ecg2[pos1] - ecg2[pos1 + 3]) / 2) > slope))
            {
                flagpeak1=1;
            }
        } else if (ecg[pos2]<ecg[pos1]/1.25)
        {
            if (fabs((ecg2[pos2 - 3] - ecg2[pos2]) / 2) > slope && (fabs((ecg2[pos2] - ecg2[pos2 + 3]) / 2) > slope))
            {
                flagpeak2=1;
            }
        }
        if(pos3!=0)
        {
            if (ecg[pos1]<ecg[pos3]/1.25)
            {
                if (fabs((ecg2[pos1 - 3] - ecg2[pos1]) / 2) > slope && (fabs((ecg2[pos1] - ecg2[pos1 + 3]) / 2) > slope))
                {
                    flagpeak1=1;
                }
            } else if (ecg[pos3]<ecg[pos1]/1.25)
            {
                if (fabs((ecg2[pos3 - 3] - ecg2[pos3]) / 2) > slope && (fabs((ecg2[pos3] - ecg2[pos3 + 3]) / 2) > slope))
                {
                    flagpeak3=1;
                }
            }
            if (ecg[pos3]<ecg[pos2]/1.25)
            {
                if (fabs((ecg2[pos3 - 3] - ecg2[pos3]) / 2) > slope && (fabs((ecg2[pos3] - ecg2[pos3 + 3]) / 2) > slope))
                {
                    flagpeak3=1;
                }
            } else if (ecg[pos2]<ecg[pos3]/1.25)
            {
                if (fabs((ecg2[pos2 - 3] - ecg2[pos2]) / 2) > slope && (fabs((ecg2[pos2] - ecg2[pos2 + 3]) / 2) > slope))
                {
                    flagpeak2=1;
                }
            }
        }
    }

//  S- detection
    for (d = 0; d < 100; d++)
    {
        if (Ist_Minima > ecg3[d])
        {
            Ist_Minima = ecg3[d];
            posS1 = d;
        }
    }

    if (fabs((ecg2[posS1 - 3] - ecg2[posS1]) / 2) < slopeS || (fabs((ecg2[posS1] - ecg2[posS1 + 3]) / 2) < slopeS))
    {
        posS1 = 0;
    }

    ecg3[posS1] = 10000;
    for (i = 1; i <= 10; i++) {
        if ((posS1 + i) < 100)
            ecg3[(posS1 + i)] = 10000;
        if ((posS1 - i) >= 0)
            ecg3[(posS1 - i)] = 10000;
    }

//    *********** code for detection of small R-peaks ************

    //  if (device==sanket3)
//    {
//        for (d = 0; d < 100; d++) {
//            if (IInd_Minima > ecg3[d] && d != posS1 && ecg3[d] != 10000) {
//                IInd_Minima = ecg3[d];
//                posS2 = d;
//            }
//        }
//
//        if (fabs((ecg2[posS2 - 3] - ecg2[posS2]) / 2) < slopeS ||
//            (fabs((ecg2[posS2] - ecg2[posS2 + 3]) / 2) < slopeS)) {
//            posS2 = 0;
//        }
//
//        ecg3[posS2] = 10000;
//        for (i = 1; i <= 10; i++) {
//            if ((posS2 + i) < 100)
//                ecg3[(posS2 + i)] = 10000;
//            if ((posS2 - i) >= 0)
//                ecg3[(posS2 - i)] = 10000;
//        }
//
//        if (posS2!=0)
//        {
//            for (d = 0; d < 100; d++) {
//                if (IIIrd_Minima > ecg3[d] && d != posS1 && d != posS2 && ecg3[d] != 10000) {
//                    IIIrd_Minima = ecg3[d];
//                    posS3 = d;
//                }
//            }
//
//            if (fabs((ecg2[posS3 - 3] - ecg2[posS3]) / 2) < slopeS ||
//                (fabs((ecg2[posS3] - ecg2[posS3 + 3]) / 2) < slopeS)) {
//                posS3 = 0;
//            }
//        }
//
//    }
//    //else {posS2 =0;posS3=0;}
//    if(max<ecg2[pos1]&&pos1!=0)
//    {
//        max = ecg2[pos1];
//    }
//    if(ecg2[pos1]<max/1.5)
//    {
//        flagpeak1=1;
//    }

    peaks[0] = pos1;
    peaks[1] = pos2;
    peaks[2] = pos3;
    peaks[3] = posS1;
    peaks[4] = posS2;
    peaks[5] = posS3;
}




void checkClass();
//int qs();

long int sum =0,base=0;
double PQRSSampl3[WINDOW_LENGTH],durPR;
int MnC[40];
double ampP=0.0,ampQ=0.0,ampR=0.0,ampS=0.0,ampT=0.0, durP =0.0,temp=0,durQ =0.0,durR =0.0, durS =0.0,durT =0.0,jpVal=0.0;
int endP,endS,endQ,pos_T2=0,startQ,baseline=0,endR,t_invert = 0,pr,p_invert=0,horSt=0,jp=0,jpElev = 0,jpDep =0,downSt=0,upSt=0,ST,br =0,StEle = 0,StEleVal = 0,dpt=0,angle=0,sp=0;

//
// Function to find the least of 3 positions
int leasts(int a,int b,int c)
{
    if(a<b&&a<c)
        return a;
    if(b<a&&b<c)
        return b;
    if(c<a&&c<b)
        return c;

    return 0;
}
int highest(int a,int b,int c)
{
    if(a>b&&a>c)
        return a;
    if(b>a&&b>c)
        return b;
    if(c>a&&c>b)
        return c;

    return 0;
}
// Function to change to seconds
double toSeconds(int time)
{
    if(time==0)
        return 0;
    double RRPeakVal = (double)time*1/200;
    return  RRPeakVal;
}

// Function to change to mm
double tomm(double time)
{
    if(time==0)
        return 0;
    double RRPeakVal = (double)time*0.018;
    return  RRPeakVal;
}

JNIEXPORT jint JNICALL Java_com_agatsa_sanket_pdf_FileOperationsSingle_MinnesotaChecker(JNIEnv *env, jobject obj, jint ecgSample){

    if (length>=WINDOW_LENGTH)
        length=0;
    else
    {
        PQRSSampl1[length]= (double)ecgSample;
        PQRSSampl2[length]= (double)ecgSample;
        PQRSSampl3[length]= (double)ecgSample;
        length +=1;
        return NOTREADY;
    }
    int flag =1;
    for(int i = 0; i < WINDOW_LENGTH; i++)
    {
        if(PQRSSampl1[i] != PQRSSampl1[i + 1])
            flag=0;
    }
    if(flag==1)
    {
        return 821;
        exit;
    }
    int PQRSLeft_Minima  = 10000,PQRSRight_Minima = 10000,PQRSLeft_Maxima  = 10000,PQRSRight_Maxima = 0,Ist_Maxima= 10000,IInd_Maxima = 10000,IIIrd_Maxima= 10000;
    int flagnotpresent = 0,pos_Q2 = 0,pos_S2=0,pos_P2= 0,pos_P2_start=0,pos_T2_end=0,pos1= 0,pos2=0,pos3=0,d=0,e=0,f=0,g = 0,h= 0,i=0,l=0,start =0,startT=0;
    Ist_Maxima = PQRSSampl1[0];
    for ( d = 0; d < WINDOW_LENGTH; d++)
    {
        if (Ist_Maxima < PQRSSampl1[d])
        {
            Ist_Maxima = PQRSSampl1[d];
            pos1 = d;
            /*  //////////////printf("\nValues for maxima %d  and position  %d\n",Ist_Maxima,pos1);*/
        }
    }
    pos1Value = PQRSSampl1[pos1];
    //printf("\nTesting the maxima  %lf at %d \n \n",pos1Value, pos1);
    for(i=1;i<=7;i++)
    {
        if (( pos1+i) < WINDOW_LENGTH)
            PQRSSampl1[( pos1+i)] = 10000;
        if (( pos1-i) >= 0)
            PQRSSampl1[( pos1-i)] = 10000;
    }
    IInd_Maxima=-10000;

    for ( d = 0; d < WINDOW_LENGTH; d++)
    {
        if (IInd_Maxima < PQRSSampl1[d]&&d!=pos1&&PQRSSampl1[d]!=10000)
        {
            // //printf("IInd_Maxima finding %d  \n\n",IInd_Maxima);
            IInd_Maxima = PQRSSampl1[d];
            pos2 = d;
            ////printf("d %d\n",d);
        }
    }
    //printf("\n Testing the 2nd maxima %lf at %d\n ",PQRSSampl1[pos2],pos2);
    pos2Value = PQRSSampl1[pos2];
    PQRSSampl1[pos2]=10000;
    for(i=1;i<=7;i++)
    {
        if (( pos2+i) < WINDOW_LENGTH)
            PQRSSampl1[( pos2+i)] = 10000;
        if (( pos2-i) >= 0)
            PQRSSampl1[( pos2-i)] = 10000;
    }
    IIIrd_Maxima = -10000;
    for ( d = 0; d < WINDOW_LENGTH; d++)
    {
        if (IIIrd_Maxima <= PQRSSampl1[d] && d!=pos1 && d!=pos2 && PQRSSampl1[d]!=10000)
        {
            IIIrd_Maxima = PQRSSampl1[d];
            pos3 = d;
            //   //printf("d %d\n",d);
        }
    }
// Update the location in which Maxima was found with 100
    pos3Value = PQRSSampl1[pos3];
    //printf("\n Testing the 3rd maxima %lf at %d\n ",PQRSSampl1[pos3],pos3);
    PQRSSampl1[pos3]  = 10000;
    for(i=1;i<=6;i++)
    {
        if (( pos3+i) < WINDOW_LENGTH)
            PQRSSampl1[( pos3+i)] = 10000;
        if (( pos3-i) >= 0)
            PQRSSampl1[( pos3-i)] = 10000;
    }
    int lowest = least(pos1, pos2, pos3);
//  //printf("position of 1st R   %d    \n",lowest);
    int high = highest(pos1, pos2, pos3);
    int nextLowest = 0;
    if(lowest == pos1)
    {
        if(pos2 > pos3)
            nextLowest = pos3;
        else
            nextLowest = pos2;
    }else if(lowest == pos2)
    {
        if(pos1 > pos3)
            nextLowest = pos3;
        else
            nextLowest = pos1;
    }
    else
    {
        if(pos1 > pos2)
            nextLowest = pos2;
        else
            nextLowest = pos1;
    }
    if((nextLowest - lowest) > 40)
    {
        //printf("\n More than one R-Peaks are obtained in the sample \n");
    }
    else
    {
        if(PQRSSampl2[lowest]<PQRSSampl2[nextLowest])
        {
            for(i=1;i<=7;i++)
            {
                if (( lowest+i) < WINDOW_LENGTH)
                    PQRSSampl1[( lowest+i)] = PQRSSampl2[( lowest+i)];
                if (( lowest-i) >= 0)
                    PQRSSampl1[( lowest-i)] = PQRSSampl2[( lowest-i)];
            }
            lowest=nextLowest;
            nextLowest=high;
        }
    }
    pos2 = nextLowest;
    pos1 = lowest;
    if(pos1<50)
    {
        //printf("\n 1st R wave is not codeable . \n");
        if(pos1<=10)
            start=lowest*3;
        else if(pos1<=15)
            start =lowest*2;
        else
            start= lowest+lowest/2;
        pos1=pos2;
        pos2=high;
    }else start = pos1/2;

    if(pos1>250)
    {
        start = pos1-start*2;
    }
    else if(pos1>200)
    {
        if(lowest <15)
            start = pos1-start*4;
        else if(pos1==lowest)
            start= pos1/1.5;
        else
            start = pos1-start*2;
    }
    else if(pos1>130)
    {
        if(lowest <5)
            start = pos1-start*5;
        else if(lowest <15)
            start = pos1-start*2.5;
        else if(lowest == pos1)
            start = lowest/1.5;
        else if(lowest>45)
            start = pos1-start;
        else
            start = pos1-start*2;
    }
    else if(pos1-lowest<=130&&pos1-lowest!=0)
    {
        start =((int)(pos1-lowest)/2)+5;
    }

    //printf("\n\nPosition of coded R   %d and start %d and lowest R %d\n\n",pos1,start,lowest);
    if(PQRSSampl2[pos1]-PQRSSampl2[pos1-9]<=200||PQRSSampl2[pos1]-PQRSSampl2[pos1+9]<=200)
    {
        br =1;
        if(PQRSSampl2[pos1]-PQRSSampl2[pos1-9]>=200)
        {
            for(i=1;i<=7;i++)
            {
                if (( pos1-7+i) < WINDOW_LENGTH)
                    PQRSSampl1[( pos1-7+i)] = 10000;
                if (( pos1-7-i) >= 0)
                    PQRSSampl1[(pos1-7-i)] = 10000;
            }
        }else
        {
            for(i=1;i<=7;i++)
            {
                if (( pos1+7+i) < WINDOW_LENGTH)
                    PQRSSampl1[( pos1+7+i)] = 10000;
                if (( pos1+7-i) >= 0)
                    PQRSSampl1[(pos1+7-i)] = 10000;
            }
        }
    }
    // if (flagnotpresent == 1)
    {
// //printf("\n%d  start \n",start);
        PQRSLeft_Maxima = -10000;
        for (f = start; f < pos1; f++)
        {
            if (PQRSLeft_Maxima < PQRSSampl1[f]&&PQRSSampl1[f]!=10000)
            {
                PQRSLeft_Maxima = PQRSSampl2[f];
                pos_P2 = f;
                //printf("%d  1st p  %lf\n",pos_P2,PQRSSampl1[f]);
            }
        }
        for(i=1;i<=4;i++)
        {
            if (( pos_P2+i) < WINDOW_LENGTH)
                PQRSSampl1[( pos_P2+i)] = 10000;
            if (( pos_P2-i) >= 0)
                PQRSSampl1[( pos_P2-i)] = 10000;
        }
        if((pos1-pos_P2)-(pos_P2-start)>35)
        {
            start = pos_P2+1;
            PQRSLeft_Maxima = -10000;
            for (f = start; f < pos1; f++)
            {
                if (PQRSLeft_Maxima <= PQRSSampl1[f]&&PQRSSampl1[f]!=10000)
                {
                    PQRSLeft_Maxima = PQRSSampl2[f];
                    pos_P2 = f;
                    ////printf("%d  1st p recalculate\n",pos_P2);
                }
            }
        }
        PQRSLeft_Minima =PQRSSampl2[pos1];
        PQRSRight_Minima = PQRSSampl2[pos1];
        for (e = pos_P2; e < pos1; e++)
        {
            if (PQRSLeft_Minima >= PQRSSampl1[e]&&PQRSSampl1[e]!=10000)
            {
                PQRSLeft_Minima = PQRSSampl2[e];
                pos_Q2 = e;
            }
        }
        for(i=1;i<=7;i++)
        {
            if (( pos_Q2+i) < WINDOW_LENGTH)
                PQRSSampl1[( pos_Q2+i)] = 10000;
            if (( pos_Q2-i) >= 0)
                PQRSSampl1[( pos_Q2-i)] = 10000;
        }
        if(pos_P2==pos_Q2)
        {
            //printf("\n A possible p inversion detection .\n");
            p_invert=1;
            for(i=1;i<=15;i++)
            {
                if (( pos_P2+i) < WINDOW_LENGTH)
                    PQRSSampl1[( pos_P2+i)] = PQRSSampl2[( pos_P2+i)];
                if (( pos_P2-i) >= 0)
                    PQRSSampl1[( pos_P2-i)] = PQRSSampl2[( pos_P2+i)];
            }
            //  //printf("\n checking 3 array %lf %lf %lf at %d\n",PQRSSampl1[i],PQRSSampl2[i],PQRSSampl3[i],i);
            PQRSLeft_Maxima = 10000;
            for (f = start; f < pos1; f++)
            {
                //	//printf("\n%lf  p values at %d \n",PQRSSampl1[f],f);
                if (PQRSLeft_Maxima > PQRSSampl1[f]&&PQRSSampl1[f]!=10000)
                {
                    PQRSLeft_Maxima = PQRSSampl2[f];
                    pos_P2 = f;
                    //  //printf("%d  1st p  %lf\n",pos_P2,PQRSSampl1[f]);
                }
            }
            for(i=1;i<=15;i++)
            {
                if (( pos_Q2+i) < WINDOW_LENGTH)
                    PQRSSampl1[( pos_Q2+i)] = PQRSSampl2[( pos_Q2+i)];
                if (( pos_Q2-i) >= 0)
                    PQRSSampl1[( pos_Q2-i)] = PQRSSampl2[( pos_Q2+i)];
            }

            for(i=1;i<=5;i++)
            {
                if (( pos_P2+i) < WINDOW_LENGTH)
                    PQRSSampl1[( pos_P2+i)] = 10000;
                if (( pos_P2-i) >= 0)
                    PQRSSampl1[( pos_P2-i)] = 10000;
            }
            PQRSLeft_Minima =PQRSSampl2[pos1];
            PQRSRight_Minima = PQRSSampl2[pos1];
            for (e = pos_P2; e < pos1; e++)
            {
                if (PQRSLeft_Minima >= PQRSSampl1[e]&&PQRSSampl1[e]!=10000&&e!=pos_P2)
                {
                    PQRSLeft_Minima = PQRSSampl2[e];
                    pos_Q2 = e;
                }
            }
            for(i=1;i<=15;i++)
            {
                if (( pos_Q2+i) < WINDOW_LENGTH)
                    PQRSSampl1[( pos_Q2+i)] = 10000;
                if (( pos_Q2-i) >= 0)
                    PQRSSampl1[( pos_Q2-i)] = 10000;
            }
        }
        start = pos_P2-(pos_Q2-pos_P2+3);
        if(pos_Q2-pos_P2<10)
            start-=10;
        if(pos_Q2-pos_P2<15)
            start-=5;

        if(start<0)
            start=0;
        PQRSLeft_Minima = PQRSSampl1[start];
        pos_P2_start=start;
        ////printf("\nchecking \t %d\t%lf\n\n",start,PQRSSampl1[start]);
        for (h = start; h < pos_P2; h++)
        {
            if (PQRSLeft_Minima>= PQRSSampl1[h]&&PQRSLeft_Minima>=PQRSSampl1[pos_Q2])
            {
                PQRSLeft_Minima = PQRSSampl1[h];
                pos_P2_start = h;
            }
        }
        // 	//printf("\nStart of p %d   %lf\n",pos_P2_start,PQRSSampl1[pos_P2_start]);
        //  //printf("\nchecking \t %d\t%d\n",pos1+(pos1-pos_Q2),pos2-(pos1-pos_P2_start));

        PQRSRight_Maxima = PQRSSampl1[pos_Q2];
        for (g = pos1+(pos1-pos_Q2); g < pos2-(pos1-pos_P2_start); g=g+1)
        {
            if (PQRSRight_Maxima < PQRSSampl1[g]&&PQRSSampl1[g]!=10000)
            {
                PQRSRight_Maxima = PQRSSampl1[g];
                pos_T2 = g;
                // //printf("%d 1st T\t%lf\n",pos_T2,PQRSSampl1[g]);
            }
        }////printf("\n%d\t 1st T %lf\n",pos_T2,PQRSSampl1[pos_T2]);

// If p has more amplitude than t-wave
        PQRSRight_Maxima = -10000;
        if((pos_T2-pos1)-(pos2-pos_T2)>40)
        {
            temp = pos_T2;
            for (g = pos1+(pos1-pos_Q2); g < temp; g=g+1)
            {
                if (PQRSRight_Maxima <= PQRSSampl1[g]&&PQRSSampl1[g]!=10000)
                {
                    PQRSRight_Maxima = PQRSSampl1[g];
                    pos_T2 = g;
                    // //printf("%d 1st T checking\t%d\n",pos_T2,PQRSSampl1[g]);
                }
            }////printf("%d\t 1st t\n",pos_T2);
        }
        for(i=1;i<=4;i++)
        {
            if (( pos_T2+i) < WINDOW_LENGTH)
                PQRSSampl1[( pos_T2+i)] = 10000;
            if (( pos_T2-i) >= 0)
                PQRSSampl1[( pos_T2-i)] = 10000;
        }
        PQRSRight_Maxima = -10000;
        if((pos1-pos_P2)-(pos1-pos_T2)>100)
        {
            temp = pos_T2;
            for (g = pos1+(pos1-pos_Q2); g < temp; g=g+1)
            {
                if (PQRSRight_Maxima <= PQRSSampl1[g]&&PQRSSampl1[g]!=10000)
                {
                    PQRSRight_Maxima = PQRSSampl1[g];
                    pos_T2 = g;
                    ////printf("%d 1st T checking\t%lf\n",pos_T2,PQRSSampl1[g]);
                }
            }////printf("%d\t 1st t\n",pos_T2);
        }

        PQRSRight_Maxima = PQRSSampl1[0];
        for(i=1;i<=7;i++)
        {
            if (( pos_T2+i) < WINDOW_LENGTH)
                PQRSSampl1[( pos_T2+i)] = 10000;
            if (( pos_T2-i) >= 0)
                PQRSSampl1[( pos_T2-i)] = 10000;
        }

        for (e = pos1; e < pos_T2; e++)
        {
            if(PQRSRight_Minima > PQRSSampl1[e]&&PQRSSampl1[e]!=10000)
            {
                PQRSRight_Minima =PQRSSampl2[e];
                pos_S2 = e;
                // //printf("%d 1st S\t%lf\n",pos_S2,PQRSSampl1[e]);
            }
        }////printf("%d\t 1st S\n",pos_S2);
        if((pos_T2-pos_S2)-(pos1-pos_P2)>85)
        {
            int end = pos_T2-(pos1-pos_P2_start);
            PQRSRight_Maxima=-10000;
            for (g = pos1+(pos1-pos_Q2); g < end; g=g+1)
            {
                if(PQRSRight_Maxima < PQRSSampl1[g]&&PQRSSampl1[g]!=10000)
                {
                    PQRSRight_Maxima = PQRSSampl1[g];
                    pos_T2 = g;
                    // //printf("%d 1st T\t%lf\n",pos_T2,PQRSSampl1[g]);
                }
            }////printf("%d\t 1st T\n",pos_T2);
        }

        PQRSRight_Maxima = PQRSSampl1[pos_S2];
        PQRSLeft_Minima = PQRSSampl1[pos_P2];
// After, this find the positions of Start of P and End of T Segment, by finding the Minima Points

        for(i=1;i<=7;i++)
        {
            if (( pos_S2+i) < WINDOW_LENGTH)
                PQRSSampl1[( pos_S2+i)] = 10000;
            if (( pos_S2-i) >= 0)
                PQRSSampl1[( pos_S2-i)] = 10000;
        }
        PQRSRight_Minima = PQRSSampl1[pos1];

        if(PQRSSampl1[pos_T2]<=PQRSSampl1[pos_P2_start]||((PQRSSampl1[pos_T2]-PQRSSampl1[pos_P2_start]<3)&&(PQRSSampl1[pos1]-PQRSSampl1[pos_P2]<200))||(((PQRSSampl1[pos_T2]-PQRSSampl1[pos_P2_start])<80&&(PQRSSampl2[pos1]-PQRSSampl2[pos_P2]>450)))||PQRSSampl1[pos_T2]-PQRSSampl1[pos_P2_start]<50)
        {
            if((PQRSSampl1[pos_T2]-PQRSSampl1[pos_P2_start]<45)&&(PQRSSampl2[pos1]-PQRSSampl2[pos_P2]<250))
            {
                goto jmp;
            }
            t_invert = 1;
            int end=pos2-(pos1-pos_P2_start);
            if(pos2-(pos1-pos_P2_start)<pos1+(pos1-pos_Q2))
                end = high-(pos1-pos_P2_start);
            PQRSRight_Minima=PQRSSampl1[pos1+(pos1-pos_Q2)];

            for (g = pos1+(pos1-pos_Q2)+3; g < end; g=g+1)
            {
                if(PQRSRight_Minima > PQRSSampl1[g]&&PQRSSampl1[g]!=10000)
                {
                    PQRSRight_Minima = PQRSSampl1[g];
                    pos_T2 = g;
                    ////printf("%d 1st T\t%lf\n",pos_T2,PQRSSampl1[g]);
                }
            }
        }
        jmp:  if(t_invert == 0)
    {
        int mini=PQRSSampl1[pos_T2],mn=pos_T2;
        for(i = pos_S2+1;i<pos_T2;i++)
        {
            if(mini>PQRSSampl1[i]&&PQRSSampl1[i]!=10000)
            {
                mini=PQRSSampl1[i];
                mn=i;
            }
        }
        if(fabs(PQRSSampl2[pos_S2]-mini)<=80&&pos_S2-mn>15)
        {
            t_invert=1;
            dpt=1;
            pos_T2=mn;
        }
        //	//printf("\n Checking the value of mini %d at position %d .\n",mini,mn);
    }
        if(t_invert == 1)
        {
            for(i=1;i<=7;i++)
            {
                if (( pos_S2+i) < WINDOW_LENGTH)
                    PQRSSampl1[( pos_S2+i)] = PQRSSampl3[( pos_S2+i)];
                if (( pos_S2-i) >= 0)
                    PQRSSampl1[( pos_S2-i)] = PQRSSampl3[( pos_S2+i)];
            }
        }
//	//printf("\nStart of p %d   %lf\n",pos_P2_start,PQRSSampl1[pos_P2_start]);
//	//printf("\n T invert  %d\n",t_invert);
        PQRSRight_Minima=PQRSSampl1[pos1];

        if(t_invert == 1)
        {
            for (e = pos1+5; e < pos_T2; e++)
            {
                if (PQRSRight_Minima > PQRSSampl1[e]&&PQRSSampl1[e]!=10000)
                {
                    ////printf("%d 1st S\t%lf\n",e,PQRSSampl1[e]);
                    PQRSRight_Minima =PQRSSampl2[e];
                    pos_S2 = e;
                }
                if (PQRSRight_Minima > PQRSSampl1[e]&&PQRSSampl1[e]!=10000&&PQRSSampl1[e-2]!=10000&&((PQRSSampl1[e]-PQRSSampl1[e-2])/2)-(PQRSSampl1[e+2]-PQRSSampl1[e])/2>10)
                {
                    //	//printf("%d 1st S\t%lf\n",e,PQRSSampl1[e]);
                    PQRSRight_Minima =PQRSSampl2[e];
                    pos_S2 = e;
                    break;
                }
            }
        }
        if(pos_S2==0||pos_T2-pos_S2<5)
        {
            t_invert = 1;sp=1;
            for(i=1;i<=7;i++)
            {
                if (( pos_T2+i) < WINDOW_LENGTH)
                    PQRSSampl1[( pos_T2+i)] = PQRSSampl2[( pos_T2+i)];
                if (( pos_T2-i) >= 0)
                    PQRSSampl1[( pos_T2-i)] = PQRSSampl2[( pos_T2+i)];
            }
            int end=pos2-(pos1-pos_P2_start);
            for (g = pos1+(pos1-pos_Q2)+3; g < end; g=g+1)
            {
                if(PQRSRight_Minima > PQRSSampl1[g]&&PQRSSampl1[g]!=10000)
                {
                    PQRSRight_Minima = PQRSSampl1[g];
                    pos_T2 = g;
                    //  //printf("%d 1st T\t%lf\n",pos_T2,PQRSSampl1[g]);
                }
            }
            for(i=1;i<=7;i++)
            {
                if (( pos_S2+i) < WINDOW_LENGTH)
                    PQRSSampl1[( pos_S2+i)] = PQRSSampl3[( pos_S2+i)];
                if (( pos_S2-i) >= 0)
                    PQRSSampl1[( pos_S2-i)] = PQRSSampl3[( pos_S2+i)];
            }
            PQRSRight_Minima=10000;
            for (e = pos1+5; e < pos_T2; e++)
            {
                if (PQRSRight_Minima > PQRSSampl1[e]&&PQRSSampl1[e]!=10000&&((PQRSSampl1[e+2]-PQRSSampl1[e])/2)>(PQRSSampl1[e+4]-PQRSSampl1[e+2])/2)
                {
                    ////printf("%d 1st S\t%lf\n",e,PQRSSampl1[e]);
                    PQRSRight_Minima =PQRSSampl2[e];
                    pos_S2 = e;
                    break;
                }
            }
        }
        PQRSRight_Minima=10000;
        if(t_invert==0)
        {
            for (i = pos_T2+5; i < pos_T2+(pos_T2-pos1); i++)
            {
                if ((PQRSRight_Minima > PQRSSampl1[i]&&PQRSSampl1[i]!=10000)||((PQRSSampl1[i]-PQRSSampl1[i-2])/2)>((PQRSSampl1[i+2]-PQRSSampl1[i])/2)&&PQRSSampl1[i]!=10000&&PQRSSampl1[i+2]!=10000&&PQRSSampl1[i-2]!=10000)
                {
                    if((((PQRSSampl2[i]-PQRSSampl2[i-2])/2)-((PQRSSampl2[i+2]-PQRSSampl2[i])/2)<-8)&&PQRSSampl1[i]!=100000&&fabs((PQRSSampl2[i]-PQRSSampl2[pos_P2_start]))<100)
                    {
                        PQRSRight_Minima = PQRSSampl1[i];
                        pos_T2_end = i;
                        break;
                    }
                    PQRSRight_Minima = PQRSSampl1[i];
                    pos_T2_end = i;
                    ////printf("%d\t1st end t %lf\n",pos_T2_end,PQRSSampl1[i]);
                }
            }
        }else
        {
            PQRSRight_Minima= PQRSSampl1[pos_T2];
            for (i = pos_T2; i < pos_T2+(pos_T2-pos1); i++)
            {
                if ((PQRSRight_Minima < PQRSSampl1[i]&&PQRSSampl1[i]!=10000&&PQRSSampl1[i]<PQRSSampl1[baseline]))
                {
                    PQRSRight_Minima = PQRSSampl1[i];
                    pos_T2_end = i;
                    //  //printf("%d\t1st end t\n",pos_T2_end);
                }
                if(((PQRSSampl2[i]-PQRSSampl2[i-3])/3)>((PQRSSampl2[i+3]-PQRSSampl2[i])/3))
                {
                    pos_T2_end = i;
                    break;
                }
            }
        }
    }

    {
        //printf("\n Peak 	Position	Value\n");
        //printf("\n P		%d	%lf\n ",pos_P2,PQRSSampl2[pos_P2]);
        //printf("\n Q		%d	%lf\n",pos_Q2,PQRSSampl2[pos_Q2]);
        //printf("\n R		%d	%lf\n",pos1,PQRSSampl2[pos1]);
        //printf("\n S		%d    %lf\n",pos_S2,PQRSSampl2[pos_S2]);
        //printf("\n T		%d	%lf\n",pos_T2,PQRSSampl1[pos_T2]);
        //printf("\nStart of p %d   %lf\n",pos_P2_start,PQRSSampl1[pos_P2_start]);
        //printf("\nEnd of t  %d	%lf\n",pos_T2_end,PQRSSampl1[pos_T2_end]);
        baseline = pos_P2_start;
        //printf("\nBaseLine %d   %lf\n",baseline,PQRSSampl1[baseline]);
    }
//=================================================//
// Ok, now, Calculate the P-Q, Q-R, R-S And S-T

    ampP=(double)(PQRSSampl2[pos_P2]-PQRSSampl2[baseline]);
    ampP=tomm(ampP);
    ampQ=(double)(PQRSSampl2[baseline]-PQRSSampl2[pos_Q2]);
    if(ampQ<0)
        ampQ=0;
    ampQ=tomm(ampQ);
    ampR=(double)(PQRSSampl2[pos1]-PQRSSampl2[baseline]);
    ampR=tomm(ampR);
    ampS=(double)(PQRSSampl2[baseline]-PQRSSampl2[pos_S2]);
    if(ampS<0)
    {
        StEle = 1;
        ampS=-ampS;
        //printf("\nST Elevation Detected . \n");
    }
    ampS=tomm(ampS);
    if(t_invert==0)
        ampT=(double)(PQRSSampl2[pos_T2]-PQRSSampl2[baseline]);
    else
    {
        //printf("\n \nT Inversion detected .\n\n");
        ampT=(double)(fabs(PQRSSampl2[pos_T2]-PQRSSampl2[pos_Q2]));
    }
    ampT=tomm(ampT);

    angle = ampR-(ampQ>ampS?ampQ:ampS);
    //printf("\n Y  axis values \n ");
    for(i=1;i<=7;i++)
    {
        if (( pos_T2+i) < WINDOW_LENGTH)
            PQRSSampl1[( pos_T2+i)] = 10000;
        if (( pos_T2-i) >= 0)
            PQRSSampl1[( pos_T2-i)] = 10000;
    }
    endP = pos_Q2-1;
    for(i=pos_P2;i<=pos_Q2;i++)
    {
        if(PQRSSampl2[i]==PQRSSampl2[baseline])//||(PQRSSampl2[i]-PQRSSampl2[i-2])/2 < (PQRSSampl2[i+2]-PQRSSampl2[i])/2)
        {
            endP = i;
            break;
        }
        if((PQRSSampl2[i]-PQRSSampl2[i-2])/2 < (PQRSSampl2[i+2]-PQRSSampl2[i])/2&&PQRSSampl2[i]<PQRSSampl2[baseline])
        {
            endP = i;
            break;
        }
        if(PQRSSampl2[i]<PQRSSampl2[baseline])
        {
            endP = i;
            break;
        }
    }
    //printf("\n%d\tco-ord of P\t%d\n",pos_P2_start,endP);
    durP = (double)(endP-pos_P2_start);
    //printf("\n P 	\t	%lf \n ",durP);
    durP=toSeconds(durP);

    endQ=pos1;
    for(i=pos1;i>endP;i--)
    {
        if(PQRSSampl2[i]==PQRSSampl2[baseline]||(PQRSSampl2[i]-PQRSSampl2[i-2])/2 < (PQRSSampl2[i+2]-PQRSSampl2[i])/2)
        {
            endQ=i;
            break;
        }
        if(PQRSSampl2[i]<PQRSSampl2[baseline])
        {
            endQ=i+1;
            break;
        }
    }
    startQ=pos_Q2-1;
    for(i=pos_Q2-2;i>endP;i--)
    {
        if(PQRSSampl2[i]==PQRSSampl2[baseline]&&PQRSSampl1[i]!=10000||(PQRSSampl2[i]-PQRSSampl2[i-2])/2 < (PQRSSampl2[i+2]-PQRSSampl2[i])/2)
        {
            startQ=i;
            break;
        }
        if(PQRSSampl2[i]>PQRSSampl2[baseline]&&PQRSSampl1[i]!=10000||(PQRSSampl2[i]-PQRSSampl2[i-2])/2 < (PQRSSampl2[i+2]-PQRSSampl2[i])/2)
        {
            startQ=i-1;
            break;
        }
    }
    //printf("\n%d\tco-ord of Q\t%d\n",startQ,endQ);
    durQ=(double)(endQ-startQ);
    //printf("\n Q 	\t	%lf \n ",durQ);
    durQ=toSeconds(durQ);
    endR=pos_S2-1;
    for(i=pos1;i<pos_S2;i++)
    {
        if(PQRSSampl2[i]==PQRSSampl2[baseline]||PQRSSampl2[i]<PQRSSampl2[baseline])
        {
            endR=i;
            break;
        }
    }
    //printf("\n%d\tco-ord of R\t%d\n",endQ,endR);
    durR=(double)(endR-endQ);
    //printf("\n R 	\t	%lf \n ",durR);
    durR=toSeconds(durR);
    for(i=pos_S2+2;i<pos_T2;i++)
    {
        if((PQRSSampl2[i]==PQRSSampl2[baseline]||PQRSSampl2[i]>PQRSSampl2[baseline])&&(PQRSSampl2[i]-PQRSSampl2[i-2])/2 < (PQRSSampl2[i+2]-PQRSSampl2[i])/2||(((PQRSSampl2[i-2]-PQRSSampl2[i])/2) ==0&& ((PQRSSampl2[i]>PQRSSampl2[i+2])/2)==0))
        {
            if(((PQRSSampl2[i-2]-PQRSSampl2[i])/2) ==0&& ((PQRSSampl2[i]>PQRSSampl2[i+2])/2)==0)
                horSt=1;

            if(PQRSSampl2[i]>PQRSSampl2[baseline])
            {
                endS=i-1;
                jp=endS;
                break;
            }
            if(PQRSSampl2[i]<PQRSSampl2[baseline]&&(PQRSSampl2[i]-PQRSSampl2[i-2])/2 > (PQRSSampl2[i+2]-PQRSSampl2[i])/2)
            {
                endS=i-1;
                jp=endS;
                break;
            }
            if((PQRSSampl2[i-2]-PQRSSampl2[i])/2 > (PQRSSampl2[i]-PQRSSampl2[i+2])/2&&t_invert==1)
            {
                endS=i;
                jp=endS;
                break;
            }else
                continue;
            endS=i;
            jp=endS;
            break;
        }
    }
    //printf("\n%d\tco-ord of S\t%d\n",endR,endS);
    durS=(double)(endS-endR);
    //printf("\n S 	\t	%lf \n ",durS);
    durS=toSeconds(durS);
    if(t_invert==0)
    {
        for(i=endS;i<pos_T2;i++)
        {
            if(PQRSSampl2[i]==PQRSSampl2[baseline]||PQRSSampl2[i]>PQRSSampl2[baseline]||(PQRSSampl1[i]-PQRSSampl1[i-2])/2>(PQRSSampl1[i+2]-PQRSSampl1[i])/2)
            {
                startT=i;
                break;
            }
        }
    }else
    {
        if(sp==1)
        {
            for(i=endS;i<pos_T2;i++)
            {
                if(PQRSSampl2[i]==PQRSSampl2[baseline]||PQRSSampl2[i]<PQRSSampl2[baseline]||(PQRSSampl1[i+2]-PQRSSampl1[i])/2>(PQRSSampl1[i+4]-PQRSSampl1[i+2])/2)
                {
                    startT=i;
                    break;
                }
            }
        }else
            for(i=pos_T2-2;i>endS;i--)
            {
                //	//printf("\n Checking the loop %lf at %d\n",PQRSSampl2[i],i);
                if(PQRSSampl2[i]==PQRSSampl2[baseline]||PQRSSampl2[i]>PQRSSampl2[baseline]||(PQRSSampl1[i]-PQRSSampl1[i-2])/2<(PQRSSampl1[i+2]-PQRSSampl1[i])/2&&PQRSSampl1[i-2]!=10000)
                {
                    startT=i;
                    break;
                }
            }
    }
    if(startT==endS)
    {
        ST=pos_T2;
    }else
    {
        ST=startT;
    }
    //printf("\n%d\tco-ord of T\t%d\n",startT,pos_T2_end);
    durT=(double)(pos_T2_end-startT);
    //printf("\n T 	\t	%lf \n ",durT);
    durT=toSeconds(durT);

    //printf("\n Peak \t Amplitude\n");
    //printf("\n P \t %lf\n",ampP);
    //printf("\n Q \t %lf\n",ampQ);
    //printf("\n R \t %lf\n",ampR);
    //printf("\n S \t %lf\n",ampS);
    //printf("\n T \t %lf\n",ampT);

    //printf("\n Peak \t	Duration \n ");
    //printf("\n P 	\t	%lf  ",durP);
    //printf("\n Q 	\t	%lf  ",durQ);
    //printf("\n R 	\t	%lf  ",durR);
    //printf("\n S 	\t	%lf  ",durS);
    //printf("\n T 	\t	%lf  ",durT);
    //printf("\n\n j point\t	%d  \n ",jp);
    pr=startQ-pos_P2_start;
    durPR=toSeconds(pr);
    if(PQRSSampl2[baseline]-PQRSSampl2[jp]<0)
    {
        //printf("\n jp elevation 	\t	%lf  ",tomm(-(PQRSSampl2[startQ]-PQRSSampl2[jp])));
        jpElev = 1;
        jpVal=tomm(-(PQRSSampl2[startQ]-PQRSSampl2[jp]));
    }
    else
    {
        //printf("\n jp depression 	\t	%lf  ",tomm((PQRSSampl2[startQ]-PQRSSampl2[jp])));
        jpDep = 1;
        jpVal=tomm((PQRSSampl2[startQ]-PQRSSampl2[jp]));
    }
    if((PQRSSampl2[ST]-PQRSSampl2[jp])<0)
    {
        downSt=1;
        //printf("\n Downward Sloping ST segment .\n");
    }else if((PQRSSampl2[pos_T2]-PQRSSampl2[jp])>0)
    {
        upSt=1;
        //printf("\n Upward Sloping ST segment .\n");
    }else
    {
        horSt=1;
        //printf("\n Horizontal ST segment .\n");
    }
    checkClass();


    return 0;
}

JNIEXPORT jintArray JNICALL Java_com_agatsa_sanket_pdf_FileOperationsSingle_getMinneSotaCodeArray(JNIEnv *env, jobject obj){


    jintArray MinneSota_data = (*env)->NewIntArray(env,37);
    (*env)->SetIntArrayRegion(env, MinneSota_data, 0, 37, MnC);

    return MinneSota_data;
}
int checker_1_1_1()
{
    if(ampR!=0)
    {
        double ampQ_R =ampQ/ampR;
        if(ampQ_R>=0.333333&&durQ>=0.03)
            return 1;
    }
    return 0;
}

int checker_1_1_2()
{
    int i;
    if(ampR!=0)
    {
        double ampQ_R =ampQ/ampR;
        if(ampQ_R>=.1666666667&&durQ>=0.04)
            return 1;
    }
    return 0;
}

int checker_1_1_3()
{
    int i;
    if(ampQ>=1.0&&ampR>=3.0&&durQ>=0.04)
        return 1;
    return 0;
}

int checker_1_1_4(int a)
{
    switch(a)
    {
        case 1:
            if(ampQ>=1.0&&durQ>=0.05)
                return 1;
            else
                return 0;
        case 2:
            if(ampQ>=1)
                return 1;
            else
                return 0;
    }
    return 0;
}

int checker_1_1_5()
{
    if(ampQ>=1&&durQ>=0.05)
        return 1;
    return 0;
}

int checker_1_2_1( )
{
    if(ampR!=0)
    {
        double ampQ_R = ampQ/ampR;
        if(ampQ_R>=.03333333&&0.02<=durQ<=0.05&&ampQ>1)
            return 1;
    }
    return 0;
}

int checker_1_2_2( )
{
    if(ampR!=0)
    {
        double ampQ_R = ampQ/ampR;
        if(ampQ_R<.3333333&&0.03<=durQ<=0.04)
            return 1;
    }
    return 0;
}

int checker_1_2_4( int a)
{
    switch(a)
    {
        case 1:
            if(ampQ>=1&&0.04<=durQ&&durQ<0.05)
                return 1;
            else
                return 0;
        case 2:
            if(ampQ>=1&&durQ<0.05)
                return 1;
            else
                return 0;
    }
    return 0;
}

int checker_1_2_5( )
{
    if(ampR!=0)
    {
        double ampQ_R = ampQ/ampR;
        if(0.04<=durQ&&durQ<0.05&&ampQ>=1)
            return 1;
    }
    return 0;
}

int checker_1_3_1( )
{
    if(ampR!=0)
    {
        double ampQ_R = ampQ/ampR;
        if(0.333333<=ampQ_R<=.2&&0.02<=durQ<=0.03&&ampQ>=1)
            return 1;
    }
    return 0;
}

int checker_1_3_3( )
{
    if(0.03<=durQ&&durQ<0.04&&ampR>3)
        return 1;
    else
        return 0;
}

int checker_1_3_4( int a)
{
    switch(a)
    {
        case 1:
            if(0.03<=durQ&&durQ<0.04)
                return 1;
            else
                return 0;
        case 2:
            if(ampQ>=1)
                return 1;
            else
                return 0;
    }
    return 0;
}

int checker_1_3_5( )
{
    if(0.03<=durQ&&durQ<0.04&&ampQ>=1)
        return 1;
    else
        return 0;
}
int checker_3_1(int a)
{
    switch(a)
    {
        case 1:
            if(ampR>=26)
                return 1;
            else
                return 0;
        case 2:
            if(ampR>=20)
                return 1;
            else
                return 0;
        case 3:
            if(ampR>=12)
                return 1;
            else
                return 0;
    }
    return 0;
}

int checker_3_2(int a)
{
    switch(a)
    {
        case 1:
            if(ampR>=5&&ampR>=ampS)
                return 1;
            else
                return 0;
        case 2:
            if(ampS>ampR)
                return 1;
            else
                return 0;
    }
    return 0;
}

int checker_3_3(int a)
{
    switch(a)
    {
        case 1:
            if(ampR>15&&ampR<=20)
                return 1;
            else
                return 0;
        case 2:
            if(ampR>=35)
                return 1;
            else
                return 0;
    }

    return 0;
}

int checker_3_4( int a)
{
    switch(a)
    {
        case 1:
            if(ampR>12)
                return 1;
            else
                return 0;
        case 2:
            if(ampR>5&&ampR>ampS)
                return 1;
            else
                return 0;
        case 3:
            if(ampR<ampS)
                return 1;
            else
                return 0;
    }

    return 0;
}
int checker_4_1_1( )
{
    if(jpDep == 1)
    {
        if(jpVal==2&&(horSt==1||downSt==1))
            return 1;
    }
    else
        return 0;

    return 0;
}

int checker_4_1_2( )
{
    if(jpDep == 1)
    {

        if(1<=jpVal&&jpVal<2&&(horSt==1||downSt==1))
            return 1;
    }
    else
        return 0;

    return 0;
}
int checker_4_2( )
{
    if(jpDep == 1)
    {
        if(0.5<=jpVal&&jpVal<1&&(horSt==1||downSt==1))
            return 1;
    }
    else
        return 0;

    return 0;
}

int checker_4_3( )
{
    if(jpDep == 1)
    {
        if(0.5>jpVal&&jpVal<1&&downSt==1&&(PQRSSampl2[baseline]-PQRSSampl2[ST])>0.5)
            return 1;
        else
            return 0;
    }
    else
        return 0;
}

int checker_4_4( )
{
    double jpDep=baseline-jp;
    if(jpDep<0)
        return 0;
    if(jpDep>=1&&(pos_T2-jp)>0&&horSt!=1)
        return 1;
    else
        return 0;
}

int checker_5_1(int a)
{
    switch(a)
    {
        case 1:
            if(ampT>5&&t_invert==1)
                return 1;
            else
                return 0;
        case 2:
            if(ampT>5&&t_invert==1&&ampR>=5)
                return 1;
            else
                return 0;
            if(ampT>5&&t_invert==1&&ampR>=5&&ampR>(ampR+ampS))
                return 1;
            else
                return 0;
    }
    return 0;
}

int checker_5_2(int a)
{
    switch(a)
    {
        case 1:
            if(1<=ampT&&ampT<5&&t_invert==1)
                return 1;
            else
                return 0;
        case 2:
            if(1<=ampT&&ampT<5&&t_invert==1&&ampR>=5)
                return 1;
            else
                return 0;
            if(1<=ampT&&ampT<5&&t_invert==1&&ampR>=5&&ampR>(ampR+ampS))
                return 1;
            else
                return 0;
    }
    return 0;
}
int checker_5_3( )
{
    if(ampT<1&&t_invert==1)
        return 1;
    else
        return 0;
}

int checker_5_4( )
{
    if(ampT/ampR<.05&&ampR>=10  )
        return 1;
    else
        return 0;
}

int checker_6_3( )
{
    if(durPR>= 0.22)
        return 1;
    else
        return 0;
}
int checker_6_4_1( )
{
    if(p_invert==0&&durPR<.12&&durQ+durR+durS>0.12&&durP>0.06)
        return 1;
    else
        return 0;
}
int checker_6_5( )
{
    if(durPR<0.12)
        return 1;
    else
        return 0;
}

int checker_7_1_1( int a)
{
    switch(a)
    {
        case 1:
            if((durR+durQ+durS)>=0.12)
                return 1;
            else
                return 0;
        case 2:
            if(durR>=0.06)
                return 1;
            else
                return 0;
    }
    return 0;
}

int checker_7_2_1(int a)
{
    switch(a)
    {
        case 1:
            if((durR+durQ+durS)<=0.12)
                return 1;
            else
                return 0;
        case 2:
            if((durR>durQ||durR>durS)&&durR>=0.06)
                return 1;
            else
                return 0;
        case 3:
            if(durR<durS)
                return 1;
            else
                return 0;
    }
    return 0;
}
int checker_7_4( )
{
    if(0.12<=(durR+durQ+durS))
        return 1;
    else
        return 0;
}

int checker_7_6( )
{
    if(0.10<=(durR+durQ+durS)&&(durR+durQ+durS)<0.12)
        return 1;
    else
        return 0;
}

int checker_9_1(int a)
{
    switch(a)
    {
        case 1:
            if((ampR+ampQ)<5)
                return 1;
            else
                return 0;
        case 2:
            if((ampR+ampQ)<10)
                return 1;
            else
                return 0;
    }
    return 0;
}
int checker_9_2(int a)
{
    switch(a)
    {
        case 1:
            if(StEle>1&&(upSt==1||horSt==0))
                return 1;
            else
            if(downSt==1)
            {
                int b=ST-jp;
                double bb = toSeconds(b);
                double a = PQRSSampl2[ST]-PQRSSampl2[jp];
                a = tomm(a);
                if(a<0.5&&bb==.08)
                    return 1;
                else
                    return 0;
            }return 0;
        case 2:
            if(StEle>2)
            {
                if(downSt==1)
                {
                    int b=ST-jp;
                    double bb = toSeconds(b);
                    double a = PQRSSampl2[ST]-PQRSSampl2[jp];
                    a = tomm(a);
                    if(a<0.5&&bb==.08)
                        return 1;
                    else
                        return 0;
                }
                return 1;
            }
            else
                return 0;
    }
    return 0;
}

int checker_9_3( )
{
    if(ampP>=2.5)
        return 1;
    else
        return 0;
}

int checker_9_4_1( )
{

    double max = ampQ>ampS?ampQ:ampS;
    if(ampR>max)
        return 1;
    else
        return 0;
}
int checker_9_5( )
{
    if(ampT>12)
        return 1;
    else
        return 0;
}

void checkClass()
{
    if(checker_1_1_1())
        MnC[1]=111;
    else
        MnC[1]=999;
    if(checker_1_1_2())
        MnC[2]=112;
    else
        MnC[2]=999;
    if(checker_1_1_3())
        MnC[3]=113;
    else
        MnC[3]=999;
    if(checker_1_1_4(1))
        MnC[4]=114;
    else
        MnC[4]=999;
    if(checker_1_1_5())
        MnC[5]=115;
    else
        MnC[5]=999;
    if(checker_1_2_1())
        MnC[6]=121;
    else
        MnC[6]=999;
    if(checker_1_2_2())
        MnC[7]=122;
    else
        MnC[7]=999;
    if(checker_1_2_4(1))
        MnC[8]=124;
    else
        MnC[8]=999;
    if(checker_1_2_5())
        MnC[9]=125;
    else
        MnC[9]=999;
    if(checker_1_3_1())
        MnC[10]=131;
    else
        MnC[10]=999;
    if(checker_1_3_3())
        MnC[11]=133;
    else
        MnC[11]=999;
    if(checker_1_3_4(1))
        MnC[12]=134;
    else
        MnC[12]=999;
    if(checker_1_3_5())
        MnC[13]=135;
    else
        MnC[13]=999;
    if(checker_3_1(1))
        MnC[14]=31;
    else
        MnC[14]=999;
    if(checker_3_2(1))
        MnC[15]=32;
    else
        MnC[15]=999;
    if(checker_3_4(1))
        MnC[16]=34;
    else
        MnC[16]=999;
    if(checker_4_1_1())
        MnC[17]=411;
    else
        MnC[17]=999;
    if(checker_4_1_2())
        MnC[18]=412;
    else
        MnC[18]=999;
    if(checker_4_2())
        MnC[19]=42;
    else
        MnC[19]=999;
    if(checker_4_3())
        MnC[20]=43;
    else
        MnC[20]=999;
    if(checker_4_4())
        MnC[21]=44;
    else
        MnC[21]=999;
    if(checker_5_1(1))
        MnC[22]=51;
    else
        MnC[22]=999;
    if(checker_5_2(1))
        MnC[23]=52;
    else
        MnC[23]=999;
    if(checker_5_3())
        MnC[24]=53;
    else
        MnC[24]=999;
    if(checker_5_4())
        MnC[25]=54;
    else
        MnC[25]=999;
    if(checker_6_3())
        MnC[26]=63;
    else
        MnC[26]=999;
    if(checker_6_5())
        MnC[27]=65;
    else
        MnC[27]=999;
    if(checker_7_1_1(1))
        MnC[28]=711;
    else
        MnC[28]=999;
    if(checker_7_2_1(1))
        MnC[29]=721;
    else
        MnC[29]=999;
    if(checker_7_4())
        MnC[30]=74;
    else
        MnC[30]=999;
    if(checker_7_6())
        MnC[31]=76;
    else
        MnC[31]=999;
    if(checker_6_4_1())
        MnC[32]=641;
    else
        MnC[32]=999;
    if(checker_9_1(1))
        MnC[33]=91;
    else
        MnC[33]=999;
    if(checker_9_2(1))
        MnC[34]=92;
    else
        MnC[34]=999;
    if(checker_9_3())
        MnC[35]=93;
    else
        MnC[35]=999;
    if(checker_9_4_1())
        MnC[36]=941;
    else
        MnC[36]=999;
    if(checker_9_5())
        MnC[37]=95;
    else
        MnC[37]=999;

}

int  qs()
{
    int i=0,start=0,high=0;
    int Ist_Minima;
    int d,pos1,pos1Value,IInd_Minima,pos2,pos2Value,pos3,posT,posP,IIIrd_Minima,flagnotpresent;
    int RRPeak,posIniR;
    int PQRSLeftMaxima;
    int PQRSRightMaxima;
    Ist_Minima = PQRSSampl3[0];
    for ( d = 0; d < WINDOW_LENGTH; d++)
    {
        if (Ist_Minima > PQRSSampl3[d])
        {
            Ist_Minima = PQRSSampl3[d];
            pos1 = d;
            /*  //printf("\npos of the minima %d\n",pos1);
              //printf("\nValues for minima %d \n",Ist_Maxima);*/
        }
    }
    pos1Value = PQRSSampl3[pos1];
    // //printf("\npos of the Minima %d\n",pos1);
    // PQRSSampl3[pos1]  = 10000;
    //printf("\nTesting the Minima  %d  \n \n",pos1Value);
//Depending on the mentioned conditions replace the values in the immediate next position in which 1st Minima was found by 100 as well.
    for(i=1;i<=3;i++)
    {
        if (( pos1+i) < WINDOW_LENGTH)
            PQRSSampl1[( pos1+i)] = 10000;
        if (( pos1-i) >= 0)
            PQRSSampl1[( pos1-i)] = 10000;
    }
    IInd_Minima=PQRSSampl3[0];
    for ( d = 0; d < WINDOW_LENGTH; d++){
        if (IInd_Minima > PQRSSampl3[d] && d!=pos1 && PQRSSampl3[d]!=10000){
            // //printf("\nIInd_Minima finding %d\n\n",IInd_Minima);
            IInd_Minima = PQRSSampl3[d];
            pos2 = d;
            //     //printf("d %d\n",d);
        }
    }
    pos2Value = PQRSSampl3[pos2];
    //printf("\n Testing the 2nd Minima %d\n ",PQRSSampl3[pos2]);

    for(i=1;i<=3;i++)
    {
        if (( pos2+i) < WINDOW_LENGTH)
            PQRSSampl1[( pos2+i)] = 10000;
        if (( pos2-i) >= 0)
            PQRSSampl1[( pos2-i)] = 10000;
    }
    IIIrd_Minima = PQRSSampl3[0];
    for ( d = 0; d < WINDOW_LENGTH; d++){
        if (IIIrd_Minima > PQRSSampl3[d]&&d!=pos1&&d!=pos2&&PQRSSampl3[d]!=10000){
//Once we have the 1st Minima we store it...
            IIIrd_Minima = PQRSSampl3[d];
            ////printf("IIIrd_Minima finding %d     line 224\n\n",IIIrd_Minima);
//... as well as its location
            pos3 = d;
            //   //printf("d %d\n",d);
        }
        high = highest(pos1,pos2,pos3);
    }
// Update the location in which Minima was found with 100

    pos3Value = PQRSSampl3[pos3];
    //printf("\n Testing the 3rd Minima %d\n ",PQRSSampl3[pos3]);
    PQRSSampl3[pos3]  = 10000;

    for(i=1;i<=3;i++)
    {
        if (( pos3+i) < WINDOW_LENGTH)
            PQRSSampl1[( pos3+i)] = 10000;
        if (( pos3-i) >= 0)
            PQRSSampl1[( pos3-i)] = 10000;
    }

    int lowest = leasts(pos1, pos2, pos3);
    //printf("------------------------lowest of three   %d    \n",lowest);
    int nextLowest = 0;
    if(lowest == pos1)
    {
        if(pos2 > pos3)
            nextLowest = pos3;
        else
            nextLowest = pos2;

    }else if(lowest == pos2)
    {
        if(pos1 > pos3)
            nextLowest = pos3;
        else
            nextLowest = pos1;
    }
    else {
        if(pos1 > pos2)
            nextLowest = pos2;
        else
            nextLowest = pos1;
    }

    if((nextLowest - lowest) > 150)
    {
        flagnotpresent = 1;
        //printf("\n This means more than one QS-Peaks are obtained in the sample \n");
    }
    else {
        flagnotpresent = 0;
        RRPeak         = 0;
        //printf("\n Single QS peak in the sample .\n\n");
    }

    pos2 = nextLowest;
    pos1 = lowest;
    if(pos1<4){
        //printf(" 1st QS wave is not codeable . \n");
        start = pos1;
        pos1=pos2;
        pos2 = high;

    }
    PQRSLeftMaxima=PQRSSampl3[0];
    for(  i=start ; i < pos1 ; i++)
    {
        if(PQRSLeftMaxima<PQRSSampl3[i] && PQRSSampl3[i]!=10000)
        {
            PQRSLeftMaxima = PQRSSampl3[i];
            posP = i;
        }
    }
    //printf("\n%d p of qs %d",posP,PQRSSampl3[posP]);
    for(i=1;i<=3;i++)
    {
        if (( posP+i) < WINDOW_LENGTH)
            PQRSSampl1[( posP+i)] = 10000;
        if (( posP-i) >= 0)
            PQRSSampl1[( posP-i)] = 10000;
    }
    PQRSLeftMaxima=PQRSSampl3[0];
    for(i=posP;i<pos1;i++)
    {
        if(PQRSLeftMaxima<PQRSSampl3[i]&&PQRSSampl3[i]!=10000)
            PQRSLeftMaxima = PQRSSampl3[i];
        posIniR = i;
    }
    //printf("\n%d Initial R of qs %d",posIniR,PQRSSampl3[posIniR]);

    PQRSRightMaxima=PQRSSampl3[0];

    for(i=pos1;i<pos2;i++)
    {
        if(PQRSRightMaxima<PQRSSampl3[i]&&PQRSSampl3[i]!=10000)
            PQRSRightMaxima = PQRSSampl3[i];
        posT = i;
    }
    //printf("\n%d T of qs %d",posT,PQRSSampl3[posT]);
    for(i=1;i<=3;i++)
    {
        if (( posT+i) < WINDOW_LENGTH)
            PQRSSampl1[( posT+i)] = 10000;
        if (( posT-i) >= 0)
            PQRSSampl1[( posT-i)] = 10000;
    }
    PQRSLeftMaxima=PQRSSampl3[0];

    PQRSRightMaxima=PQRSSampl3[0];
    for(i=pos1;i<posT;i++)
    {
        if(PQRSRightMaxima<PQRSSampl3[i]&&PQRSSampl3[i]!=10000)
            PQRSRightMaxima = PQRSSampl3[i];
        posT = i;
    }
    //printf("\n%d Terminal R of qs %d",posT,PQRSSampl3[posT]);
    return 0;
}
double* pluto(double ECG[])
{
  // remove baseline wandering

  filterECG128Hz(ECG, ECG_new_data);

  // find R-R Interval


  int ecg_size[1];
  int ecg_len1 = 1500;
  ecg_size[0] = ecg_len1;//
  int b_ecg_size[1];
  b_ecg_size[0] = ecg_size[0];
  static double locs_data[1500];
  int locs_size[2];
    double tmp;
  emxInit_real_T(&g_qrs_i_raw, 2);
  emxInit_real_T(&g_qrs_amp_raw, 2);
  emxInit_real_T(&g_noise, 2);
  emxInit_real_T(&g_sig_value,2);

  double ret_array[ecg_len1];
  pan_tompkin(ECG_new_data, b_ecg_size, g_qrs_amp_raw, g_qrs_i_raw, g_noise, g_sig_value, locs_data,
              locs_size);
  // g_qrs_i_raw is the R index in ECG_new_data array
  // g_qrs_amp_raw is the R amplitutude in ECG_new_data array
  // both are global variables
  for(i=0;i<10;i++)
  {

  }
    g_withtompkin = false;
    if((g_qrs_i_raw[0].data[1] - g_qrs_i_raw[0].data[0])<30)
    {
        g_withtompkin = false;
    }
    else
    {
        g_withtompkin = true;
    }


  return ECG_new_data;
}

int least(int a, int b, int c)
{
  if (a < b)
  {
    if (a < c)
      return a;
    else
      return c;
  }

  if (b < c)
    return b;
  else return c;
}

#if 0
void bdac(double array[])
{


  counter +=1;
  //if (counter < 100) // Discard first 540 samples
//		return NOTREADY;



  double PQRSLeft_Minima  = 10000;
  double PQRSRight_Minima = 10000;
  double PQRSLeft_Maxima  = 100;
  double PQRSRight_Maxima = 100;
  double PQRSLeft_Minima1 = 10000;
  double PQRSRight_Minima1= 10000;
  double Ist_Maxima       = 10000;
  double IInd_Maxima      = 10000;
  double IIIrd_Maxima     = 10000;
  int   flagnotpresent    = 0;
  int   FlagPQRSLeft      = 0;
  int   FlagPQRSRight     = 0;
  double RRPeak           = 0;
  double RRPeakValInSecs  = 0;
  //double QTcvalue         = 0;
  //double QTcval1          = 0;
  //double QTcval2          = 0;
  //double m;
  int pos4_P              = 0;
  int pos4_T              = 0;
  int pos3_Q              = 0;
  int pos3_S              = 0;
  int pos4_P_start        = 0;
  int pos4_T_end          = 0;
  int pos3_Q2             = 0;
  int pos3_S2             = 0;
  int pos4_P2             = 0;
  int pos4_T2             = 0;
  int pos4_P2_start       = 0;
  int pos4_T2_end         = 0;
  int pos_P_Q             = 0;
  int pos_Q_R             = 0;
  int pos_R_S             = 0;
  int pos_S_T             = 0;
  int pos_Q_R_S           = 0;
  int pos_P_R             = 0;
  int pos_Q_T             = 0;
  int pos_RRPeak          = 0;
  //int QT_Diff             = 0;
  int pos_P               = 0;

  int   pos1              = 0;
  int   pos2              = 0;
  int   pos3              = 0;
  int   d                 = 0;
  int   e                 = 0;
  int   f                 = 0;
  int   g                 = 0;
  int   h                 = 0;
  int   i                 = 0;
  int   l                 = 0;
  //int flag_sqrt           = 0;
for( int x=0 ;x<400;x++) {

  PQRSSampl1[x] = array[x];
  PQRSSampl2[x] = array[x];
}

  for ( d = 0; d < WINDOW_LENGTH; d++){
    if (Ist_Maxima > PQRSSampl1[d]){
      //Once we have the 1st maxima we store it...
      Ist_Maxima = PQRSSampl1[d];
      //... as well as its location
      pos1 = d;
//          printf("pos1 %d\n",d);
    }
  }
  // Update the location in which Maxima was found with 100
  pos1Value = PQRSSampl1[pos1];
  PQRSSampl1[pos1]  = 10000;



  //Depending on the mentioned conditions replace the values in the immediate next position in which 1st Maxima was found by 100 as well.
  if ((pos1+1) < WINDOW_LENGTH)
    PQRSSampl1[(pos1+1)] = 10000;

  if ((pos1+2) < WINDOW_LENGTH)
    PQRSSampl1[(pos1+2)] = 10000;

  if ((pos1+3) < WINDOW_LENGTH)
    PQRSSampl1[(pos1+3)] = 10000;

  if ((pos1-1) >= 0)
    PQRSSampl1[(pos1-1)] = 10000;

  if ((pos1-2) >= 0)
    PQRSSampl1[(pos1-2)] = 10000;

  if ((pos1-3) >= 0)
    PQRSSampl1[(pos1-3)] = 10000;

  for ( d = 0; d < WINDOW_LENGTH; d++){
    if (IInd_Maxima > PQRSSampl1[d]){
      IInd_Maxima = PQRSSampl1[d];
      pos2 = d;
//          printf("d %d\n",d);
    }
  }

  pos2Value = PQRSSampl1[pos2];
  PQRSSampl1[pos2]  = 10000;

  if ((pos2+1) < WINDOW_LENGTH)
    PQRSSampl1[(pos2+1)] = 10000;

  if ((pos2+2) < WINDOW_LENGTH)
    PQRSSampl1[(pos2+2)] = 10000;

  if ((pos2+3) < WINDOW_LENGTH)
    PQRSSampl1[(pos2+3)] = 10000;

  if ((pos2-1) >= 0)
    PQRSSampl1[(pos2-1)] = 10000;

  if ((pos2-2) >= 0)
    PQRSSampl1[(pos2-2)] = 10000;

  if ((pos2-3) >= 0)
    PQRSSampl1[(pos2-3)] = 10000;

  for ( d = 0; d < WINDOW_LENGTH; d++){
    if (IIIrd_Maxima > PQRSSampl1[d]){
      //Once we have the 1st maxima we store it...
      IIIrd_Maxima = PQRSSampl1[d];
      //... as well as its location
      pos3 = d;
//            printf("d %d\n",d);
    }
  }
  // Update the location in which Maxima was found with 100

  pos3Value = PQRSSampl1[pos3];
  PQRSSampl1[pos3]  = 100;

  if ((pos3+1) < WINDOW_LENGTH)
    PQRSSampl1[(pos3+1)] = 100;

  if ((pos3+2) < WINDOW_LENGTH)
    PQRSSampl1[(pos3+2)] = 100;

  if ((pos3+3) < WINDOW_LENGTH)
    PQRSSampl1[(pos3+3)] = 100;

  if ((pos3-1) >= 0)
    PQRSSampl1[(pos3-1)] = 100;

  if ((pos3-2) >= 0)
    PQRSSampl1[(pos3-2)] = 100;

  if ((pos3-3) >= 0)
    PQRSSampl1[(pos3-3)] = 100;



  int lowest = least(pos1, pos2, pos3);
//    printf("lowest of three %d",lowest);


  int nextLowest = 0;

  if(lowest == pos1) {
    if(pos2 > pos3)
      nextLowest = pos3;
    else
      nextLowest = pos2;

  }else if(lowest == pos2) {

    if(pos1 > pos3)
      nextLowest = pos3;
    else
      nextLowest = pos1;

  }else {
    if(pos1 > pos2)
      nextLowest = pos2;
    else
      nextLowest = pos1;
  }


 // printf("lowest of two %d and %d\n",lowest,nextLowest);

  /*
  PQRSSampl1[pos2]       = 100;
  if ((pos2+1) < WINDOW_LENGTH)
  PQRSSampl1[(pos2+1)] = 100;

  if ((pos2-1) >= 0)
  PQRSSampl1[(pos2-1)] = 100;

  for (d = 0; d<WINDOW_LENGTH; d++){
    if (IIIrd_Maxima < PQRSSampl1[d]){
        IIIrd_Maxima = PQRSSampl1[d];
        pos3 = d;
    }
  }*/

  if((nextLowest - lowest) > 50)
    flagnotpresent = 1;
  else {
    flagnotpresent = 0;
    RRPeak         = 0;
  }

  pos2 = nextLowest;
  pos1 = lowest;

  // This means more than one R-Peaks are obtained in the sample
  if (flagnotpresent == 1){
    // Start of POS2 Calculation
    //==========================================//
    // Create the window having 20 Samples to Left and 20
    // Samples to Right of the Peak R-Value.
    if (((pos2-20) >= 0) && ((pos2+20) < WINDOW_LENGTH)){
      FlagPQRSLeft  = 1;
      PQRSLeft[19]  = PQRSSampl2[pos2-1];
      PQRSLeft[18]  = PQRSSampl2[pos2-2];
      PQRSLeft[17]  = PQRSSampl2[pos2-3];
      PQRSLeft[16]  = PQRSSampl2[pos2-4];
      PQRSLeft[15]  = PQRSSampl2[pos2-5];
      PQRSLeft[14]  = PQRSSampl2[pos2-6];
      PQRSLeft[13]  = PQRSSampl2[pos2-7];
      PQRSLeft[12]  = PQRSSampl2[pos2-8];
      PQRSLeft[11]  = PQRSSampl2[pos2-9];
      PQRSLeft[10]  = PQRSSampl2[pos2-10];
      PQRSLeft[9]   = PQRSSampl2[pos2-11];
      PQRSLeft[8]   = PQRSSampl2[pos2-12];
      PQRSLeft[7]   = PQRSSampl2[pos2-13];
      PQRSLeft[6]   = PQRSSampl2[pos2-14];
      PQRSLeft[5]   = PQRSSampl2[pos2-15];
      PQRSLeft[4]   = PQRSSampl2[pos2-16];
      PQRSLeft[3]   = PQRSSampl2[pos2-17];
      PQRSLeft[2]   = PQRSSampl2[pos2-18];
      PQRSLeft[1]   = PQRSSampl2[pos2-19];
      PQRSLeft[0]   = PQRSSampl2[pos2-20];

      PQRSRight[19] = PQRSSampl2[pos2+1];
      PQRSRight[18] = PQRSSampl2[pos2+2];
      PQRSRight[17] = PQRSSampl2[pos2+3];
      PQRSRight[16] = PQRSSampl2[pos2+4];
      PQRSRight[15] = PQRSSampl2[pos2+5];
      PQRSRight[14] = PQRSSampl2[pos2+6];
      PQRSRight[13] = PQRSSampl2[pos2+7];
      PQRSRight[12] = PQRSSampl2[pos2+8];
      PQRSRight[11] = PQRSSampl2[pos2+9];
      PQRSRight[10] = PQRSSampl2[pos2+10];
      PQRSRight[9]  = PQRSSampl2[pos2+11];
      PQRSRight[8]  = PQRSSampl2[pos2+12];
      PQRSRight[7]  = PQRSSampl2[pos2+13];
      PQRSRight[6]  = PQRSSampl2[pos2+14];
      PQRSRight[5]  = PQRSSampl2[pos2+15];
      PQRSRight[4]  = PQRSSampl2[pos2+16];
      PQRSRight[3]  = PQRSSampl2[pos2+17];
      PQRSRight[2]  = PQRSSampl2[pos2+18];
      PQRSRight[1]  = PQRSSampl2[pos2+19];
      PQRSRight[0]  = PQRSSampl2[pos2+20];

      // Find the positions of Q and S Respectively to the
      // Left and Right, by finding the Minima Points
      for (e = 10; e < 20; e++){
        if (PQRSLeft_Maxima < PQRSLeft[e]){
          PQRSLeft_Maxima = PQRSLeft[e];
          pos3_Q2 = e;
        }
        if (PQRSRight_Maxima < PQRSRight[e]){
          PQRSRight_Maxima = PQRSRight[e];
          pos3_S2 = e;
        }
      }

      // Now, find the Positions of P and T to the Left and
      // Right, by finding the Maxima Points
      for (f = 0; f < pos3_Q2; f++){
        if (PQRSLeft_Maxima < PQRSLeft[f]){
          PQRSLeft_Maxima = PQRSLeft[f];
          pos4_P2 = f;
        }
      }
      for (g = 0; g < pos3_S2; g++){
        if (PQRSRight_Maxima < PQRSRight[g]){
          PQRSRight_Maxima = PQRSRight[g];
          pos4_T2 = g;
        }
      }

      // After, this find the positions of Start of P and End
      // of T Segment, by finding the Minima Points
      for (h = 0; h < pos4_P2; h++){
        if (PQRSLeft_Minima1 > PQRSLeft[h]){
          PQRSLeft_Minima1 = PQRSLeft[h];
          pos4_P2_start = h;
        }
      }
      for (i = 0; i < pos4_T2; i++){
        if (PQRSRight_Minima1 > PQRSRight[i]){
          PQRSRight_Minima1 = PQRSRight[i];
          pos4_T2_end = i;
        }
      }
    }//End if (((pos2-20) >= 0) && ((pos2+20) < WINDOW_LENGTH)){

    // Create the window having 10 Samples to Left and 10
    // Samples to Right of the Peak R-Value. This is if, 20
    // Samples to the Left and Right are not present
    if (FlagPQRSLeft == 0){
      if ((pos2-10) >= 0 && (pos2+10) <= WINDOW_LENGTH){
        PQRSLeft[9]   =PQRSSampl2[pos2-1];
        PQRSLeft[8]   =PQRSSampl2[pos2-2];
        PQRSLeft[7]   =PQRSSampl2[pos2-3];
        PQRSLeft[6]   =PQRSSampl2[pos2-4];
        PQRSLeft[5]   =PQRSSampl2[pos2-5];
        PQRSLeft[4]   =PQRSSampl2[pos2-6];
        PQRSLeft[3]   =PQRSSampl2[pos2-7];
        PQRSLeft[2]   =PQRSSampl2[pos2-8];
        PQRSLeft[1]   =PQRSSampl2[pos2-9];
        PQRSLeft[0]   =PQRSSampl2[pos2-10];

        PQRSRight[9]  =PQRSSampl2[pos2+1];
        PQRSRight[8]  =PQRSSampl2[pos2+2];
        PQRSRight[7]  =PQRSSampl2[pos2+3];
        PQRSRight[6]  =PQRSSampl2[pos2+4];
        PQRSRight[5]  =PQRSSampl2[pos2+5];
        PQRSRight[4]  =PQRSSampl2[pos2+6];
        PQRSRight[3]  =PQRSSampl2[pos2+7];
        PQRSRight[2]  =PQRSSampl2[pos2+8];
        PQRSRight[1]  =PQRSSampl2[pos2+9];
        PQRSRight[0]  =PQRSSampl2[pos2+10];

        // Find the positions of Q and S Respectively to the
        // Left and Right, by finding the Minima Points
        for (e = 0; e< 10; e++){
          if (PQRSLeft_Maxima < PQRSLeft[e]){
            PQRSLeft_Maxima = PQRSLeft[e];
            pos3_Q2 = e;
          }
          if (PQRSRight_Maxima < PQRSRight[e]){
            PQRSRight_Maxima = PQRSRight[e];
            pos3_S2 = e;
          }
        }

        // Now, find the Positions of P and T to the Left and
        // Right, by finding the Maxima Points
        for (f = 0; f < pos3_Q2; f++){
          if (PQRSLeft_Maxima < PQRSLeft[f]){
            PQRSLeft_Maxima = PQRSLeft[f];
            pos4_P2 = f;
          }
        }
        for (g = 0; g < pos3_S2; g++){
          if (PQRSRight_Maxima > PQRSRight[g]){
            PQRSRight_Maxima = PQRSRight[g];
            pos4_T2 = g;
          }
        }
      }// End if ((pos2-10) >= 0 && (pos2+10) <= WINDOW_LENGTH)
    }
    sVal=PQRSRight_Maxima;
    // End if (FlagPQRSLeft == 0){
    // End of POS2 Calculation
    //=================================================//

    PQRSLeft_Minima     = 10000;
    PQRSRight_Minima    = 10000;
    PQRSLeft_Maxima     = 100;
    PQRSRight_Maxima    = 100;
    PQRSLeft_Minima1    = 10000;
    PQRSRight_Minima1   = 10000;
    // Create the window having 20 Samples to Left and 20
    // Samples to Right of the Peak R-Value.
    if ((pos1-20) >= 0 && (pos1+20) <= WINDOW_LENGTH){
      FlagPQRSRight = 1;
      PQRSLeft[19]  = PQRSSampl2[pos1-1];
      PQRSLeft[18]  = PQRSSampl2[pos1-2];
      PQRSLeft[17]  = PQRSSampl2[pos1-3];
      PQRSLeft[16]  = PQRSSampl2[pos1-4];
      PQRSLeft[15]  = PQRSSampl2[pos1-5];
      PQRSLeft[14]  = PQRSSampl2[pos1-6];
      PQRSLeft[13]  = PQRSSampl2[pos1-7];
      PQRSLeft[12]  = PQRSSampl2[pos1-8];
      PQRSLeft[11]  = PQRSSampl2[pos1-9];
      PQRSLeft[10]  = PQRSSampl2[pos1-10];
      PQRSLeft[9]   = PQRSSampl2[pos1-11];
      PQRSLeft[8]   = PQRSSampl2[pos1-12];
      PQRSLeft[7]   = PQRSSampl2[pos1-13];
      PQRSLeft[6]   = PQRSSampl2[pos1-14];
      PQRSLeft[5]   = PQRSSampl2[pos1-15];
      PQRSLeft[4]   = PQRSSampl2[pos1-16];
      PQRSLeft[3]   = PQRSSampl2[pos1-17];
      PQRSLeft[2]   = PQRSSampl2[pos1-18];
      PQRSLeft[1]   = PQRSSampl2[pos1-19];
      PQRSLeft[0]   = PQRSSampl2[pos1-20];

      PQRSRight[19] =PQRSSampl2[pos1+1];
      PQRSRight[18] =PQRSSampl2[pos1+2];
      PQRSRight[17] =PQRSSampl2[pos1+3];
      PQRSRight[16] =PQRSSampl2[pos1+4];
      PQRSRight[15] =PQRSSampl2[pos1+5];
      PQRSRight[14] =PQRSSampl2[pos1+6];
      PQRSRight[13] =PQRSSampl2[pos1+7];
      PQRSRight[12] =PQRSSampl2[pos1+8];
      PQRSRight[11] =PQRSSampl2[pos1+9];
      PQRSRight[10] =PQRSSampl2[pos1+10];
      PQRSRight[9]  =PQRSSampl2[pos1+11];
      PQRSRight[8]  =PQRSSampl2[pos1+12];
      PQRSRight[7]  =PQRSSampl2[pos1+13];
      PQRSRight[6]  =PQRSSampl2[pos1+14];
      PQRSRight[5]  =PQRSSampl2[pos1+15];
      PQRSRight[4]  =PQRSSampl2[pos1+16];
      PQRSRight[3]  =PQRSSampl2[pos1+17];
      PQRSRight[2]  =PQRSSampl2[pos1+18];
      PQRSRight[1]  =PQRSSampl2[pos1+19];
      PQRSRight[0]  =PQRSSampl2[pos1+20];

      // Find the positions of Q and S Respectively to the
      // Left and Right, by finding the Minima Points
      for (e = 10; e < 20; e++){
        if (PQRSLeft_Maxima < PQRSLeft[e]){
          PQRSLeft_Maxima = PQRSLeft[e];
          pos3_Q = e;
        }
        if (PQRSRight_Maxima < PQRSRight[e]){
          PQRSRight_Maxima = PQRSRight[e];
          pos3_S = e;
        }
      }

      // Now, find the Positions of P and T to the Left and
      // Right, by finding the Maxima Points
      for (f = 0; f < pos3_Q; f++){
        if (PQRSLeft_Maxima < PQRSLeft[f]){
          PQRSLeft_Maxima = PQRSLeft[f];
          pos4_P = f;
        }
      }
      for (g = 0; g < pos3_S; g++){
        if (PQRSRight_Maxima < PQRSRight[g]){
          PQRSRight_Maxima = PQRSRight[g];
          pos4_T = g;
        }
      }

      // After, this find the positions of Start of P and End
      // of T Segment, by finding the Minima Points
      for (h = 0; h < pos4_P; h++){
        if (PQRSLeft_Minima1 > PQRSLeft[h]){
          PQRSLeft_Minima1 = PQRSLeft[h];
          pos4_P_start = h;
        }
      }
      for (i = 0; i < pos4_T; i++){
        if (PQRSRight_Minima1 > PQRSRight[i]){
          PQRSRight_Minima1 = PQRSRight[i];
          pos4_T_end = i;
        }
      }
    }// End if ((pos1-20) >= 1 && (pos1+20) <= Window_Size){

    // Create the window having 10 Samples to Left and 10
    // Samples to Right of the Peak R-Value.This is if, 20
    // Samples to the Left and Right are not present
    if (FlagPQRSRight == 0){
      if ((pos1-10) >= 1 && (pos1+10) <= WINDOW_LENGTH){
        PQRSLeft[9]  =PQRSSampl2[pos1-1];
        PQRSLeft[8]  =PQRSSampl2[pos1-2];
        PQRSLeft[7]  =PQRSSampl2[pos1-3];
        PQRSLeft[6]  =PQRSSampl2[pos1-4];
        PQRSLeft[5]  =PQRSSampl2[pos1-5];
        PQRSLeft[4]  =PQRSSampl2[pos1-6];
        PQRSLeft[3]  =PQRSSampl2[pos1-7];
        PQRSLeft[2]  =PQRSSampl2[pos1-8];
        PQRSLeft[1]  =PQRSSampl2[pos1-9];
        PQRSLeft[0]  =PQRSSampl2[pos1-10];

        PQRSRight[9] =PQRSSampl2[pos1+1];
        PQRSRight[8] =PQRSSampl2[pos1+2];
        PQRSRight[7] =PQRSSampl2[pos1+3];
        PQRSRight[6] =PQRSSampl2[pos1+4];
        PQRSRight[5] =PQRSSampl2[pos1+5];
        PQRSRight[4] =PQRSSampl2[pos1+6];
        PQRSRight[3] =PQRSSampl2[pos1+7];
        PQRSRight[2] =PQRSSampl2[pos1+8];
        PQRSRight[1] =PQRSSampl2[pos1+9];
        PQRSRight[0] =PQRSSampl2[pos1+10];

        // Find the positions of Q and S Respectively to the
        // Left and Right, by finding the Minima Points
        for (e = 0; e < 10; e++){
          if (PQRSLeft_Maxima < PQRSLeft[e]){
            PQRSLeft_Maxima = PQRSLeft[e];
            pos3_Q = e;
          }
          if (PQRSRight_Maxima < PQRSRight[e]){
            PQRSRight_Maxima = PQRSRight[e];
            pos3_S = e;
          }
        }
        // Now, find the Positions of P and T to the Left and
        // Right, by finding the Maxima Points
        for (f = 0; f < pos3_Q; f++){
          if (PQRSLeft_Maxima < PQRSLeft[f]){
            PQRSLeft_Maxima = PQRSLeft[f];
            pos4_P = f;
          }
        }
        for (g = 0; g < pos3_S; g++){
          if (PQRSRight_Maxima < PQRSRight[g]){
            PQRSRight_Maxima = PQRSRight[g];
            pos4_T = g;
          }
        }
      }// End if ((pos1-10) >= 1 && (pos1+10) <= WINDOW_LENGTH){
    }// End if (FlagPQRSRight == 0){

    // Ok, now, Calculate the P-Q, Q-R, R-S And S-T
    // Intervals. For 20 Length Window and 10 Length Window
    // Cases
    if (FlagPQRSRight == 1){
      pos_P_Q = pos3_Q - pos4_P;//pos4_P_start;
      pos_Q_R = 20-pos3_Q;
      pos_R_S = 20-pos3_S;
      pos_S_T = pos3_S - pos4_T;//pos4_T_end;//pos4_T;
      pos_Q_R_S = (pos_Q_R + pos_R_S);
      pos_P_R   = pos_P_Q + pos_Q_R;
      pos_Q_T   = pos_Q_R_S + pos_S_T;
      pos_P     = pos4_P - pos4_P_start;
    }

    // Ok, now, Calculate the P-Q, Q-R, R-S And S-T Intervals
    if (FlagPQRSRight == 0){
      if (FlagPQRSLeft == 1){
        pos_P_Q = pos3_Q2 - pos4_P2;//pos4_P2_start;
        pos_Q_R = 20-pos3_Q2;
        pos_R_S = 20-pos3_S2;
        pos_S_T = pos3_S2 - pos4_T2;//pos4_T2_end;//pos4_T2;
        pos_Q_R_S = (pos_Q_R + pos_R_S);
        pos_P_R   = pos_P_Q + pos_Q_R;
        pos_Q_T   = pos_Q_R_S + pos_S_T;
        pos_P     = pos4_P - pos4_P_start;
      }
      else{
        pos_P_Q = pos3_Q - pos4_P;
        pos_Q_R = 10 - pos3_Q;
        //pos_Q_R = pos3_Q + 1;
        pos_R_S = 10 - pos3_S;
        //pos_R_S = pos3_S + 1;
        pos_S_T = pos3_S - pos4_T;
        pos_Q_R_S = (pos_Q_R + pos_R_S);
        pos_P_R   = pos_P_Q + pos_Q_R;
        pos_Q_T   = pos_Q_R_S + pos_S_T;// + 3;
        pos_P     = pos4_P;
      }
    }

    if (pos1 > pos2)
      pos_RRPeak = pos1 - pos2;
    else
      pos_RRPeak = pos2 - pos1;

    // There can be two cases here. if there are 3 peaks in
    // the RRPeakValWindow, and the Peaks are not consecutive, but a
    // R Peak inbetween interleaves them.
    //if (pos_RRPeak > 140)
    //    RRPeak = (15000/pos_RRPeak);
    //else
    RRPeak = (60*200)/pos_RRPeak;//(7500/pos_RRPeak);

    RRPeakVal = (int)RRPeak;

    if (RRPeakVal >= 160)
      RRPeakVal = (RRPeakVal >> 1);

    if (RRPeakVal < 54)
      RRPeakVal = (RRPeakVal << 1);



    RRPeakValInSecs    = (double)(RRPeakVal/60);
    IntervalInSecs     = (double)(RRPeakValInSecs/pos_RRPeak);
    //	hr = RRPeakVal  ;//60/(RRPeakValInSecs * 0.04);//60/ IntervalInSecs;//   1000 * (double)(0.0182 * RRPeakVal );
    hr = RRPeakVal;//RRPeak;
    QRS                = (double)(pos_Q_R_S);///2;
    PR                 = (double)(0.0142 * pos_P_R);///2;
    QT                 = (double)(0.0142 * pos_Q_T);///2;
    P                  = (double)(IntervalInSecs * pos_P);

    //===============Rajiv Start

    //QTc = QT;
    //RRPeakValInSecs    = (double)(RRPeakVal/60);

    //QTc   = QT + (1.75*(RRPeakVal - 60));


    //RRPeakValInSecs = (pos_RRPeak * 0.0182);
    /*
     if (RRPeakValInSecs < 1){
        RRPeakValInSecs = (RRPeakValInSecs * 100);
        flag_sqrt = 1;
     }

     for(m=0;m<RRPeakValInSecs;m=m+n_sqrt)
     {
      if((m*m)>RRPeakValInSecs)
      {
       m=m-n_sqrt;  // This if() is used to calculate the final value as soon as the square of the number exceeds
       break;  // the number then we deduct the value exceeded and stop the procedure using break; this is our final value which is stored in m;
      }
     }

     if (flag_sqrt)
     m = m/10;

     QTc  = (double)(QT/m);
    */

    //===============Rajiv end

    /*
     if (RRPeakValInSecs > 1)
         RRPeakValInSecs = RRPeakValInSecs -1;
     else
         RRPeakValInSecs = 1 - RRPeakValInSecs;

     QTcvalue  = (RRPeakValInSecs/2);
     QTcval2   = QTcvalue;
     QTcvalue  = 1 + QTcvalue;
     QTcval1   = ((RRPeakValInSecs * RRPeakValInSecs)/8);
     QTcvalue  = QTcvalue - QTcval1;
     QTcvalue  = QTcvalue + (QTcval1 * QTcval2);

     QTc       = (double)(QT/QTcvalue);
     */


    QRSVal             = (int)((1000*((QRS/10)+0.7)*0.04)); // In milliseconds, QRSD_Duration
    PRVal              = (int)(1000*PR);  // In milliseconds, PR_Interval
    PVal               = RRPeakVal;//(int)(1000*P);   // P_Duration
    QTVal              = (int)(1000*QT);  // QT_Interval
    //QTcVal             = (int)(1000*QTc);  // QT_Interval
    // HODGES REgression Formulae for QT Correction Value/QTc Value Calculation.
    QTcVal             = QTVal + (1.75*(RRPeakVal - 60));

    if (PRVal >= 260)
      PRVal = (PRVal >> 1);

    if (PRVal < 80)
      PRVal = (PRVal << 1);

    //	if (QTVal >= 480)
    //		QTVal = (QTVal >> 1);

    //	if (QTVal < 150)
//			QTVal = (QTVal << 1);

    doubleAvgRRPeakVal += (double)RRPeakVal;
    doubleAvgPRVal     += PRVal;
    doubleAvgQTVal     += QTVal;
    doubleAvgQTcVal    += QTcVal;
    doubleAvgQRSVal    += QRSVal;

    iter += 1;

    AvgRRPeakVal = (int)(doubleAvgRRPeakVal/iter);
    //hr = AvgRRPeakVal* 0.12;//AvgRRPeakVal*0.132;//
    AvgPRVal     = (int)(doubleAvgPRVal/iter);
    AvgQTVal     = (int)(doubleAvgQTVal/iter);
    AvgQTcVal    = (int)(doubleAvgQTcVal/iter);
    AvgQRSVal    = (int)(doubleAvgQRSVal/iter);


    if (iter==16){
      iter =1;
      doubleAvgRRPeakVal = 0;
      doubleAvgPRVal     = 0;
      doubleAvgQTVal     = 0;
      doubleAvgQTcVal    = 0;
      doubleAvgQRSVal    = 0;
    }

    /*
     doubleAvgRRPeakVal += (double)RRPeakVal;
     doubleAvgPRVal     += PR;
     doubleAvgQTVal     += QT;
     doubleAvgQTcVal    += QTc;
     doubleAvgQRSVal    += QRS;

     doubleAvgRRPeakVal = (double)(doubleAvgRRPeakVal/iter);
     doubleAvgPRVal     = (double)(doubleAvgPRVal/iter);
     doubleAvgQTVal     = (double)(doubleAvgQTVal/iter);
     doubleAvgQTcVal    = (double)(doubleAvgQTcVal/iter);
     doubleAvgQRSVal    = (double)(doubleAvgQRSVal/iter);
     */

    /*
     if (indexval!=2){
     doubleAvgRRPeakVal[indexval] = RRPeakVal;
     doubleAvgPRVal[indexval]     = PRVal;
     doubleAvgQTVal[indexval]     = QTVal;
     doubleAvgQTcVal[indexval]    = QTcVal;
     doubleAvgQRSVal[indexval]    = QRSVal;
     if ((doubleAvgQRSVal[(indexval+1)%3] - QRSVal) > 20)
         QRSVal = (doubleAvgQRSVal[(indexval+1)%3]+QRSVal)/2;



     }else
         indexval =0;


         indexval +=1;
     AvgRRPeakVal = (int)RRPeakVal;//((doubleAvgRRPeakVal[0] + doubleAvgRRPeakVal[1] + doubleAvgRRPeakVal[2])/3);
     AvgPRVal     = (int)((doubleAvgPRVal[0] + doubleAvgPRVal[1] + doubleAvgPRVal[2])/3);
     AvgQTVal     = (int)((doubleAvgQTVal[0] + doubleAvgQTVal[1] + doubleAvgQTVal[2])/3);
     AvgQTcVal    = (int)((doubleAvgQTcVal[0] + doubleAvgQTcVal[1] + doubleAvgQTcVal[2])/3);
     AvgQRSVal    = (int)((doubleAvgQRSVal[0] + doubleAvgQRSVal[1] + doubleAvgQRSVal[2])/3);
     //QRSVal       = AvgQRSVal;
     //PRVal        = AvgPRVal;
     //QTVal        = AvgQTVal;
     //QTcVal       = AvgQTcVal;

     //iter += 1;

    */


  }// End if (flagnotpresent == 1){
  else{
    RRPeak    =0;
    RRPeakVal =0;
  }
  POS_QV = PRVal;
  POS_TV = pos4_T2;

  __// android_log_print(// android_log_VERBOSE, APPNAME, "AvgPRVal %d", AvgPRVal);
  __// android_log_print(// android_log_VERBOSE, APPNAME, "AvgQTVal %d", AvgQTVal);
  __// android_log_print(// android_log_VERBOSE, APPNAME, "AvgQTcVal %d", AvgQTcVal);
  __// android_log_print(// android_log_VERBOSE, APPNAME, "AvgQRSVal %d", AvgQRSVal);


  // return RRPeakVal;//QRSVal;//1;
  //return pos4_P2;
}
#endif



/*
10 91

1111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111
1111111111111111111111110011111111111111111111111111111111111111111111001111111111111111111
1111111111111111111111100001111111111111111111111111111111111111111110000111111111111111111
1111111111111111111111001100111111111111111111111111111111111111111100110011111111111111111
1111111111111111111110011110011111111111111111111111111111111111111001111001111111111111111
1111111111111111111100111111001111111111000011111111111111111111110011111100111111000011111
1111111111000011111001111111100111111110010001111111111111111111100111111110011110010011111
0000000000011000000011111111110011111100111100111111000000111111001111111111001110011001111
1111111111111111111111111111111001111001111110000000011110000000011111111111100000111100000
1111111111111111111111111111111100000011111111111111111111111111111111111111111111111111111


111111111111111111111111111111111111111111111111111111111111111111111111111111111111
111111111111111111111111111111111111111001111111111111001111111111111111111111111111
111111111111111111111111111111111111110000111111111100100111111111111111111111111111
111111111111111111111111000011111111100110011111111001110011111111111111111111111111
111111111111100011111110010001111111001111001111110011111100111111111111111111111111
111100001111001001111100111100111110011111100111100111111110011111111111111111111111
111000100110011100111001111110011100111111110011001111111111100111111111111111111111
110011110000111110010011111111001001111111111000011111111111110011111111111111111111
100111111001111111000111111111100011111111111100111111111111111111111111111111111111
111111111111111111111111111111111111111111111111111111111111111111111111111111111111


10 91

1111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111
1111111111111111111111110011111111111111111111111111111111111111111111001111111111111111111
1111111111111111111111100001111111111111111111111111111111111111111110000111111111111111111
1111111111111111111111001100111111111111111111111111111111111111111100110011111111111111111
1111111111111111111110011110011111111111111111111111111111111111111001111001111111111111111
1111111111111111111100111111001111111111000011111111111111111111110011111100111111000011111
1111111111000011111001111111100111111110010001111111111111111111100111111110011110010011111
0000000000011000000011111111110011111100111100111111000000111111001111111111001110011001111
1111111111111111111111111111111001111001111110000000011110000000011111111111100000111100000
1111111111111111111111111111111100000011111111111111111111111111111111111111111111111111111


111111111111111111111111111111111111111111111111111111111111111111111111111111111111
111111111111111111111111111111111111111001111111111111001111111111111111111111111111
111111111111111111111111111111111111110000111111111100100111111111111111111111111111
111111111111111111111111000011111111100110011111111001110011111111111111111111111111
111111111111100011111110010001111111001111001111110011111100111111111111111111111111
111100001111001001111100111100111110011111100111100111111110011111111111111111111111
111000100110011100111001111110011100111111110011001111111111100111111111111111111111
110011110000111110010011111111001001111111111000011111111111110011111111111111111111
100111111001111111000111111111100011111111111100111111111111111111111111111111111111
111111111111111111111111111111111111111111111111111111111111111111111111111111111111



*/
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include <stdlib.h>
#include<math.h>

#define PI 3.14159265
#define WINDOW_SIZE 20

#define THRESHOLD_PEAK_DIFF 10
#define LEFT_WINDOW_SIZE 30
#define RIGHT_WINDOW_SIZE 40
char inputdata[250][250];
char outdata[250][250];

int g_row_no = 50, g_col_no = 50, g_peak_cnt = 0, g_avg_R_Hight = 99999;
int g_window_size = 20;
int g_ecg_data_cnt = 1300;
int g_no_of_R_peak = 1;
double ecg_data_org[9999], ecg_data_org_tmp[9999], min_x, max_x;
//double input_data[9999] = {
//	-0.06, -0.065, -0.06, -0.075, -0.065, -0.07, -0.07, -0.09, -0.08, -0.095, -0.08, -0.095, -0.08, -0.095, -0.085, -0.09, -0.09, -0.1, -0.085, -0.105, -0.09, -0.045, 0.005, 0.015, 0.045, 0.155, 0.14, 0.045, 0.005, -0.04, -0.085, -0.2, -0.195, -0.2, -0.2, -0.24, -0.13, 0.34, 1.155, 1.47, -0.155, -0.825, -0.59, -0.35, -0.155, -0.17, -0.14, -0.155, -0.115, -0.125, -0.09, -0.095, -0.065, -0.055, -0.015, -0.005, 0.035, 0.045, 0.09, 0.11, 0.15, 0.18, 0.205, 0.225, 0.23, 0.22, 0.235, 0.23, 0.2, 0.17, 0.12, 0.075, 0.04, 0.02, 0.005, -0.005, -0.005, -0.01, -0.015, -0.01, 0, -0.005, -0.02, -0.02, -0.015, -0.025, -0.035, -0.045, -0.045, -0.06, -0.065, -0.07, -0.055, -0.06, -0.08, -0.075, -0.09, -0.065, -0.075, -0.08, -0.08, -0.085, -0.09, -0.075, -0.085, -0.095, -0.105, -0.09, -0.095, -0.07, -0.045, 0.005, 0.03, 0.07, 0.195, 0.145, 0.05, -0.015, -0.025, -0.115, -0.18, -0.19, -0.185, -0.185, -0.215, 0.01, 0.575, 1.185, 0.96, -0.16, -0.74, -0.64, -0.35, -0.175, -0.145, -0.15, -0.145, -0.105, -0.12, -0.09, -0.085, -0.07, -0.04, -0.02, 0.005, 0.02, 0.045, 0.08, 0.115, 0.14, 0.165, 0.21, 0.19, 0.22, 0.21, 0.215, 0.215, 0.16, 0.135, 0.1, 0.05, 0.035, -0.005, 0.005, -0.005, -0.015, -0.02, -0.01, 0, -0.02, -0.005, -0.04, -0.035, -0.05, -0.04, -0.055, -0.075, -0.05, -0.075, -0.09, -0.08, -0.09, -0.08, -0.075, -0.085, -0.085, -0.095, -0.095, -0.105, -0.08, -0.115, -0.1, -0.095, -0.1, -0.075, -0.005, 0.01, 0.055, 0.185, 0.145, 0.075, -0.005, -0.025, -0.11, -0.175, -0.18, -0.22, -0.175, -0.24, -0.03, 0.53, 1.21, 1.22, -0.105, -0.795, -0.69, -0.375, -0.165, -0.155, -0.135, -0.15, -0.12, -0.12, -0.09, -0.095, -0.05, -0.05, -0.01, -0.005, 0.03, 0.08, 0.075, 0.145, 0.155, 0.2, 0.2, 0.24, 0.235, 0.245, 0.245, 0.215, 0.18, 0.15, 0.09, 0.035, 0.025, 0, -0.015, -0.03, 0, -0.02, -0.005, -0.025, -0.03, -0.015, -0.01, -0.035, -0.04, -0.05, -0.045, -0.04, -0.075, -0.065, -0.095, -0.085, -0.085, -0.09, -0.1, -0.1, -0.08, -0.105, -0.105, -0.1, -0.105, -0.095, -0.1, -0.09, -0.035, 0.01, 0.035, 0.07, 0.19, 0.125, 0.045, -0.035, -0.06, -0.155, -0.21, -0.225, -0.2, -0.215, -0.265, 0.07, 0.865, 1.495, 0.625, -0.44, -0.815, -0.6, -0.31, -0.175, -0.18, -0.165, -0.135, -0.135, -0.115, -0.105, -0.085, -0.06, -0.03, -0.02, 0.015, 0.05, 0.07, 0.105, 0.165, 0.2, 0.205, 0.235, 0.235, 0.265, 0.26, 0.235, 0.215, 0.17, 0.125, 0.09, 0.025, -0.015, -0.03, -0.035, -0.04, -0.04, -0.035, -0.04, -0.02, -0.04, -0.04, -0.03, -0.045, -0.065, -0.06, -0.06, -0.09, -0.08, -0.095, -0.085, -0.105, -0.09, -0.1, -0.105, -0.115, -0.1, -0.115, -0.11, -0.135, -0.095, -0.125, -0.105, -0.085, -0.015, 0.02, 0.03, 0.135, 0.16, 0.105, 0.03, -0.05, -0.05, -0.17, -0.205, -0.215, -0.175, -0.21, -0.215, 0.175, 1.09, 1.7, 0.335, -0.615, -0.705, -0.46, -0.175, -0.165, -0.145, -0.155, -0.115, -0.12, -0.105, -0.105, -0.085, -0.06, -0.015, -0.005, 0.02, 0.05, 0.08, 0.105, 0.18, 0.205, 0.24, 0.235, 0.27, 0.27, 0.28, 0.26, 0.23, 0.175, 0.135, 0.09, 0.045, 0, -0.005, -0.025, -0.025, -0.025, -0.02, 0, -0.005, -0.005, -0.005, -0.005, -0.01, -0.025, -0.02, -0.04, -0.04, -0.04, -0.05, -0.06, -0.05, -0.065, -0.06, -0.07, -0.06, -0.09, -0.08, -0.095, -0.085, -0.1, -0.09, -0.095, -0.085, -0.1, -0.075, -0.095, -0.08, -0.075, 0, 0.03, 0.035, 0.135, 0.145, 0.07, 0.015, -0.045, -0.055, -0.185, -0.19, -0.22, -0.18, -0.21, -0.185, 0.22, 1.15, 1.72, 0.155, -0.7, -0.66, -0.405, -0.15, -0.16, -0.135, -0.145, -0.12, -0.125, -0.09, -0.09, -0.055, -0.04, -0.015, 0, 0.03, 0.05, 0.095, 0.11, 0.165, 0.19, 0.23, 0.245, 0.255, 0.255, 0.265, 0.255, 0.23, 0.185, 0.145, 0.1, 0.06, 0.025, 0.025, 0.01, 0.01, 0.01, 0.015, 0.01, 0.025, 0.02, 0.02, 0.02, 0.015, 0, 0, -0.01, -0.015, -0.02, -0.03, -0.035, -0.035, -0.045, -0.04, -0.045, -0.045, -0.055, -0.045, -0.055, -0.045, -0.06, -0.05, -0.06, -0.055, -0.08, -0.065, -0.09, -0.075, -0.09, -0.07, -0.075, -0.065, -0.08, -0.065, -0.03, 0.025, 0.04, 0.065, 0.18, 0.145, 0.055, 0.015, -0.035, -0.06, -0.175, -0.17, -0.195, -0.175, -0.22, -0.115, 0.355, 1.24, 1.595, -0.115, -0.815, -0.565, -0.3, -0.13, -0.16, -0.13, -0.135, -0.11, -0.12, -0.09, -0.085, -0.055, -0.04, -0.005, 0, 0.04, 0.05, 0.095, 0.125, 0.17, 0.19, 0.22, 0.23, 0.245, 0.25, 0.26, 0.24, 0.22, 0.175, 0.135, 0.085, 0.055, 0.02, 0.015, 0.01, 0.01, 0, 0, 0, 0.01, 0.005, 0.02, 0.01, 0, -0.01, -0.015,
//};
//double input_data[9999] = {
//	591, 591, -254.6, -251, -251.2, -248, -243, -249, -259.2, -263.4, -261.6, -267.4, -272.4, -268.2, -268, -280.4, -275.6, -266.2, -264.2, -261.4, -244.6, -234.6, -224.6, -218.2, -218.6, -224.6, -236, -247.4, -250, -246, -244.2, -243, -237.8, -237.6, -234.6, -227.8, -226.4, -228.4, -232.4, -236.4, -234.4, -205, -144.4, -76.6, -40, -35.8, -70.6, -146, -219.6, -260.8, -274.6, -274.6, -263.6, -258.4, -260, -264, -262, -259, -258.2, -254.6, -246, -239.4, -240, -237.2, -234.2, -235.2, -234.4, -225.2, -217.4, -211.4, -206.4, -202.2, -210.8, -222.4, -229.8, -229.6, -232.6, -231.8, -224, -218.2, -225.2, -233.6, -237.4, -246.2, -255.4, -258, -262, -273.6, -280.2, -288.8, -302.8, -313.2, -316.8, -318.2, -316.4, -309.2, -305.2, -305.2, -305.2, -287.2, -287.2, -287.2, -284, -281.6, -281.6, -280.8, -275.6, -262.6, -252.2, -253.4, -257.8, -252.8, -256.2, -253.6, -242.4, -236.4, -236.4, -227.6, -217.4, -216.6, -214.6, -209.2, -202.4, -200.8, -197, -194, -198.4, -210.2, -215.6, -213, -210.8, -212, -212.2, -222, -235.8, -245.2, -242.4, -237.2, -233.8, -235.2, -235.4, -238.8, -185.4, -104, -37.4, -5.6, -2.6, -74, -170.2, -239.8, -275, -275.6, -261.6, -246.6, -238.6, -230.2, -239.2, -247.6, -248.6, -253.2, -255.8, -251.4, -250.8, -258.2, -251.8, -244.4, -232.8, -221, -209, -214.8, -225.8, -237, -236.4, -241.6, -248, -257.8, -265.8, -287, -302.6, -311.4, -325.4, -344, -355.4, -363, -377.2, -380.4, -380, -389, -402.4, -414, -430.2, -455, -474.8, -493, -503, -517.8, -530.4, -530.4, -530.4, -569.6, -569.6, -569.6, -570.4, -576, -571.2, -571.2, -577.4, -583.6, -582, -571.8, -562.6, -560.6, -569.8, -579.4, -598, -608.6, -612.4, -615.2, -617.4, -613.6, -613.6, -615.2, -604.4, -599.4, -597.6, -591.4, -584, -587, -593.6, -598.4, -602.8, -605.6, -605.8, -597.6, -589.2, -588, -589.6, -585.2, -593.4, -608, -625.6, -648.4, -663, -634, -561, -472.2, -406.6, -382.8, -409.2, -484.8, -582.4, -645.2, -679.8, -692.4, -690, -672.6, -661.2, -648.8, -634, -612.6, -600.2, -581.4, -555.2, -534.6, -528.4, -514.2, -503.8, -506.6, -517.4, -524.2, -533, -533.4, -522.2, -494, -465.6, -437.2, -414.2, -393.2, -377.2, -362.2, -344.8, -334, -331.6, -330.2, -327.6, -337.6, -348.6, -355, -365, -378, -384.8, -388.8, -393, -395.6, -392.2, -381.4, -372.6, -372.6, -372.6, -335.6, -335.6, -335.6, -340.6, -355.4, -362.8, -362.6, -358, -342.6, -327.6, -306, -279.6, -259.6, -244.8, -219.6, -203.4, -197.4, -186.6, -173, -165.2, -156.6, -142.2, -129.2, -117.2, -112.4, -118.6, -126.6, -125.2, -123, -119.4, -111, -102.4, -103.8, -111.6, -119, -115.6, -116, -120, -123.6, -127, -139.6, -161, -173.2, -184.4, -191.4, -202.2, -206, -214.8, -210.8, -182.2, -102.2, -36.8, 0.2, 4.2, -33.4, -122, -187, -215, -219.4, -204.6, -184.8, -169.8, -158.6, -149.6, -147.6, -144, -135.6, -126.6, -114.8, -99.4, -86, -73, -65.8, -62.2, -58.2, -56.8, -55.4, -40.8, -22.6, -12.2, 0.2, 7.4, 9.6, 5.4, 3.8, 5.6, 8.2, 4, 3.6, 4.8, -7.4, -22, -39, -55, -72.4, -82.4, -90.2, -97.2, -97.2, -97.2, -130.8, -130.8, -130.8, -134.2, -137.2, -140, -135.8, -129.6, -125.6, -127.8, -129.2, -133.6, -135.8, -127.4, -116.4, -108.4, -99.6, -93.6, -88.8, -86.8, -86.8, -88.6, -85.8, -93.4, -92.2, -78.8, -70.2, -70.2, -63.2, -60.4, -72.4, -70.6, -65.4, -65.6, -69.6, -64.4, -60.8, -61.8, -63.4, -59, -64, -73.2, -78.4, -81.8, -93.2, -99, -106, -113, -115.4, -124.8, -132.6, -132.6, -119.2, -78, 7.2, 90, 136, 136.8, 77.6, -13.8, -98, -145.4, -165.6, -157.2, -147.8, -140.4, -142.8, -143.8, -138, -131, -131.4, -130, -126.8, -128.4, -129.8, -130.8, -122, -115.2, -113.6, -111.6, -106.4, -113.6, -116.4, -111, -109.8, -108.2, -103.2, -100.2, -103.6, -109.2, -108.4, -108.4, -111.4, -114.2, -122.2, -143.8, -162.6, -178, -178, -178, -223.4, -223.4, -223.4, -229.8, -223.6, -219.6, -218, -224.6, -215.8, -211.4, -209.8, -207.8, -199.6, -200.6, -200.4, -194.8, -191.8, -191.6, -190.6, -188.2, -186.4, -180.6, -176.6, -170.8, -161.2, -151.8, -147.6, -144.4, -144.2, -153, -154.8, -157.2, -154.2, -143.8, -131, -125.6, -113.6, -107, -104.4, -113.2, -122, -133.6, -140.6, -141.8, -124.2, -110.2, -95.2, -80.2, -71.2, -76.4, -80.8, -86.4, -89, -78.6, -40.8, 30.8, 110, 130.8, 122, 76.6, -8.6, -95.4, -122.2, -135.8, -132, -117.2, -106, -98.4, -81.2, -62.6, -51.2, -46.6, -44.4, -46.4, -47.8, -48, -42.8, -43.8, -45, -45.8, -44.4, -37.8, -27.8, -22.4, -27.2, -35.4, -44, -47.8, -54, -50, -36.6, -24.8, -20, -12.2, -10.2, -18.8, -33, -36.6, -36.6, -36.6, -83.2, -83.2, -83.2, -93.4, -100, -96.8, -89, -90.4, -81.6, -74.2, -69.8, -74.2, -74, -75.6, -70.8, -69.8, -63.6, -64.6, -69.2, -71.6, -69.2, -72.2, -65, -61.8, -61.2, -60.2, -58.2, -59.6, -56, -52.6, -50.6, -47.6, -49, -51.2, -48.8, -54.4, -56.6, -58.2, -62.4, -77.4, -85, -90, -88.6, -84.6, -78.8, -72.2, -70.6, -81.2, -96.6, -104.8, -114.2, -117.6, -114.6, -117, -121.8, -107.2, -40.8, 30, 96, 138.6, 128.4, 45.6, -42.6, -109.8, -152.8, -156, -137.6, -124.6, -122.8, -120.6, -121.4, -126.6, -130.2, -133.4, -123.6, -116.8, -112, -99.8, -87, -85.2, -82.8, -77.4, -83.8, -79.8, -78.2, -76.4, -79.8, -74.6, -71, -59.6, -50.4, -45.4, -34.2, -28.6, -37.6, -47.6, -53.6, -71.8, -71.8, -71.8, -104.4, -104.4, -104.4, -108.6, -118, -123.8, -126.4, -130.2, -137.4, -144.2, -147.2, -157.4, -164.8, -157, -143.6, -137.8, -131, -121.6, -115, -115.6, -117.4, -116.8, -123, -137, -149.2, -153.8, -151.8, -144.2, -137.6, -134.2, -128.2, -128, -131, -127.8, -127.2, -127, -121.2, -110.6, -101, -89.6, -92.8, -100.6, -110, -117.4, -123.2, -116.8, -117.2, -122.2, -130, -134, -145, -150, -147.8, -147, -151.6, -147.6, -123.8, -79, -8.6, 72.8, 129.8, 138.2, 94.6, 8.6, -87.2, -157.8, -195.2, -201.4, -192, -184.6, -181.8, -177.6, -177.6, -183.6, -183.8, -181.2, -182, -179.8, -162.6, -148.2, -137.4, -125.2, -117.8, -119.4, -116.8, -109, -106.6, -107, -105.8, -100.2, -95.6, -97.8, -90, -82.6, -82.4, -83, -72.2, -70.8, -70.8, -70.8, -83.2, -83.2, -83.2, -77.6, -78.4, -87.4, -94.2, -99.2, -112.2, -115, -108.4, -107.6, -117.2, -123.2, -125, -129, -126, -109, -95.6, -90.4, -77.6, -61.2, -56.2, -58.4, -58.4, -65.4, -84.8, -93.2, -97.4, -96.4, -88.2, -69.2, -54.2, -35, -24.4, -20.8, -23.2, -30, -37.6, -41, -47.8, -49.2, -46.4, -44.8, -51.4, -48.2, -47.4, -48.8, -49.6, -48, -49.6, -56, -62.2, -65, -62, -65.4, -59.6, -56.4, -54.4, -56.2, -39.2, 3, 81.8, 142.8, 179.4, 174.2, 124.2, 40.4, -20, -50, -49.2, -41.6, -34.8, -34.6, -34.8, -42, -43, -45, -40.6, -33, -26.8, -27.8, -18.2, -13, -13, -17, -15, -11.6, -0.2, 1.8, 14.2, 31.6, 33.2, 20.4, 16.8, 9, 5.6, 7.6, 7.6, 7.6, 0.4, 0.4, 0.4, -7.4, -15.2, -25, -34.6, -38.6, -41.2, -45.2, -52.4, -53.8, -57.2, -62.8, -62.6, -63, -61, -52.6, -40, -44.4, -41.8, -40.4, -42.6, -45.4, -47.8, -45.6, -36.6, -38.4, -47, -36.4, -32.6, -42.8, -37.4, -27.4, -18.2, -6, 4.6, 7.8, 8.8, 3.6, -2, -5, -10.2, -13.2, -14.4, -11.8, -2.2, 12.2, 30.6, 41.8, 50.2, 59.8, 66.4, 55.4, 52, 42.4, 24.4, 6.8, 4.2, -2, -10.2, -13.2, -14, -18.6, -21.4, -19.4, -13.4, 16.2, 77.6, 163, 245.2, 258, 226.8, 152.6, 53.2, -40.2, -67.6, -69.6, -55.2, -43.8, -30.4, -27.6, -31.8, -33.2, -30.2, -33.8, -33.4, -26.4, -22.2, -20.4, -14, -6.6, -4, 1.4, 2.8, 4.8, 13.2, 31.6, 31.6, 31.6,
//};
double input_data[9999] = {
        -5215, -5197, -5168, -5168, -5155, -5136, -5149, -5148, -5165, -5145, -5125, -5130, -5135, -5142, -5117, -5107, -5097, -5065, -5079, -5061, -5043, -5045, -5025, -5027, -5033, -5043, -5057, -5002, -5016, -5033, -5015, -4995, -4984, -4974, -4945, -4932, -4930, -4927, -4937, -4938, -4944, -4971, -4980, -4994, -4975, -4958, -4981, -4968, -4962, -4912, -4824, -4674, -4438, -4296, -4447, -4645, -4774, -4872, -4869, -4862, -4876, -4873, -4875, -4851, -4849, -4814, -4799, -4814, -4806, -4785, -4764, -4775, -4768, -4765, -4762, -4762, -4743, -4733, -4764, -4764, -4744, -4727, -4688, -4688, -4691, -4684, -4661, -4653, -4659, -4667, -4636, -4673, -4683, -4684, -4675, -4691, -4713, -4711, -4727, -4748, -4755, -4746, -4762, -4778, -4781, -4774, -4759, -4763, -4745, -4743, -4754, -4769, -4758, -4761, -4749, -4756, -4732, -4720, -4720, -4732, -4727, -4715, -4711, -4710, -4720, -4716, -4678, -4670, -4641, -4576, -4537, -4543, -4562, -4554, -4567, -4599, -4584, -4584, -4578, -4560, -4575, -4579, -4594, -4588, -4590, -4596, -4604, -4611, -4617, -4603, -4606, -4604, -4597, -4608, -4612, -4593, -4621, -4609, -4617, -4631, -4627, -4628, -4608, -4606, -4589, -4586, -4575, -4615, -4641, -4629, -4631, -4645, -4626, -4629, -4635, -4639, -4631, -4645, -4625, -4543, -4246, -4076, -4000, -4091, -4293, -4484, -4570, -4599, -4600, -4600, -4587, -4595, -4593, -4581, -4577, -4587, -4572, -4556, -4554, -4545, -4543, -4539, -4548, -4558, -4534, -4522, -4528, -4527, -4507, -4525, -4508, -4493, -4469, -4461, -4478, -4471, -4431, -4414, -4419, -4432, -4462, -4458, -4459, -4496, -4516, -4525, -4537, -4576, -4577, -4570, -4592, -4615, -4621, -4617, -4622, -4631, -4650, -4654, -4659, -4616, -4625, -4627, -4624, -4622, -4637, -4657, -4644, -4615, -4621, -4652, -4652, -4638, -4649, -4637, -4617, -4611, -4629, -4608, -4603, -4615, -4624, -4624, -4647, -4670, -4679, -4688, -4684, -4658, -4648, -4631, -4650, -4662, -4636, -4643, -4643, -4641, -4637, -4632, -4606, -4610, -4630, -4647, -4651, -4618, -4606, -4610, -4616, -4633, -4657, -4627, -4626, -4626, -4616, -4614, -4620, -4635, -4615, -4575, -4595, -4628, -4634, -4636, -4635, -4622, -4626, -4638, -4647, -4639, -4663, -4659, -4640, -4576, -4423, -4210, -4071, -4152, -4367, -4534, -4594, -4626, -4638, -4629, -4626, -4628, -4631, -4645, -4663, -4665, -4640, -4607, -4596, -4609, -4616, -4615, -4608, -4594, -4570, -4589, -4605, -4605, -4579, -4555, -4534, -4540, -4553, -4531, -4491, -4477, -4475, -4482, -4492, -4477, -4484, -4501, -4515, -4522, -4493, -4457, -4453, -4478, -4506, -4504, -4537, -4538, -4523, -4543, -4551, -4544, -4523, -4504, -4508, -4500, -4497, -4502, -4512, -4502, -4499, -4494, -4463, -4455, -4442, -4445, -4429, -4447, -4464, -4452, -4449, -4481, -4468, -4454, -4487, -4502, -4498, -4483, -4475, -4467, -4471, -4469, -4483, -4487, -4490, -4477, -4487, -4484, -4476, -4504, -4511, -4519, -4510, -4501, -4496, -4497, -4500, -4501, -4519, -4516, -4516, -4520, -4507, -4512, -4510, -4532, -4535, -4525, -4505, -4499, -4504, -4522, -4506, -4499, -4482, -4476, -4513, -4545, -4554, -4535, -4532, -4521, -4508, -4482, -4466, -4414, -4389, -4310, -4138, -3897, -3710, -3674, -3817, -4031, -4263, -4299, -4292, -4305, -4310, -4305, -4324, -4313, -4302, -4328, -4316, -4318, -4325, -4314, -4277, -4308, -4307, -4270, -4279, -4288, -4246, -4242, -4255, -4251, -4241, -4245, -4245, -4251, -4238, -4222, -4221, -4225, -4229, -4248, -4256, -4250, -4305, -4312, -4328, -4332, -4351, -4362, -4372, -4372, -4408, -4450, -4461, -4496, -4496, -4495, -4501, -4511, -4518, -4510, -4522, -4525, -4514, -4544, -4557, -4530, -4512, -4499, -4508, -4489, -4476, -4509, -4497, -4480, -4472, -4482, -4480, -4492, -4504, -4481, -4487, -4488, -4508, -4517, -4506, -4504, -4525, -4545, -4533, -4537, -4558, -4552, -4571, -4596, -4599, -4572, -4621, -4628, -4641, -4642, -4643, -4654, -4666, -4675, -4690, -4680, -4698, -4699, -4707, -4715, -4739, -4740, -4720, -4735, -4757, -4738, -4747, -4754, -4748, -4744, -4741, -4741, -4714, -4748, -4764, -4775, -4796, -4799, -4792, -4777, -4785, -4786, -4791, -4796, -4783, -4722, -4581, -4370, -4180, -4120, -4214, -4597, -4701, -4746, -4738, -4740, -4740, -4746, -4762, -4763, -4757, -4744, -4743, -4736, -4725, -4730, -4722, -4737, -4719, -4734, -4740, -4728, -4706, -4701, -4720, -4718, -4705, -4685, -4637, -4653, -4666, -4662, -4655, -4662, -4644, -4650, -4650, -4643, -4677, -4664, -4650, -4669, -4697, -4728, -4736, -4739, -4798, -4814, -4798, -4789, -4803, -4826, -4848, -4850, -4833, -4820, -4846, -4860, -4864, -4845, -4832, -4818, -4798, -4808, -4815, -4788, -4787, -4789, -4789, -4812, -4809, -4805, -4793, -4786, -4798, -4777, -4776, -4771, -4776, -4785, -4778, -4784, -4757, -4754, -4775, -4783, -4794, -4789, -4780, -4766, -4758, -4793, -4789, -4775, -4770, -4796, -4790, -4774, -4766, -4771, -4743, -4753, -4765, -4752, -4719, -4769, -4738, -4741, -4729, -4747, -4730, -4689, -4675, -4669, -4654, -4662, -4670, -4694, -4651, -4648, -4674, -4686, -4669, -4692, -4679, -4665, -4622, -4680, -4644, -4519, -4308, -4079, -3977, -4036, -4258, -4487, -4618, -4614, -4615, -4598, -4603, -4622, -4616, -4624, -4621, -4600, -4591, -4587, -4592, -4583, -4560, -4573, -4576, -4548, -4549, -4545, -4532, -4533, -4515, -4510, -4504, -4499, -4494, -4446, -4441, -4427, -4432, -4432, -4444, -4445, -4436, -4450, -4485, -4497, -4475, -4486, -4488, -4515, -4530, -4538, -4551, -4575, -4610, -4628, -4652, -4632, -4623, -4602, -4593, -4586, -4620, -4617, -4617, -4596, -4607, -4599, -4605, -4599, -4605, -4623, -4612, -4612, -4607, -4584, -4589, -4611, -4623, -4591, -4613, -4615, -4601, -4582, -4584, -4580, -4574, -4591, -4603, -4570, -4568, -4577, -4577, -4576, -4584, -4597, -4584, -4575, -4567, -4570, -4560, -4559, -4570, -4577, -4558, -4559, -4545, -4508, -4528, -4515, -4525, -4524, -4539, -4538, -4537, -4520, -4524, -4529, -4521, -4539, -4537, -4518, -4516, -4497, -4498, -4473, -4485, -4507, -4509, -4509, -4509, -4517, -4506, -4531, -4497, -4503, -4509, -4513, -4500, -4452, -4298, -4099, -3897, -3990, -4207, -4393, -4458, -4484, -4483, -4487, -4481, -4495, -4515, -4497, -4481, -4500, -4521, -4515, -4496, -4486, -4469, -4496, -4506, -4502, -4478, -4481, -4493, -4496, -4460, -4442, -4439, -4421, -4400, -4426, -4385, -4350, -4338, -4364, -4366, -4359, -4360, -4357, -4384, -4422, -4439, -4441, -4426, -4447, -4485, -4489, -4487, -4503, -4528, -4538, -4537, -4557, -4550, -4570, -4567, -4568, -4536, -4546, -4554, -4554, -4536, -4539, -4515, -4509, -4529, -4532, -4523, -4515, -4513, -4505, -4478, -4506, -4508, -4489, -4479, -4487, -4495, -4492, -4475, -4465, -4432, -4412, -4416, -4437, -4437, -4428, -4422, -4445, -4445, -4455, -4478, -4462, -4442, -4442, -4441, -4432, -4448, -4441, -4422, -4417, -4437, -4436, -4427, -4416, -4421, -4418, -4408, -4419, -4399, -4375, -4377, -4370, -4361, -4378, -4382, -4403, -4357, -4330, -4332, -4322, -4311, -4308, -4294, -4277, -4295, -4336, -4326, -4333, -4355, -4354, -4362, -4352, -4366, -4364, -4366, -4370, -4290, -4126, -3914, -3751, -3728, -3846, -4073, -4347, -4373, -4355, -4381, -4400, -4414, -4412, -4417, -4413, -4411, -4418, -4425, -4421, -4411, -4392, -4368, -4381, -4384, -4382, -4378, -4390, -4395, -4400, -4379, -4368, -4370, -4339, -4334, -4321, -4313, -4307, -4302, -4294, -4296, -4293, -4301, -4336, -4320, -4329, -4318, -4308, -4317, -4336, -4370, -4403, -4452, -4493, -4482, -4481, -4492, -4509, -4518, -4509, -4514, -4492, -4503, -4518, -4526, -4489, -4466, -4498, -4517, -4501, -4472, -4491, -4482, -4461, -4472, -4485, -4469, -4452, -4454, -4415, -4441, -4451, -4420, -4394, -4409, -4436, -4420, -4397, -4368, -4338, -4330, -4335, -4342, -4324, -4326, -4325, -4337, -4327, -4313, -4339, -4346, -4336, -4318, -4322, -4328, -4296, -4309, -4339, -4346, -4347, -4344, -4335, -4340, -4345, -4332, -4325, -4335, -4335, -4337, -4337, -4337, -4323, -4321, -4293, -4280, -4287, -4262, -4244, -4245, -4266, -4273, -4302, -4341, -4343, -4346, -4352, -4346, -4348, -4338, -4349, -4360, -4312, -4005, -3787, -3675, -3736, -3957, -4224, -4317, -4336, -4340, -4357, -4376, -4378, -4366, -4371, -4391, -4378, -4355, -4357, -4364, -4348, -4325, -4332, -4316, -4300, -4300, -4303, -4303, -4297, -4314, -4295, -4264, -4250, -4249, -4224, -4214, -4222, -4236, -4230, -4212, -4217, -4216, -4206, -4225, -4238, -4235, -4264, -4279, -4297, -4308, -4325, -4340, -4329, -4346, -4375, -4386, -4357, -4365, -4387, -4397, -4388, -4387, -4377, -4380, -4393, -4400, -4393, -4365, -4371, -4397, -4380, -4363, -4351, -4374, -4365, -4369, -4369, -4361, -4354, -4370, -4367, -4378, -4370, -4383, -4373, -4357, -4350, -4359, -4340, -4350, -4357, -4362, -4368, -4381, -4356, -4362, -4368, -4380, -4362, -4346, -4369, -4393, -4368, -4385, -4383, -4372, -4372, -4372, -4389, -4352, -4359, -4393, -4374, -4369, -4382, -4394, -4383, -4383, -4384, -4394, -4387, -4381, -4383, -4365, -4353, -4351, -4343, -4332, -4333, -4329, -4306, -4295, -4311, -4332, -4350, -4357, -4341, -4338, -4341, -4355, -4339, -4348, -4358, -4360, -4293, -3970, -3779, -3689, -3743, -3957, -4175, -4289, -4305, -4302, -4333, -4331, -4327, -4333, -4339, -4331, -4349, -4354, -4340, -4299, -4301, -4296, -4283, -4284, -4300, -4279, -4251, -4252, -4246, -4216, -4233, -4245, -4247, -4236, -4208, -4195, -4202, -4175, -4185, -4188, -4189, -4195, -4190, -4213, -4215, -4212, -4250, -4251, -4280, -4281, -4297, -4315, -4330, -4353, -4354, -4388, -4398, -4392, -4383, -4402, -4374, -4376, -4381, -4398, -4365, -4372, -4368, -4378, -4361, -4354, -4365, -4372, -4359, -4325, -4335, -4335, -4334, -4335, -4332, -4333, -4339, -4350, -4337, -4321, -4337, -4351, -4346, -4332, -4343, -4345, -4336, -4356, -4347, -4342, -4336, -4337, -4348, -4343, -4336, -4348, -4358,
};
//double input_data[9999] = {
//	-5215, -5197, -5168, -5168, -5155, -5136, -5149, -5148, -5165, -5145, -5125, -5130, -5135, -5142, -5117, -5107, -5097, -5065, -5079, -5061, -5043, -5045, -5025, -5027, -5033, -5043, -5057, -5002, -5016, -5033, -5015, -4995, -4984, -4974, -4945, -4932, -4930, -4927, -4937, -4938, -4944, -4971, -4980, -4994, -4975, -4958, -4981, -4968, -4962, -4912, -4824, -4674, -4438, -4296, -4447, -4645, -4774, -4872, -4869, -4862, -4876, -4873, -4875, -4851, -4849, -4814, -4799, -4814, -4806, -4785, -4764, -4775, -4768, -4765, -4762, -4762, -4743, -4733, -4764, -4764, -4744, -4727, -4688, -4688, -4691, -4684, -4661, -4653, -4659, -4667, -4636, -4673, -4683, -4684, -4675, -4691, -4713, -4711, -4727, -4748, -4755, -4746, -4762, -4778, -4781, -4774, -4759, -4763, -4745, -4743, -4754, -4769, -4758, -4761, -4749, -4756, -4732, -4720, -4720, -4732, -4727, -4715, -4711, -4710, -4720, -4716, -4678, -4670, -4641, -4576, -4537, -4543, -4562, -4554, -4567, -4599, -4584, -4584, -4578, -4560, -4575, -4579, -4594, -4588, -4590, -4596, -4604, -4611, -4617, -4603, -4606, -4604, -4597, -4608, -4612, -4593, -4621, -4609, -4617, -4631, -4627, -4628, -4608, -4606, -4589, -4586, -4575, -4615, -4641, -4629, -4631, -4645, -4626, -4629, -4635, -4639, -4631, -4645, -4625, -4543, -4246, -4076, -4000, -4091, -4293, -4484, -4570, -4599, -4600, -4600, -4587, -4595, -4593, -4581, -4577, -4587, -4572, -4556, -4554, -4545, -4543, -4539, -4548, -4558, -4534, -4522, -4528, -4527, -4507, -4525, -4508, -4493, -4469, -4461, -4478, -4471, -4431, -4414, -4419, -4432, -4462, -4458, -4459, -4496, -4516, -4525, -4537, -4576, -4577, -4570, -4592, -4615, -4621, -4617, -4622, -4631, -4650, -4654, -4659, -4616, -4625, -4627, -4624, -4622, -4637, -4657, -4644, -4615, -4621, -4652, -4652, -4638, -4649, -4637, -4617, -4611, -4629, -4608, -4603, -4615, -4624, -4624, -4647, -4670, -4679, -4688, -4684, -4658, -4648, -4631, -4650, -4662, -4636, -4643, -4643, -4641, -4637, -4632, -4606, -4610, -4630, -4647, -4651, -4618, -4606, -4610, -4616, -4633, -4657, -4627, -4626, -4626, -4616, -4614, -4620, -4635, -4615, -4575, -4595, -4628, -4634, -4636, -4635, -4622, -4626, -4638, -4647, -4639, -4663, -4659, -4640, -4576, -4423, -4210, -4071, -4152, -4367, -4534, -4594, -4626, -4638, -4629, -4626, -4628, -4631, -4645, -4663, -4665, -4640, -4607, -4596, -4609, -4616, -4615, -4608, -4594, -4570, -4589, -4605, -4605, -4579, -4555, -4534, -4540, -4553, -4531, -4491, -4477, -4475, -4482, -4492, -4477, -4484, -4501, -4515, -4522, -4493, -4457, -4453, -4478, -4506, -4504, -4537, -4538, -4523, -4543, -4551, -4544, -4523, -4504, -4508, -4500, -4497, -4502, -4512, -4502, -4499, -4494, -4463, -4455, -4442, -4445, -4429, -4447, -4464, -4452, -4449, -4481, -4468, -4454, -4487, -4502, -4498, -4483, -4475, -4467, -4471, -4469, -4483, -4487, -4490, -4477, -4487, -4484, -4476, -4504, -4511, -4519, -4510, -4501, -4496, -4497, -4500, -4501, -4519, -4516, -4516, -4520, -4507, -4512, -4510, -4532, -4535, -4525, -4505, -4499, -4504, -4522, -4506, -4499, -4482, -4476, -4513, -4545, -4554, -4535, -4532, -4521, -4508, -4482, -4466, -4414, -4389, -4310, -4138, -3897, -3710, -3674, -3817, -4031, -4263, -4299, -4292, -4305, -4310, -4305, -4324, -4313, -4302, -4328, -4316, -4318, -4325, -4314, -4277, -4308, -4307, -4270, -4279, -4288, -4246, -4242, -4255, -4251, -4241, -4245, -4245, -4251, -4238, -4222, -4221, -4225, -4229, -4248, -4256, -4250, -4305, -4312, -4328, -4332, -4351, -4362, -4372, -4372, -4408, -4450, -4461, -4496, -4496, -4495, -4501, -4511, -4518, -4510, -4522, -4525, -4514, -4544, -4557, -4530, -4512, -4499, -4508, -4489, -4476, -4509, -4497, -4480, -4472, -4482, -4480, -4492, -4504, -4481, -4487, -4488, -4508, -4517, -4506, -4504, -4525, -4545, -4533, -4537, -4558, -4552, -4571, -4596, -4599, -4572, -4621, -4628, -4641, -4642, -4643, -4654, -4666, -4675, -4690, -4680, -4698, -4699, -4707, -4715, -4739, -4740, -4720, -4735, -4757, -4738, -4747, -4754, -4748, -4744, -4741, -4741, -4714, -4748, -4764, -4775, -4796, -4799, -4792, -4777, -4785, -4786, -4791, -4796, -4783, -4722, -4581, -4370, -4180, -4120, -4214, -4597, -4701, -4746, -4738, -4740, -4740, -4746, -4762, -4763, -4757, -4744, -4743, -4736, -4725, -4730, -4722, -4737, -4719, -4734, -4740, -4728, -4706, -4701, -4720, -4718, -4705, -4685, -4637, -4653, -4666, -4662, -4655, -4662, -4644, -4650, -4650, -4643, -4677, -4664, -4650, -4669, -4697, -4728, -4736, -4739, -4798, -4814, -4798, -4789, -4803, -4826, -4848, -4850, -4833, -4820, -4846, -4860, -4864, -4845, -4832, -4818, -4798, -4808, -4815, -4788, -4787, -4789, -4789, -4812, -4809, -4805, -4793, -4786, -4798, -4777, -4776, -4771, -4776, -4785, -4778, -4784, -4757, -4754, -4775, -4783, -4794, -4789, -4780, -4766, -4758, -4793, -4789, -4775, -4770, -4796, -4790, -4774, -4766, -4771, -4743, -4753, -4765, -4752, -4719, -4769, -4738, -4741, -4729, -4747, -4730, -4689, -4675, -4669, -4654, -4662, -4670, -4694, -4651, -4648, -4674, -4686, -4669, -4692, -4679, -4665, -4622, -4680, -4644, -4519, -4308, -4079, -3977, -4036, -4258, -4487, -4618, -4614, -4615, -4598, -4603, -4622, -4616, -4624, -4621, -4600, -4591, -4587, -4592, -4583, -4560, -4573, -4576, -4548, -4549, -4545, -4532, -4533, -4515, -4510, -4504, -4499, -4494, -4446, -4441, -4427, -4432, -4432, -4444, -4445, -4436, -4450, -4485, -4497, -4475, -4486, -4488, -4515, -4530, -4538, -4551, -4575, -4610, -4628, -4652, -4632, -4623, -4602, -4593, -4586, -4620, -4617, -4617, -4596, -4607, -4599, -4605, -4599, -4605, -4623, -4612, -4612, -4607, -4584, -4589, -4611, -4623, -4591, -4613, -4615, -4601, -4582, -4584, -4580, -4574, -4591, -4603, -4570, -4568, -4577, -4577, -4576, -4584, -4597, -4584, -4575, -4567, -4570, -4560, -4559, -4570, -4577, -4558, -4559, -4545, -4508, -4528, -4515, -4525, -4524, -4539, -4538, -4537, -4520, -4524, -4529, -4521, -4539, -4537, -4518, -4516, -4497, -4498, -4473, -4485, -4507, -4509, -4509, -4509, -4517, -4506, -4531, -4497, -4503, -4509, -4513, -4500, -4452, -4298, -4099, -3897, -3990, -4207, -4393, -4458, -4484, -4483, -4487, -4481, -4495, -4515, -4497, -4481, -4500, -4521, -4515, -4496, -4486, -4469, -4496, -4506, -4502, -4478, -4481, -4493, -4496, -4460, -4442, -4439, -4421, -4400, -4426, -4385, -4350, -4338, -4364, -4366, -4359, -4360, -4357, -4384, -4422, -4439, -4441, -4426, -4447, -4485, -4489, -4487, -4503, -4528, -4538, -4537, -4557, -4550, -4570, -4567, -4568, -4536, -4546, -4554, -4554, -4536, -4539, -4515, -4509, -4529, -4532, -4523, -4515, -4513, -4505, -4478, -4506, -4508, -4489, -4479, -4487, -4495, -4492, -4475, -4465, -4432, -4412, -4416, -4437, -4437, -4428, -4422, -4445, -4445, -4455, -4478, -4462, -4442, -4442, -4441, -4432, -4448, -4441, -4422, -4417, -4437, -4436, -4427, -4416, -4421, -4418, -4408, -4419, -4399, -4375, -4377, -4370, -4361, -4378, -4382, -4403, -4357, -4330, -4332, -4322, -4311, -4308, -4294, -4277, -4295, -4336, -4326, -4333, -4355, -4354, -4362, -4352, -4366, -4364, -4366, -4370, -4290, -4126, -3914, -3751, -3728, -3846, -4073, -4347, -4373, -4355, -4381, -4400, -4414, -4412, -4417, -4413, -4411, -4418, -4425, -4421, -4411, -4392, -4368, -4381, -4384, -4382, -4378, -4390, -4395, -4400, -4379, -4368, -4370, -4339, -4334, -4321, -4313, -4307, -4302, -4294, -4296, -4293, -4301, -4336, -4320, -4329, -4318, -4308, -4317, -4336, -4370, -4403, -4452, -4493, -4482, -4481, -4492, -4509, -4518, -4509, -4514, -4492, -4503, -4518, -4526, -4489, -4466, -4498, -4517, -4501, -4472, -4491, -4482, -4461, -4472, -4485, -4469, -4452, -4454, -4415, -4441, -4451, -4420, -4394, -4409, -4436, -4420, -4397, -4368, -4338, -4330, -4335, -4342, -4324, -4326, -4325, -4337, -4327, -4313, -4339, -4346, -4336, -4318, -4322, -4328, -4296, -4309, -4339, -4346, -4347, -4344, -4335, -4340, -4345, -4332, -4325, -4335, -4335, -4337, -4337, -4337, -4323, -4321, -4293, -4280, -4287, -4262, -4244, -4245, -4266, -4273, -4302, -4341, -4343, -4346, -4352, -4346, -4348, -4338, -4349, -4360, -4312, -4005, -3787, -3675, -3736, -3957, -4224, -4317, -4336, -4340, -4357, -4376, -4378, -4366, -4371, -4391, -4378, -4355, -4357, -4364, -4348, -4325, -4332, -4316, -4300, -4300, -4303, -4303, -4297, -4314, -4295, -4264, -4250, -4249, -4224, -4214, -4222, -4236, -4230, -4212, -4217, -4216, -4206, -4225, -4238, -4235, -4264, -4279, -4297, -4308, -4325, -4340, -4329, -4346, -4375, -4386, -4357, -4365, -4387, -4397, -4388, -4387, -4377, -4380, -4393, -4400, -4393, -4365, -4371, -4397, -4380, -4363, -4351, -4374, -4365, -4369, -4369, -4361, -4354, -4370, -4367, -4378, -4370, -4383, -4373, -4357, -4350, -4359, -4340, -4350, -4357, -4362, -4368, -4381, -4356, -4362, -4368, -4380, -4362, -4346, -4369, -4393, -4368, -4385, -4383, -4372, -4372, -4372, -4389, -4352, -4359, -4393, -4374, -4369, -4382, -4394, -4383, -4383, -4384, -4394, -4387, -4381, -4383, -4365, -4353, -4351, -4343, -4332, -4333, -4329, -4306, -4295, -4311, -4332, -4350, -4357, -4341, -4338, -4341, -4355, -4339, -4348, -4358, -4360, -4293, -3970, -3779, -3689, -3743, -3957, -4175, -4289, -4305, -4302, -4333, -4331, -4327, -4333, -4339, -4331, -4349, -4354, -4340, -4299, -4301, -4296, -4283, -4284, -4300, -4279, -4251, -4252, -4246, -4216, -4233, -4245, -4247, -4236, -4208, -4195, -4202, -4175, -4185, -4188, -4189, -4195, -4190, -4213, -4215, -4212, -4250, -4251, -4280, -4281, -4297, -4315, -4330, -4353, -4354, -4388, -4398, -4392, -4383, -4402, -4374, -4376, -4381, -4398, -4365, -4372, -4368, -4378, -4361, -4354, -4365, -4372, -4359, -4325, -4335, -4335, -4334, -4335, -4332, -4333, -4339, -4350, -4337, -4321, -4337, -4351, -4346, -4332, -4343, -4345, -4336, -4356, -4347, -4342, -4336, -4337, -4348, -4343, -4336, -4348, -4358,
//};
//double input_data[9999] = {
//	-47,-47,-47,-47,-47,-47,-47,-47,-47,-47,-47,-47,-47,-47,-47,-47,-47,-48,-47,-47,-47,-47,-47,-47,-47,-47,-47,-47,-47,-47,-44,-24,-65,-66,-53,-49,-49,-49,-49,-49,-49,-48,-49,-49,-49,-49,-49,-49,-50,-50,-51,-52,-53,-55,-57,-59,-60,-60,-58,-55,-52,-50,-49,-49,-49,-49,-49,-49,-49,-49,-49,-49,
//};
//stemi4
//double input_data[9999] = {
//	-1,0,0,0,0,-78,-78,-78,-78,-78,-78,-80,-79,-79,-79,-79,-79,-79,-78,-78,-78,-78,-79,-81,-82,-82,-82,-82,-82,-83,-80,-79,-41,-74,-75,-75,-66,-69,-69,-70,-70,-70,-70,-70,-69,-69,-69,-66,-66,-65,-62,-58,-58,-56,-52,-57,-63,-66,-71,-75,-76,-77,-77,-77,-77,-77,-77,-77,-77,-77,-77,-77,-78,-78,-78,-78,-78,-78,-78,-79,
//};
//double input_data[9999] = {
//	-0.06, -0.065, -0.06, -0.075, -0.065, -0.07, -0.07, -0.09, -0.08, -0.095, -0.08, -0.095, -0.08, -0.095, -0.085, -0.09, -0.09, -0.1, -0.085, -0.105, -0.09, -0.045, 0.005, 0.015, 0.045, 0.155, 0.14, 0.045, 0.005, -0.04, -0.085, -0.2, -0.195, -0.2, -0.2, -0.24, -0.13, 0.34, 1.155, 1.47, -0.155, -0.825, -0.59, -0.35, -0.155, -0.17, -0.14, -0.155, -0.115, -0.125, -0.09, -0.095, -0.065, -0.055, -0.015, -0.005, 0.035, 0.045, 0.09, 0.11, 0.15, 0.18, 0.205, 0.225, 0.23, 0.22, 0.235, 0.23, 0.2, 0.17, 0.12, 0.075, 0.04, 0.02, 0.005, -0.005, -0.005, -0.01, -0.015, -0.01, 0, -0.005, -0.02, -0.02, -0.015, -0.025, -0.035, -0.045, -0.045, -0.06, -0.065, -0.07, -0.055, -0.06, -0.08, -0.075, -0.09, -0.065, -0.075, -0.08, -0.08, -0.085, -0.09, -0.075, -0.085, -0.095, -0.105, -0.09, -0.095, -0.07, -0.045, 0.005, 0.03, 0.07, 0.195, 0.145, 0.05, -0.015, -0.025, -0.115, -0.18, -0.19, -0.185, -0.185, -0.215, 0.01, 0.575, 1.185, 0.96, -0.16, -0.74, -0.64, -0.35, -0.175, -0.145, -0.15, -0.145, -0.105, -0.12, -0.09, -0.085, -0.07, -0.04, -0.02, 0.005, 0.02, 0.045, 0.08, 0.115, 0.14, 0.165, 0.21, 0.19, 0.22, 0.21, 0.215, 0.215, 0.16, 0.135, 0.1, 0.05, 0.035, -0.005, 0.005, -0.005, -0.015, -0.02, -0.01, 0, -0.02, -0.005, -0.04, -0.035, -0.05, -0.04, -0.055, -0.075, -0.05, -0.075, -0.09, -0.08, -0.09, -0.08, -0.075, -0.085, -0.085, -0.095, -0.095, -0.105, -0.08, -0.115, -0.1, -0.095, -0.1, -0.075, -0.005, 0.01, 0.055, 0.185, 0.145, 0.075, -0.005, -0.025, -0.11, -0.175, -0.18, -0.22, -0.175, -0.24, -0.03, 0.53, 1.21, 1.22, -0.105, -0.795, -0.69, -0.375, -0.165, -0.155, -0.135, -0.15, -0.12, -0.12, -0.09, -0.095, -0.05, -0.05, -0.01, -0.005, 0.03, 0.08, 0.075, 0.145, 0.155, 0.2, 0.2, 0.24, 0.235, 0.245, 0.245, 0.215, 0.18, 0.15, 0.09, 0.035, 0.025, 0, -0.015, -0.03, 0, -0.02, -0.005, -0.025, -0.03, -0.015, -0.01, -0.035, -0.04, -0.05, -0.045, -0.04, -0.075, -0.065, -0.095, -0.085, -0.085, -0.09, -0.1, -0.1, -0.08, -0.105, -0.105, -0.1, -0.105, -0.095, -0.1, -0.09, -0.035, 0.01, 0.035, 0.07, 0.19, 0.125, 0.045, -0.035, -0.06, -0.155, -0.21, -0.225, -0.2, -0.215, -0.265, 0.07, 0.865, 1.495, 0.625, -0.44, -0.815, -0.6, -0.31, -0.175, -0.18, -0.165, -0.135, -0.135, -0.115, -0.105, -0.085, -0.06, -0.03, -0.02, 0.015, 0.05, 0.07, 0.105, 0.165, 0.2, 0.205, 0.235, 0.235, 0.265, 0.26, 0.235, 0.215, 0.17, 0.125, 0.09, 0.025, -0.015, -0.03, -0.035, -0.04, -0.04, -0.035, -0.04, -0.02, -0.04, -0.04, -0.03, -0.045, -0.065, -0.06, -0.06, -0.09, -0.08, -0.095, -0.085, -0.105, -0.09, -0.1, -0.105, -0.115, -0.1, -0.115, -0.11, -0.135, -0.095, -0.125, -0.105, -0.085, -0.015, 0.02, 0.03, 0.135, 0.16, 0.105, 0.03, -0.05, -0.05, -0.17, -0.205, -0.215, -0.175, -0.21, -0.215, 0.175, 1.09, 1.7, 0.335, -0.615, -0.705, -0.46, -0.175, -0.165, -0.145, -0.155, -0.115, -0.12, -0.105, -0.105, -0.085, -0.06, -0.015, -0.005, 0.02, 0.05, 0.08, 0.105, 0.18, 0.205, 0.24, 0.235, 0.27, 0.27, 0.28, 0.26, 0.23, 0.175, 0.135, 0.09, 0.045, 0, -0.005, -0.025, -0.025, -0.025, -0.02, 0, -0.005, -0.005, -0.005, -0.005, -0.01, -0.025, -0.02, -0.04, -0.04, -0.04, -0.05, -0.06, -0.05, -0.065, -0.06, -0.07, -0.06, -0.09, -0.08, -0.095, -0.085, -0.1, -0.09, -0.095, -0.085, -0.1, -0.075, -0.095, -0.08, -0.075, 0, 0.03, 0.035, 0.135, 0.145, 0.07, 0.015, -0.045, -0.055, -0.185, -0.19, -0.22, -0.18, -0.21, -0.185, 0.22, 1.15, 1.72, 0.155, -0.7, -0.66, -0.405, -0.15, -0.16, -0.135, -0.145, -0.12, -0.125, -0.09, -0.09, -0.055, -0.04, -0.015, 0, 0.03, 0.05, 0.095, 0.11, 0.165, 0.19, 0.23, 0.245, 0.255, 0.255, 0.265, 0.255, 0.23, 0.185, 0.145, 0.1, 0.06, 0.025, 0.025, 0.01, 0.01, 0.01, 0.015, 0.01, 0.025, 0.02, 0.02, 0.02, 0.015, 0, 0, -0.01, -0.015, -0.02, -0.03, -0.035, -0.035, -0.045, -0.04, -0.045, -0.045, -0.055, -0.045, -0.055, -0.045, -0.06, -0.05, -0.06, -0.055, -0.08, -0.065, -0.09, -0.075, -0.09, -0.07, -0.075, -0.065, -0.08, -0.065, -0.03, 0.025, 0.04, 0.065, 0.18, 0.145, 0.055, 0.015, -0.035, -0.06, -0.175, -0.17, -0.195, -0.175, -0.22, -0.115, 0.355, 1.24, 1.595, -0.115, -0.815, -0.565, -0.3, -0.13, -0.16, -0.13, -0.135, -0.11, -0.12, -0.09, -0.085, -0.055, -0.04, -0.005, 0, 0.04, 0.05, 0.095, 0.125, 0.17, 0.19, 0.22, 0.23, 0.245, 0.25, 0.26, 0.24, 0.22, 0.175, 0.135, 0.085, 0.055, 0.02, 0.015, 0.01, 0.01, 0, 0, 0, 0.01, 0.005, 0.02, 0.01, 0, -0.01, -0.015
//};
 struct SPoint {
    double x;
    double y;
};
#define PI 3.14159265
#define WGHT_PQR 0.70
#define WGHT_QRS 0.55
#define WGHT_RST 0.70
#define WGHT_PQS 0.60
#define WGHT_RQS 0.60
#define WGHT_QST 0.70

#define err_rate 0.30
#define DBG_MSG_LEVEL1
#define DBG_MSG_LEVEL2

struct SPoint P, Q, R, S, T, R_Peaks[50], ecg_data[1600];
struct SPQRSTPoint {
    struct SPoint P, Q, R, S, T;
    double PQ, PR, QRS, QT, ST;
    int status;
};
struct SPQRSTPoint pqrst[50];
struct SEcgFeature {
    struct SPoint P, Q, R, S, T;
    double pqr_ang, qrs_ang, rst_ang, pqs_ang, rqs_ang, qst_ang;
    double pq, qr, rs, st, qrs;
};
struct SPQRSTPoint  g_SStemi5[50], g_SStemi2[50], g_SStemi1[50], g_SSchema_Peaked[50], g_SSchema1[50];
int g_stemi5_detect_cnt = 0;
int g_stemi2_detect_cnt = 0;
int g_stemi1_detect_cnt = 0;
int g_schema1_detect_cnt = 0;
int g_schemapeaked_detect_cnt = 0;

int g_feature_cnt = 0;
int g_master_cnt = 0;
double g_best_match;
double g_worst_match;
double g_avg_match;
double g_avg_qrs;
int g_avg_cnt;
int g_RR_Window_Size = 60;
int g_foundcorrectRR_dist = false;
int g_avg_RPeak_Interval;
int g_org_RPeak_Interval;
int g_ecg_normal = true, g_stemi5_detected = false, g_stemi2_detected = false, g_stemi1_detected = false;
int g_schema1_detected = false, g_schemapeaked_detected = false;
double g_Heart_Rate;
int g_QTc_Val;
double find_angle(struct SPoint p0, struct SPoint p1, struct SPoint c);
#define STEMI5 5
#define STEMI2 2
#define STEMI1 1
#define SCHEMA1 6
#define SCHEMA_PEAKED 7

double find_angle(struct SPoint p0, struct SPoint p1, struct SPoint c) {
  double p0c, p1c, p0p1, val;
  p0c = sqrt(pow(c.x - p0.x, 2) +
             pow(c.y - p0.y, 2)); // p0->c (b)
  p1c = sqrt(pow(c.x - p1.x, 2) +
             pow(c.y - p1.y, 2)); // p1->c (a)
  p0p1 = sqrt(pow(p1.x - p0.x, 2) +
              pow(p1.y - p0.y, 2)); // p0->p1 (c)

  val = 180.0 / PI;
  return acos((p1c*p1c + p0c*p0c - p0p1*p0p1) / (2 * p1c*p0c))*val;
}

int AlreadyInPeak(int a, int b)
{
  int i;
  for (i = 0; i < g_peak_cnt; i++)
  {
    if (R_Peaks[i].y + g_RR_Window_Size >= b && R_Peaks[i].y - g_RR_Window_Size <= b)
      return 1;
  }

  return 0;
}
void init_data(int data_len)
{
  int i, j;
  g_avg_RPeak_Interval = 0;
  g_org_RPeak_Interval = 0;
  g_peak_cnt = 0;
  g_avg_R_Hight = 99999;
	g_ecg_data_cnt = data_len;

  g_avg_match = 0;
  g_worst_match = -1;
  g_best_match = 9999;
  g_avg_cnt = 1;
  g_feature_cnt = 0;
  g_master_cnt = 0;
  g_avg_qrs = 0;

  g_stemi1_detect_cnt = 0;
  g_stemi2_detect_cnt = 0;
  g_stemi5_detect_cnt = 0;
  g_schema1_detect_cnt = 0;
  g_schemapeaked_detect_cnt = 0;
  min_x = 99999;
  max_x = -99999;
  g_foundcorrectRR_dist = false;
  g_no_of_R_peak = 9;
  g_RR_Window_Size = 60;
  g_ecg_normal = true; g_stemi5_detected = false; g_stemi2_detected = false; g_stemi1_detected = false;
  g_schema1_detected = false; g_schemapeaked_detected = false;
	g_Heart_Rate = 0;
}
int MyComp(int i, int j)
{
  if (ecg_data[i].x > ecg_data[j].x)
    return true;
  else if (ecg_data[i].x == ecg_data[j].x && ecg_data[i].x < ecg_data[j].x)
    return true;
  else
    return false;
}
void MySwap(int i, int j)
{
  struct SPoint tmp;
  tmp = ecg_data[i];
  ecg_data[i] = ecg_data[j];
  ecg_data[j] = tmp;
}
int Partition(int l, int h)
{
  int i, j;
  i = l - 1;
  for (j = l; j <= h; j++)
  {
    if (MyComp(j, h))
    {
      i++;
      MySwap(i, j);
    }
  }
  i++;
  MySwap(i, h);
  return i;
}
void MyQSort(int low, int high)
{
  int mid;
  if (low < high)
  {
    mid = Partition(low, high);
    MyQSort(low, mid - 1);
    MyQSort(mid + 1, high);
  }
}
void copytotmp()
{
	int i, j, st, ed;

	for (i = 0; i < g_ecg_data_cnt; i++)
	{
		ecg_data_org_tmp[i] = ecg_data_org[i];
	}
}
void PrepareEcgData()
{
  int i, j, st, ed;

  for (i = 0; i < g_ecg_data_cnt; i++)
  {
    ecg_data[i].x = ecg_data_org[i];
    ecg_data[i].y = i;
  }
  // sort this data
  MyQSort(0, g_ecg_data_cnt - 1);
}

void _Extract_PQRST(int pos, int peak_no)
{
  int i, j, k;
  double minima = 99999;
  double maxima = -99999;
  int min_pos = 0, max_pos = 0;
  // get first left minima
  for (i = 0; i < LEFT_WINDOW_SIZE / 3; i++)
  {
    if (ecg_data_org[pos - i]<minima)
    {
      minima = ecg_data_org[pos - i];
      min_pos = pos - i;
      j = i;
    }
  }

  // store this as Q point
  pqrst[peak_no].Q.x = minima;
  pqrst[peak_no].Q.y = min_pos;

  minima = 99999;
  maxima = -99999;

  // get first left maxima
  for (i = j; i < LEFT_WINDOW_SIZE; i++)
  {
    if (ecg_data_org[pos - i]>maxima)
    {
      maxima = ecg_data_org[pos - i];
      max_pos = pos - i;
    }
  }

  // store this as P point
  pqrst[peak_no].P.x = maxima;
  pqrst[peak_no].P.y = max_pos;

  minima = 99999;
  maxima = -99999;
  // get first right minima
  for (i = 0; i < RIGHT_WINDOW_SIZE / 2; i++)
  {
    if (ecg_data_org[pos + i]<minima)
    {
      minima = ecg_data_org[pos + i];
      min_pos = pos + i;
      j = i;
    }
  }

  // store this as R point
  pqrst[peak_no].S.x = minima;
  pqrst[peak_no].S.y = min_pos;

  minima = 99999;
  maxima = -99999;

  // get first ritht maxima
  for (i = j; i < RIGHT_WINDOW_SIZE; i++)
  {
    if (ecg_data_org[pos + i]>maxima)
    {
      maxima = ecg_data_org[pos + i];
      max_pos = pos + i;
    }
  }

  // store this as S point
  pqrst[peak_no].T.x = maxima;
  pqrst[peak_no].T.y = max_pos;
}
int GetQTInterval(int peak_no)
{
  int i, j, k;
  double minima = 99999;
  double maxima = -99999;
  int pos1, pos2, st, ed;
	if (pqrst[peak_no].status == 0 || pqrst[peak_no].status == SCHEMA1 || pqrst[peak_no].status == STEMI5 || pqrst[peak_no].status == SCHEMA_PEAKED)
  {
    pos1 = pqrst[peak_no].T.y;
    pos2 = pqrst[peak_no].T.y;

    st = pos1;
		ed = pqrst[peak_no].R.y + g_avg_RPeak_Interval*0.50;
    // get the end of T wave
    for (i = st; i < ed; i++)
    {
      if (ecg_data_org[i]<minima)
      {
        minima = ecg_data_org[i];
        pos2 = i;
      }
    }

    return (pos2 - pqrst[peak_no].Q.y);
	}
	else if (pqrst[peak_no].status == STEMI1 || pqrst[peak_no].status == STEMI2)
	{
		if (pqrst[peak_no].status == STEMI1)
		{
			pos1 = g_SStemi1[peak_no].T.y;
			pos2 = g_SStemi1[peak_no].T.y;
			st = pos1;
			ed = g_SStemi1[peak_no].R.y + +g_avg_RPeak_Interval*0.50;
		}
		else if (pqrst[peak_no].status == STEMI2)
		{
			pos1 = g_SStemi2[peak_no].T.y;
			pos2 = g_SStemi2[peak_no].T.y;
			st = pos1;
			ed = g_SStemi2[peak_no].R.y + g_avg_RPeak_Interval*0.50;
		}


		// get the end of T wave
		for (i = st; i < ed; i++)
		{
			if (ecg_data_org[i]>maxima)
			{
				maxima = ecg_data_org[i];
				pos2 = i;
			}
		}
		return (pos2 - pqrst[peak_no].Q.y);

	}
  return 0;
}
int GetPRInterval(int peak_no)
{
	int i, j, k;
	double minima = 99999;
	double maxima = -99999;
	int pos1, pos2, st, ed;
	pos1 = pqrst[peak_no].P.y;
	pos2 = pqrst[peak_no].P.y;

	st = pos1;
	ed = pqrst[peak_no].R.y - LEFT_WINDOW_SIZE;
	// get the start of P wave
	for (i = st; i < ed; i++)
	{
		if (ecg_data_org[i]<minima)
		{
			minima = ecg_data_org[i];
			pos2 = i;
		}
	}
	return (pqrst[peak_no].R.y - pos2);
}
void Extract_PQRST()
{
  int i, j, k;

  for (i = 0; i < g_peak_cnt; i++)
  {
    _Extract_PQRST(R_Peaks[i].y, i);
    // store R point
    pqrst[i].R.x = R_Peaks[i].x;
    pqrst[i].R.y = R_Peaks[i].y;
    pqrst[i].status = 0;

  }
}
int AlreadyInEcgPeak(int r_peak)
{
  int i;
  double tmp_diff;
  for (i = 0; i < g_peak_cnt; i++)
  {
    if (R_Peaks[i].y > ecg_data[r_peak].y)
    {
      if ((R_Peaks[i].y - ecg_data[r_peak].y) <= g_RR_Window_Size)
        return 1;
    }
    else
    {
      if ((ecg_data[r_peak].y - R_Peaks[i].y) <= g_RR_Window_Size)
        return 1;
    }
  }

  return 0;
}
int IsSharpCone(int r_peak)
{
  int i, cnt = 0;

  if (ecg_data_org[r_peak - 1] < ecg_data_org[r_peak] && ecg_data_org[r_peak + 1] < ecg_data_org[r_peak])
  {
    if (ecg_data_org[r_peak - 2] < ecg_data_org[r_peak] && ecg_data_org[r_peak + 2] < ecg_data_org[r_peak])
      return true;
  }

  return false;
}
int IsEcgPeakValied(int r_peak)
{
  if (ecg_data[r_peak].y < LEFT_WINDOW_SIZE)
    return 0;
  if ((g_ecg_data_cnt - ecg_data[r_peak].y) < RIGHT_WINDOW_SIZE)
    return 0;
  //	if (!IsSharpCone(ecg_data[r_peak].y))
  //	{
  //		return 0;
  //	}
  if (g_peak_cnt == 0)
  {
    return 1;
  }
  else
  {
    if (AlreadyInEcgPeak(r_peak))
    {
      return 0;
    }
  }

  return 1;
}
double getABS(double a, double b)
{
  if (a > b)
    return (a - b);
  else
    return (b - a);
}
int getNeighbourRPeak_latest(int pos)
{
	int left_dist, right_dist, left_dist1 = 30, right_dist1 = 30, left_dist2 = 30, right_dist2 = 30;
	double left_maxima, right_maxima, left_maxima1 = -99999, left_maxima2 = -99999, right_maxima1 = -99999, right_maxima2 = -99999;
	double tmp, tmp1, tmp2, d1, d2, d3, d4;
	int i;
	copytotmp();
	d1 = 0;
	for (i = left_dist1; i < 250; i++)
	{
		if ((pos - i) > LEFT_WINDOW_SIZE && left_maxima1 < ecg_data_org_tmp[pos - i])
		{
			left_maxima1 = ecg_data_org_tmp[pos - i];
			left_dist1 = pos - i;
		}
	}
	d1 = pos - left_dist1;
	// disable this first maxima to check for second maxima
	for (i = 0; i < 10; i++)
	{
		ecg_data_org_tmp[left_dist1 + i] = -99999;
		ecg_data_org_tmp[left_dist1 - i] = -99999;
	}

	for (i = left_dist2; i < 250; i++)
	{
		if ((pos - i) > LEFT_WINDOW_SIZE && left_maxima2 < ecg_data_org_tmp[pos - i])
		{
			left_maxima2 = ecg_data_org_tmp[pos - i];
			left_dist2 = pos - i;
		}
	}
	d2 = pos - left_dist2;
	left_dist = d1;
	left_maxima = left_maxima1;
	//	tmp2 = getABS(left_maxima2, ecg_data_org[pos]);
	//	tmp1 = getABS(left_maxima1, ecg_data_org[pos]);
	//	tmp2 = tmp1 / tmp2;
	if (d1 > d2 && IsSharpCone(left_dist2))
	{
		tmp1 = d1 / d2;
		if (tmp1 > 1.85 && tmp1 < 2.15)
		{
			left_dist = d2;
			left_maxima = left_maxima2;
		}
	}

	copytotmp();
	for (i = right_dist1; i < 250; i++)
	{
		if (g_ecg_data_cnt - (pos + i) > RIGHT_WINDOW_SIZE && right_maxima1 < ecg_data_org_tmp[pos + i])
		{
			right_maxima1 = ecg_data_org_tmp[pos + i];
			right_dist1 = pos + i;
		}
	}
	d3 = right_dist1 - pos;
	// disable this first maxima to check for second maxima
	for (i = 0; i < 10; i++)
	{
		ecg_data_org_tmp[right_dist1 + i] = -99999;
		ecg_data_org_tmp[right_dist1 - i] = -99999;
	}
	for (i = right_dist2; i < 250; i++)
	{
		if (g_ecg_data_cnt - (pos + i) > RIGHT_WINDOW_SIZE && right_maxima2 < ecg_data_org_tmp[pos + i])
		{
			right_maxima2 = ecg_data_org_tmp[pos + i];
			right_dist2 = pos + i;
		}
	}
	d4 = right_dist2 - pos;
	right_dist = d3;
	right_maxima = right_maxima1;
	//	tmp2 = getABS(right_maxima2, ecg_data_org[pos]);
	//	tmp1 = getABS(right_maxima1, ecg_data_org[pos]);
	//	tmp2 = tmp1 / tmp2;

	if (d3 > d4 && IsSharpCone(right_dist2))
	{
		tmp1 = d3 / d2;
		if (tmp1 > 1.85 && tmp1 < 2.15)
		{
			right_dist = d4;
			right_maxima = right_maxima2;
		}
	}
	tmp1 = getABS(left_maxima, ecg_data_org[pos]);
	tmp2 = getABS(right_maxima, ecg_data_org[pos]);

	for (i = 1; i <= 500; i = i + 5)
	{
		if (left_dist >= 40 && tmp1 <= i)
			return left_dist;
		if (right_dist >= 40 && tmp2 <= i)
			return right_dist;
	}

	return 300;
}
int getNeighbourRPeak(int pos)
{
  int left_dist = 30, right_dist = 30;
  double left_maxima = -99999, right_maxima = -99999;
  double tmp1, tmp2;
  int i;

  for (i = left_dist; i < 250; i++)
  {
    if ((pos - i) > LEFT_WINDOW_SIZE && left_maxima < ecg_data_org[pos - i])
    {
      left_maxima = ecg_data_org[pos - i];
      left_dist = pos - i;
    }
  }
  left_dist = pos - left_dist;
  tmp1 = getABS(left_maxima, ecg_data_org[pos]);
  for (i = right_dist; i < 250; i++)
  {
    if (g_ecg_data_cnt - (pos + i) > RIGHT_WINDOW_SIZE && right_maxima < ecg_data_org[pos + i])
    {
      right_maxima = ecg_data_org[pos + i];
      right_dist = pos + i;
    }
  }
  right_dist = right_dist - pos;
  tmp2 = getABS(right_maxima, ecg_data_org[pos]);

  for (i = 1; i <= 500; i=i+5)
  {
    if (left_dist >= 40 && tmp1 <= i)
      return left_dist;
    if (right_dist >= 40 && tmp2 <= i)
      return right_dist;
  }

  return 300;
}
void Extract_RPeaks()
{
    int i, j;
    int dist;
    for (i = 0; i < g_ecg_data_cnt; i++)
    {
        if (IsEcgPeakValied(i) && g_peak_cnt <= g_no_of_R_peak)
        {
            R_Peaks[g_peak_cnt].x = ecg_data[i].x;
            R_Peaks[g_peak_cnt].y = ecg_data[i].y;
            g_peak_cnt++;

            if (!g_foundcorrectRR_dist)
            {
                dist = getNeighbourRPeak_latest(ecg_data[i].y);
                if (dist > 80 && dist < 250)
                {
                    g_org_RPeak_Interval = dist;
                    g_RR_Window_Size = dist - 20;
                    g_foundcorrectRR_dist = true;
                }
                /*
                if (R_Peaks[0].y > R_Peaks[1].y)
                {
                g_RR_Window_Size = R_Peaks[0].y - R_Peaks[1].y - 20;
                }
                else
                {
                g_RR_Window_Size = R_Peaks[1].y - R_Peaks[0].y - 20;

                }
                */
            }


        }
    }
}
void Extract_RPeaks_ver1()
{
    int i, j;
    int dist;
    g_peak_cnt = 7;
    for(i=0;i<g_peak_cnt;i++)
    {
        j = (int)g_qrs_i_raw[0].data[i+1];

        R_Peaks[i].y = j;
        R_Peaks[i].x = ecg_data_org[j];

    }
}
void sortRPeaks()
{
  int i, j;
  struct SPoint tmp;
  for (i = 0; i < g_peak_cnt - 1; i++)
  {
    for (j = 0; j < g_peak_cnt - i - 1; j++)
    {
      if (R_Peaks[j].y > R_Peaks[j + 1].y)
      {
        tmp = R_Peaks[j];
        R_Peaks[j] = R_Peaks[j + 1];
        R_Peaks[j + 1] = tmp;
      }

    }
  }
}
double getlen(struct SPoint p, struct SPoint q)
{
  double ret;

  ret = sqrt((q.y - p.y)*(q.y - p.y) + (q.x - p.x)*(q.x - p.x));
  return ret;
}
int STEMI5_Detected(int pos, int peak_no)
{
  int i, j, k;
  double minima = 99999;
  double maxima = -99999;
  double Q_R, R_S, tmp1;
  int min_pos = 0, max_pos = 0;
  // get first right minima
  for (i = 0; i < RIGHT_WINDOW_SIZE / 2; i++)
  {
    if (ecg_data_org[pos + i]<minima)
    {
      minima = ecg_data_org[pos + i];
      min_pos = pos + i;
      j = i;
    }
  }

  // get first ritht maxima
  for (i = j; i < RIGHT_WINDOW_SIZE; i++)
  {
    if (ecg_data_org[pos + i]>maxima)
    {
      maxima = ecg_data_org[pos + i];
      max_pos = pos + i;
    }
  }
  Q_R = getlen(pqrst[peak_no].Q, pqrst[peak_no].R);
  R_S = getlen(pqrst[peak_no].R, pqrst[peak_no].S);

  if (Q_R > R_S && pqrst[peak_no].S.x > pqrst[peak_no].Q.x && pqrst[peak_no].T.x > pqrst[peak_no].Q.x && min_pos < max_pos && minima < maxima)
  {
    tmp1 = R_S / Q_R;
    if (tmp1 <= 0.50)
      return true;
  }
  return false;
}
int LeadV2DataRPeakCorrect(int peak_no)
{
    int i,j;
    double minima = 99999;
    double tmp, tmp1, tmp2,d1,d2;
    int cnt = 0;
    int pos;
    minima = 99999;
    pos = pqrst[peak_no].R.y;
    for (j = 0; j < 60; j++)
    {
        if (minima > ecg_data_org[pos - j] && (pos - j) > 60)
        {
            minima = ecg_data_org[pos - j];
        }
    }
    tmp1 = pqrst[peak_no].S.x;
    tmp2 = pqrst[peak_no].R.x;

    if (tmp1 > minima && tmp1 < tmp2)
    {
        d1 = tmp2 - tmp1;
        d2 = tmp1 - minima;
        tmp = d1 / d2;
        if (tmp > 0.60 && tmp < 2)
        {
            return false;
        }
    }
    return true;
}
int STEMI2_Detected(int pos, int peak_no)
{
    int i, j, k;
    double minima1 = 99999;
    double minima2 = 99999;
    double maxima = -99999;
    double max_pos;

    int min_pos1 = 0, min_pos2 = 0;
    // get first right minima
    // check if it's lead V2 data
    if (!g_withtompkin && !LeadV2DataRPeakCorrect(peak_no))
    {
        return false;
    }
    for (i = 0; i < RIGHT_WINDOW_SIZE / 2; i++)
    {
        if (ecg_data_org[pos + i]<minima1)
        {
            minima1 = ecg_data_org[pos + i];
            min_pos1 = pos + i;
            j = i;
        }
    }
    // get first right maxima
    for (i = j; i < RIGHT_WINDOW_SIZE; i++)
    {
        if (ecg_data_org[pos + i]>maxima)
        {
            maxima = ecg_data_org[pos + i];
            max_pos = pos + i;
            j = i;
        }
    }

    // get second minima
    for (i = j; i < RIGHT_WINDOW_SIZE; i++)
    {
        if (ecg_data_org[pos + i]<minima2)
        {
            minima2 = ecg_data_org[pos + i];
            min_pos2 = pos + i;
        }
    }
    if (pqrst[peak_no].S.x < pqrst[peak_no].Q.x && pqrst[peak_no].S.x < pqrst[peak_no].R.x)
    {
        if ((min_pos2 - min_pos1) > 10 && minima2 < pqrst[peak_no].Q.x)
        {
            // store this data
            g_SStemi2[peak_no].P = pqrst[peak_no].P;
            g_SStemi2[peak_no].Q = pqrst[peak_no].Q;
            g_SStemi2[peak_no].R = pqrst[peak_no].R;
            g_SStemi2[peak_no].S = pqrst[peak_no].S;
            g_SStemi2[peak_no].T.x = minima2;
            g_SStemi2[peak_no].T.y = min_pos2;
            pqrst[peak_no].status = STEMI2;
            return true;
        }
    }
    return false;
}int STEMI1_Detected(int pos, int peak_no)
{
  int i, j, k;
  double minima1 = 99999;
  double minima2 = 99999;
  double maxima = -99999;
  double max_pos;

  int min_pos1 = 0, min_pos2 = 0;
  // get first right minima
  for (i = 0; i < RIGHT_WINDOW_SIZE / 2; i++)
  {
    if (ecg_data_org[pos + i]<minima1)
    {
      minima1 = ecg_data_org[pos + i];
      min_pos1 = pos + i;
      j = i;
    }
  }
  // get first right maxima
  for (i = j; i < RIGHT_WINDOW_SIZE; i++)
  {
    if (ecg_data_org[pos + i]>maxima)
    {
      maxima = ecg_data_org[pos + i];
      max_pos = pos + i;
      j = i;
    }
  }

  // get second minima
  for (i = j; i < RIGHT_WINDOW_SIZE; i++)
  {
    if (ecg_data_org[pos + i]<minima2)
    {
      minima2 = ecg_data_org[pos + i];
      min_pos2 = pos + i;
    }
  }
  if (pqrst[peak_no].S.x < pqrst[peak_no].Q.x && pqrst[peak_no].S.x < pqrst[peak_no].R.x)
  {
    if ((min_pos2 - min_pos1) > 10 && minima2 < minima1 && minima2 < pqrst[peak_no].S.x)
    {
      // store this data
      g_SStemi1[peak_no].P = pqrst[peak_no].P;
      g_SStemi1[peak_no].Q = pqrst[peak_no].Q;
      g_SStemi1[peak_no].R = pqrst[peak_no].R;
      g_SStemi1[peak_no].S = pqrst[peak_no].S;
      g_SStemi1[peak_no].T.x = minima2;
      g_SStemi1[peak_no].T.y = min_pos2;
      pqrst[peak_no].status = STEMI1;
      return true;
    }
  }
  return false;
}
int SchemaPeaked_Detected(int pos, int peak_no)
{
  double hight_T, hight_R;
  if (pqrst[peak_no].T.x > pqrst[peak_no].S.x && pqrst[peak_no].T.x < pqrst[peak_no].R.x && pqrst[peak_no].T.x > pqrst[peak_no].Q.x)
  {
    hight_T = pqrst[peak_no].T.x - pqrst[peak_no].Q.x;
    hight_R = pqrst[peak_no].R.x - pqrst[peak_no].Q.x;
    if (hight_R / hight_T < 0.60)
    {
      g_SSchema_Peaked[peak_no].P = pqrst[peak_no].P;
      g_SSchema_Peaked[peak_no].Q = pqrst[peak_no].Q;
      g_SSchema_Peaked[peak_no].R = pqrst[peak_no].R;
      g_SSchema_Peaked[peak_no].S = pqrst[peak_no].S;
      g_SSchema_Peaked[peak_no].T = pqrst[peak_no].T;
      pqrst[peak_no].status = SCHEMA_PEAKED;
      return true;

    }
  }
  return false;
}
int Schema1_Detected(int pos, int peak_no)
{
  double angle, tmp_angle, multi_factor = 1;
  double mid, tmp1 = 0, avg;
  int i, cnt = 0, tx;
  struct SPoint p, q, c, st, ed;
  double ty;

  if (pqrst[peak_no].S.x > pqrst[peak_no].Q.x)
  {
    return 0;
  }
  else if ((pqrst[peak_no].Q.x - pqrst[peak_no].S.x) < 3)
  {
    return 0;
  }

  tmp1 = max_x - min_x;

  if (g_ecg_data_cnt > tmp1)
    multi_factor = g_ecg_data_cnt / tmp1;

  st.x = pqrst[peak_no].S.y;
  st.y = pqrst[peak_no].S.x*multi_factor;

  ed.x = pqrst[peak_no].T.y;
  ed.y = pqrst[peak_no].T.x*multi_factor;

  p = ed;
  c = st;
  q = st;
  q.x = 800;

  angle = find_angle(p, q, c);

  for (tx = pqrst[peak_no].S.y + 1; tx < pqrst[peak_no].T.y; tx++)
  {
    ty = ecg_data_org[tx];
    p.x = tx;
    p.y = ty*multi_factor;
    tmp_angle = find_angle(p, q, c);

    if (tmp_angle < angle && (angle - tmp_angle)>5)
      cnt++;
  }

  if (cnt / (ed.x - st.x) > 0.60)
  {
    g_SSchema1[peak_no].P = pqrst[peak_no].P;
    g_SSchema1[peak_no].Q = pqrst[peak_no].Q;
    g_SSchema1[peak_no].R = pqrst[peak_no].R;
    g_SSchema1[peak_no].S = pqrst[peak_no].S;
    g_SSchema1[peak_no].T = pqrst[peak_no].T;
    pqrst[peak_no].status = SCHEMA1;
    return true;
  }
  return false;
}
void STEMI_DETECTION()
{
  int i;
  double tmp1, tmp2;
  for (i = 0; i < g_peak_cnt; i++)
  {
    //_Extract_Feature(pqrst[i].P, pqrst[i].Q, pqrst[i].R, pqrst[i].S, pqrst[i].T);

    // detect for STEMI 5
    if (STEMI5_Detected(pqrst[i].R.y, i))
    {
      g_SStemi5[i].P = pqrst[i].P;
      g_SStemi5[i].Q = pqrst[i].Q;
      g_SStemi5[i].R = pqrst[i].R;
      g_SStemi5[i].S = pqrst[i].S;
      g_SStemi5[i].T = pqrst[i].T;
      pqrst[i].status = STEMI5;
      g_stemi5_detect_cnt++;
    }

    // detect stemi2
    if (STEMI2_Detected(pqrst[i].R.y, i))
    {
      g_stemi2_detect_cnt++;
    }
    // detect stemi1
    if (STEMI1_Detected(pqrst[i].R.y, i))
    {
      g_stemi1_detect_cnt++;
    }
    // detect schema peaked
    if (SchemaPeaked_Detected(pqrst[i].R.y, i))
    {
      g_schemapeaked_detect_cnt++;
    }
    if (Schema1_Detected(pqrst[i].R.y, i))
    {
      g_schema1_detect_cnt++;
    }
  }

  tmp2 = (double)g_peak_cnt;
  tmp1 = (double)g_stemi5_detect_cnt;
  tmp1 = tmp1 / tmp2;
  if (tmp1 > 0.70)
  {
    g_stemi5_detected = true;
    g_ecg_normal = false;
  }

  tmp2 = (double)g_peak_cnt;
  tmp1 = (double)g_stemi2_detect_cnt;
  tmp1 = tmp1 / tmp2;
  if (tmp1 > 0.70)
  {
    g_stemi2_detected = true;
    g_ecg_normal = false;
  }

  tmp2 = (double)g_peak_cnt;
  tmp1 = (double)g_stemi1_detect_cnt;
  tmp1 = tmp1 / tmp2;
  if (tmp1 > 0.70)
  {
    g_stemi1_detected = true;
    g_ecg_normal = false;
  }

  tmp2 = (double)g_peak_cnt;
  tmp1 = (double)g_schema1_detect_cnt;
  tmp1 = tmp1 / tmp2;
  if (tmp1 > 0.70)
  {
    g_schema1_detected = true;
    g_ecg_normal = false;
  }

  tmp2 = (double)g_peak_cnt;
  tmp1 = (double)g_schemapeaked_detect_cnt;
  tmp1 = tmp1 / tmp2;
  if (tmp1 > 0.70)
  {
    g_schemapeaked_detected = true;
    g_ecg_normal = false;
  }

}
int FeatureOK()
{
  int i, j, k;
  int success_cnt = 0;
  for (i = 0; i < g_peak_cnt; i++)
  {
    if (pqrst[i].P.x > pqrst[i].Q.x){}
  }
  return 0;
}
void copyto_orignal(double *data)
{
  int i;
  for (i = 0; i < g_ecg_data_cnt; i++)
  {
    if (min_x > data[i])
      min_x = data[i];
    if (max_x < data[i])
      max_x = data[i];

    ecg_data_org[i] = data[i];

  }
}
void getAvgRPeakInterval()
{
    int i, avg_dist = 0, avg_diff = 0, cnt = 0, tmp, tmp2;
    g_avg_RPeak_Interval = 0;

    for (i = 1; i < g_peak_cnt; i++)
    {
        tmp = R_Peaks[i].y - R_Peaks[i - 1].y;
        tmp2 = getABS(g_org_RPeak_Interval, tmp);
        if (tmp2 <= 10)
        {
            g_avg_RPeak_Interval = g_avg_RPeak_Interval + tmp;
            cnt++;
        }

    }
    if (cnt>0)
        g_avg_RPeak_Interval = g_avg_RPeak_Interval / cnt;

}
int getAvgRPeakInterval_ver1()
{
    int i, avg_dist = 0, avg_diff = 0, cnt = 0, tmp, tmp2;
    g_avg_RPeak_Interval = 0;

    for (i = 1; i < g_peak_cnt; i++)
    {


        tmp = R_Peaks[i].y - R_Peaks[i - 1].y;

        if(tmp <30)
            false;
        g_avg_RPeak_Interval = g_avg_RPeak_Interval + tmp;

    }
        g_avg_RPeak_Interval = g_avg_RPeak_Interval / (g_peak_cnt-1);


    return 0;
}
void CheckForEcgFinalReport()
{
  /*
  if (g_ecg_normal)
  {
    printf("\n\n ** NORMAL ECG **\cgn");
  }
  else
  {
    printf("\n\n ** ECG NOT NORMAL : ");
    if (g_stemi5_detected)
    {
      printf("STEMI5 DETECTED **\n");
    }
    else if (g_stemi2_detected)
    {
      printf("STEMI2 DETECTED **\n");
    }
    else if (g_stemi1_detected)
    {
      printf("STEMI1 DETECTED **\n");
    }
    else if (g_schema1_detected)
    {
      printf("SCHEMA1 DETECTED **\n");
    }
    else if (g_schemapeaked_detected)
    {
      printf("SCHEMAPEAKED DETECTED **\n");
    }
  }
*/
}
void GetAllInterval()
{
  int i;
  for (i = 0; i < g_peak_cnt; i++)
  {
    pqrst[i].QT = GetQTInterval(i);
		pqrst[i].PR = GetPRInterval(i);
		pqrst[i].QRS = pqrst[i].S.y - pqrst[i].Q.y;
  }
}
double GetHeartRate()
{
	g_Heart_Rate = 12000 / g_avg_RPeak_Interval;

	return g_Heart_Rate;
}
void CalculateQTcValue()
{
	int i;
	double qt_avg = 0, rr_peak_val;
	int tmp;
	g_QTc_Val = 0;
	for (i = 0; i < g_peak_cnt; i++)
	{
		qt_avg = qt_avg + pqrst[i].QT;
	}
	qt_avg = qt_avg / g_peak_cnt;

	qt_avg = (double)(qt_avg / 200);///2;
	qt_avg = 1000 * qt_avg;


	tmp = (int)g_Heart_Rate;

	g_QTc_Val = qt_avg + 1.75*(g_Heart_Rate - 60);

	//printf("g_ecg_data_len: %d g_Heart_Rate: %f g_QTc_Val: %d", g_ecg_data_cnt, g_Heart_Rate, g_QTc_Val);


}
void Extract_Features(double *data, int data_len)
{

//  double *ecgdata = pluto(data);
    init_data(data_len);
    // get the data length
    copyto_orignal(data);
    PrepareEcgData();
    Extract_RPeaks();
//    Extract_RPeaks_ver1();
    sortRPeaks();
    getAvgRPeakInterval();
//    getAvgRPeakInterval_ver1();
    Extract_PQRST();
    STEMI_DETECTION();
    GetAllInterval();
    CheckForEcgFinalReport();
    GetHeartRate();
    CalculateQTcValue();

}
void Extract_Features_withtompkin(double *data, int data_len)
{

//  double *ecgdata = pluto(data);
    init_data(data_len);
    // get the data length
    copyto_orignal(data);
    PrepareEcgData();
    //Extract_RPeaks();
    Extract_RPeaks_ver1();
    sortRPeaks();
//    getAvgRPeakInterval();
    getAvgRPeakInterval_ver1();
    Extract_PQRST();
    STEMI_DETECTION();
    GetAllInterval();
    CheckForEcgFinalReport();
    GetHeartRate();
    CalculateQTcValue();

}
void display_ecg_feature()
{
  int i;
//  double p_data[g_peak_cnt];
//  double q_data[g_peak_cnt];
//  double r_data[g_peak_cnt];
//  double s_data[g_peak_cnt];
//  double t_data[g_peak_cnt];
  for(i=0;i<g_peak_cnt;i++)
  {
//     p_data[i] = pqrst[i].P.y;
//     q_data[i] = pqrst[i].Q.y;
//     r_data[i] = pqrst[i].R.y;
//     s_data[i] = pqrst[i].S.y;
//     t_data[i] = pqrst[i].T.y;




  }


}
JNIEXPORT jint JNICALL Java_com_agatsa_sanket_ECGMonitorActivity_ExtractFeature(JNIEnv *env, jobject obj,jdouble data,jint data_len) {
  if (ecglength >= 1500){
    ecglength = 0;

      double t1[100];

      for ( int x=0; x< 1500;x=x+100)
      {
          for ( int y=0; y< 100 ; y++)
          {
              t1[y] = ECGSAMPLE[x+y] ;
          }
          double *t2 = cleanup(t1);
          for ( int y=0; y< 100 ; y++)
          {
              ECGSAMPLE[x+y] = t2[y] ;

          }
      }
    double *t5=  pluto(ECGSAMPLE);
    //  g_withtompkin = 0;

      if(g_withtompkin)
      {
          Extract_Features_withtompkin(t5,1500);
      }
      else
      {
          Extract_Features(t5,1500);
      }

    display_ecg_feature();
  }
  else {
    ECGSAMPLE[ecglength] = data;//(double)(((double)(ecgSample)));// (double)(((double)(ecgSample))/1000);
    //(double)(((double)(ecgSample)));//(double)(((double)(ecgSample))/1000);
    ecglength += 1;
    counter = 101;
    return NOTREADY;
  }

    return 0;
}
JNIEXPORT jint JNICALL Java_com_agatsa_sanket_ECGMonitorActivity_stemi1(JNIEnv *env, jobject obj){
  return g_stemi1_detect_cnt;
}
JNIEXPORT jint JNICALL Java_com_agatsa_sanket_ECGMonitorActivity_stemi2(JNIEnv *env, jobject obj){

  return g_stemi2_detect_cnt;
}
JNIEXPORT jint JNICALL Java_com_agatsa_sanket_ECGMonitorActivity_stemi5(JNIEnv *env, jobject obj){
  return g_stemi5_detect_cnt;
}
JNIEXPORT jint JNICALL Java_com_agatsa_sanket_ECGMonitorActivity_schema1(JNIEnv *env, jobject obj){
  return g_schema1_detect_cnt;
}
JNIEXPORT jint JNICALL Java_com_agatsa_sanket_ECGMonitorActivity_schemapeak(JNIEnv *env, jobject obj){

  return g_schemapeaked_detect_cnt;
}

double* cleanup2(double ECG[]) {
    double b_y[100],ECG1[100],ECG2[100];
    int sp = 7,diff=3;
    double ytemp[100];
    memset(&b_y[0], 0, 100U * sizeof(double));
    memset(&ytemp[0], 0, 100U * sizeof(double));

//    if(device!=sanket3)
//        sp=15;

    for(int i=0;i<100;i++)
    {
        ECG1[i]=ECG[i];
        ECG2[i]=ECG[i];
    }
    peakfinder(ECG2);


    for (i = 0; i < 5; i++) {
        b_x[i] = ECG[i];
    }

    for (i = 0; i < 2; i++) {
        xtmp = b_x[i];
        b_x[i] = b_x[4 - i];
        b_x[4 - i] = xtmp;
    }

    for (i = 0; i < 2; i++) {
        b_y[i] = 0.0;
        for (j = 0; j < 5; j++) {
            b_y[i] += 0.19999999999999998 * b_x[j];
        }
    }

    /*  Compute the steady state output */
    memset(&ytemp[0], 0, 100U * sizeof(double));
    for (i = 0; i < 5; i++) {
        for (j = i; j + 1 < 101; j++) {
            ytemp[j] += 0.19999999999999998 * ECG[j - i];
        }
    }

    memcpy(&b_y[2], &ytemp[4], 96U * sizeof(double));

    /*  Compute the transient off */
    for (i = 0; i < 5; i++) {
        b_x[i] = ECG[95 + i];
    }

    for (i = 0; i < 2; i++) {
        xtmp = b_x[i];
        b_x[i] = b_x[4 - i];
        b_x[4 - i] = xtmp;
    }

    for (i = 0; i < 2; i++) {
        b_y[98 + i] = 0.0;
        for (j = 0; j < 5; j++) {
            b_y[98 + i] += 0.19999999999999998 * b_x[j];
        }
    }

    memcpy(&y[0], &b_y[0], 100U * sizeof(double));

    // ********************** Shruti's Code ************************ //


    for (int x=0;x<100;x++)
    {
        if(peaks[0]==x||peaks[1]==x||peaks[2]==x||peaks[3]==x||peaks[4]==x||peaks[5]==x)
        {
            if(x!=0&&peaks[3]!=x&&peaks[4]!=x&&peaks[5]!=x)
            {
                double min=10000;
                for(int j= x-1;j>(x-sp)&&j-2>0;j--)
                {
                    // if (min > ECG1[j])
                    {
                        if(fabs(((ECG1[j]-ECG1[j-2])/2) - ((ECG1[j+2]-ECG1[j])/2))>160)
                        {
                            diff=(x-j)+1;
//            				break;
                        }
                        min = ECG1[j];
//						diff = j;
                        // diff=(x-j)+1;
                    }
                }
                for(int j= 0;j<diff;j++)
                {
                    if((x+j)<100)
                        ECG[x+j] = ECG1[x+j];
                    if((x-j)>0)
                        ECG[x-j] = ECG1[x-j];
                }
                x+=diff;
            } else
                ECG[x] = y[x];
        }
        else
            ECG[x] = y[x];

        //modification ends
        //ECG[x] = y[x];
    }

    // code for detection of small R-peaks


//    if (flagpeak1==1)
//        ECG[peaks[0]]=-1;
//    if (flagpeak2==1)
//        ECG[peaks[1]]=-1;
//    if (flagpeak3==1)
//        ECG[peaks[2]]=-1;

    flagpeak1=0;
    flagpeak2=0;
    flagpeak3=0;
    return ECG;
}



//double* cleanup(double ECG[]) {
//  double b_y[100];
//  double ytemp[100];
//  memset(&b_y[0], 0, 100U * sizeof(double));
//  memset(&ytemp[0], 0, 100U * sizeof(double));
//
//  for (i = 0; i < 5; i++) {
//    b_x[i] = ECG[i];
//  }
//
//  for (i = 0; i < 2; i++) {
//    xtmp = b_x[i];
//    b_x[i] = b_x[4 - i];
//    b_x[4 - i] = xtmp;
//  }
//
//  for (i = 0; i < 2; i++) {
//    b_y[i] = 0.0;
//    for (j = 0; j < 5; j++) {
//      b_y[i] += 0.19999999999999998 * b_x[j];
//    }
//  }
//
//  /*  Compute the steady state output */
//  memset(&ytemp[0], 0, 100U * sizeof(double));
//  for (i = 0; i < 5; i++) {
//    for (j = i; j + 1 < 101; j++) {
//      ytemp[j] += 0.19999999999999998 * ECG[j - i];
//    }
//  }
//
//  memcpy(&b_y[2], &ytemp[4], 96U * sizeof(double));
//
//  /*  Compute the transient off */
//  for (i = 0; i < 5; i++) {
//    b_x[i] = ECG[95 + i];
//  }
//
//  for (i = 0; i < 2; i++) {
//    xtmp = b_x[i];
//    b_x[i] = b_x[4 - i];
//    b_x[4 - i] = xtmp;
//  }
//
//  for (i = 0; i < 2; i++) {
//    b_y[98 + i] = 0.0;
//    for (j = 0; j < 5; j++) {
//      b_y[98 + i] += 0.19999999999999998 * b_x[j];
//    }
//  }
//
//  memcpy(&y[0], &b_y[0], 100U * sizeof(double));
//  for (int x=0;x<100;x++)
//    ECG[x] = y[x];
//
//  free(&b_y);
//  return ECG;
//}

JNIEXPORT jdoubleArray JNICALL Java_com_agatsa_sanket_ECGMonitorActivity_dll22(JNIEnv *env, jobject obj,jdoubleArray  array1)
{

  jdouble *srcArrayElems =
          (*env)->GetDoubleArrayElements(env, array1, NULL);

  jint nnn = (*env)->GetArrayLength(env, array1);

  jdoubleArray result = (*env)->NewDoubleArray(env, nnn);

  jdouble *destArrayElems =
          (*env)->GetDoubleArrayElements(env, result, NULL);

  for (int i = 0; i < ecg_len1; i++) {
    destArrayElems[i] = srcArrayElems[i];
    //  b_ecg_data_single[i] = destArrayElems[i];
  }

  double t1[100];

  for ( int x=0; x< 1800;x=x+100)
  {
    for ( int y=0; y< 100 ; y++)
    {
      t1[y] = destArrayElems[x+y] ;
    }
    double *t2 = cleanup(t1);
    for ( int y=0; y< 100 ; y++)
    {
      destArrayElems[x+y] = t2[y] ;

    }
  }


  jdoubleArray  st_array = (*env)->NewDoubleArray(env,(1800));
  (*env)->SetDoubleArrayRegion(env, st_array, 0, (1800), destArrayElems);
  return st_array;
}

JNIEXPORT jdoubleArray JNICALL Java_com_agatsa_sanket_ECGMonitorActivity_dll21(JNIEnv *env, jobject obj)
{
  jdoubleArray  st_array = (*env)->NewDoubleArray(env,(1800));
  (*env)->SetDoubleArrayRegion(env, st_array, 0, (1800), b_ecg_data_single);
  return st_array;
}
// start
JNIEXPORT jdoubleArray JNICALL Java_com_agatsa_sanket_ECGMonitorActivity_dll10(JNIEnv *env, jobject obj,jdoubleArray  array1)
{

  //bdac(array1);
  //return array1;
  emxArray_real_T *qrs_i_raw;
  emxArray_real_T *qrs_amp_raw;
  emxArray_real_T *noise;
  emxArray_real_T *sig_value;
  int ecg_size[1];
  ecg_size[0] = ecg_len1;//
  int b_ecg_size[1];
  b_ecg_size[0] = ecg_size[0];
  static double locs_data[ecg_len1];
  int locs_size[2];

  jdouble *srcArrayElems =
          (*env)->GetDoubleArrayElements(env, array1, NULL);

  jint nnn = (*env)->GetArrayLength(env, array1);

  jdoubleArray result = (*env)->NewDoubleArray(env, nnn);

  jdouble *destArrayElems =
          (*env)->GetDoubleArrayElements(env, result, NULL);

  for (int i = 0; i < 399; i++) {
    destArrayElems[i] = srcArrayElems[i];
    b_ecg_data_single[i] = destArrayElems[i];
  }

//for(int y=0;y<1800;y=y+399) {
//  for (int i = 0; i < 399; i++) {
//
//    destArrayElems[i] = srcArrayElems[i];
//    b_ecg_data_single[i+y] = destArrayElems[i];
//     b_ecg_data_single[i] = destArrayElems[i];
//   }
//  }
//
//  filterECG128Hz(b_ecg_data_single, b_ecg_data_new_single);
//
//  for (int i = 0; i < 399; i++) {
//    b_ecg_data_single[i] = b_ecg_data_new_single[i];
//  }

//  emxInit_real_T(&qrs_i_raw, 2);
////  emxInit_real_T(&g_rrinterval1, 2);
//  emxInit_real_T(&qrs_amp_raw, 2);
//  emxInit_real_T(&noise, 2);
//  emxInit_real_T(&sig_value,2);

  double ret_array[ecg_len1];
  pan_tompkin(b_ecg_data_single, b_ecg_size, qrs_amp_raw, qrs_i_raw, noise, sig_value, locs_data,
              locs_size);
// start
  grepeat = 0;
  int count_start = 0;
  int count_end = 0;
  int flag1 = 0;

  for (int x=0;x< qrs_i_raw->size[1];x++)
  {

  }

  for (int x=0; x<3;x++)
  {
    g_rrinterval1[x] =  qrs_i_raw->data[x+1] - qrs_i_raw->data[x];

    if (( g_rrinterval1[x]) < 70)
      grepeat = grepeat+ 1;
  }

  for (int x=0; x<3;x++)
  {
    g_rrvalue[x] = fabs( qrs_amp_raw->data[x+1] - qrs_amp_raw->data[x]);
    if (g_rrvalue[x] < 0) g_rrvalue[x] = g_rrvalue[x]* (-1);

    if (( g_rrvalue[x]) > 0.05)
      grepeat = grepeat+ 1;
  }

  //bdac(b_ecg_data_single);

  if(AvgQRSVal == 0) grepeat++;

//if (grepeat >0 ) return 0;



//  for(int x= 0; x< qrs_i_raw->size[1]-1;x++)
//  {
////    if (qrs_i_raw->data[x] < 300 ) count_start = x;//qrs_i_raw->data[x];
////    if (qrs_i_raw->data[x] < 800 ) count_end = x;//qrs_i_raw->data[x];
//    if ((qrs_i_raw->data[x+1] - qrs_i_raw->data[x]) < 100)
//
//    {
//      if (flag1 == 0) {
//        count_start = x + 1;
//        flag1 = 1;
//      }
//      else
//      {
//        count_end = x;
//        x= qrs_i_raw->size[1]-1;
//      }
//    }
//
//
//  }

//  __// android_log_print(// android_log_VERBOSE, APPNAME, "count_start 1 %d", count_start);
//  __// android_log_print(// android_log_VERBOSE, APPNAME, "count_end 1 %d", count_end);

//  if((count_start ==0) || (count_end ==0))
//  {
////    for(int x= 0; x< qrs_i_raw->size[1]-1;x++) {
////      if (qrs_i_raw->data[x] < 50) count_start = x;
////      //qrs_i_raw->data[x];
////      if (qrs_i_raw->data[x] < 380) count_end = x;//qrs_i_raw->data[x];
// //   }
//    count_start = 1;//qrs_i_raw->data[1];
//    count_end = 3;//qrs_i_raw->data[3];
//    __// android_log_print(// android_log_VERBOSE, APPNAME, "count_start 2 %d", count_start);
//    __// android_log_print(// android_log_VERBOSE, APPNAME, "count_end 2 %d", count_end);
//
//  }
//
//  count_start = 1;//qrs_i_raw->data[1];
//  count_end = 3;//qrs_i_raw->data[3];
//  __// android_log_print(// android_log_VERBOSE, APPNAME, "count_start %d", count_start);
//  __// android_log_print(// android_log_VERBOSE, APPNAME, "count_end %d", count_end);
//
//
//
//  __// android_log_print(// android_log_VERBOSE, APPNAME, "grepeat for rr interval %d", grepeat);


  for (int x=0;x<3;x++)
  {

    sig_val1 = sig_val1 + sig_value->data[x];

  }
  sig_val1 = sig_val1/3;

  emxArray_real_T *rrinterval1;
  emxInit_real_T(&rrinterval1, 2);
  b_diff(qrs_i_raw, rrinterval1);



  for (int i13 = 0; i13 < 3; i13++)
    rrinterval_array[i13] = fabs(rrinterval1->data[i13]);

  for ( int x =0; x<ecg_len1;x++)
    ret_array[x] = b_ecg_data_single[x];

  // start
  double min_noise1 = noise->data[0];
  int noise1_loc = 0;
  int a = 0;
  int c3 = 0;

  for (int x = 0; x < noise->size[1]; x++) {
    if (min_noise1 > noise->data[x]) {
      min_noise1 = noise->data[x];
      noise1_loc = locs_data[x];
    }
  }

  int start1 =0;
  int end1 = 0;
  int diff_1800 = 1800 - noise1_loc;// end = noise1_loc + diff_1800; start = end -600;
//  if (diff_1800 > 400) {
//    start1 = noise1_loc - 100;
//    end1 = noise1_loc + 400;
//  }
//  else
//  {
//    start1 = 400;
//    end1 = 800;
//  }
  c3 =0;

  for (int x = qrs_i_raw->data[0]-10; x < qrs_i_raw->data[3] ; x++) {
    g_l4s_data[c3] = b_ecg_data_single[x];
    c3++;
  }
//  b_ecg_size[0] = count_end-count_start;//400;
//  for ( int x=0;x< qrs_i_raw->data[qrs_i_raw->size[1]-1];x++)
//    qrs_i_raw->data[x] = 0;
//
//  qrs_i_raw->size[1] = 0;
//
//  pan_tompkin(g_l4s_data, b_ecg_size, qrs_amp_raw, qrs_i_raw, noise, sig_value, locs_data,
//              locs_size);
//

  for (int x=0;x<noise->size[1];x++)
  {
    s1 = s1 + noise->data[x];
  }
  s1= s1/(noise->size[1]);

  double t1[100];

  for ( int x=0; x< 399;x=x+100)
  {
    for ( int y=0; y< 100 ; y++)
    {
      t1[y] = g_l4s_data[x+y] ;
    }
    double *t2 = cleanup(t1);
    for ( int y=0; y< 100 ; y++)
    {
      g_l4s_data[x+y] = t2[y] ;

    }
  }

  int ecg_pos = 0;



  for (int x = 0; x < 399; x++) {
    g1_l4s_data[ecg_pos] = g_l4s_data[x];
    ecg_pos++;
  }
  c3 = 0;
  ecg_pos = 0;
  jdoubleArray l_l4s_data = (*env)->NewDoubleArray(env,400);
  (*env)->SetDoubleArrayRegion(env, l_l4s_data, 0, 400 , g1_l4s_data);

  return l_l4s_data;

}
// end

JNIEXPORT jdouble JNICALL Java_com_agatsa_sanket_ECGMonitorActivity_dll20(JNIEnv *env, jobject obj)
{


  return grepeat;

}

JNIEXPORT jdoubleArray JNICALL Java_com_agatsa_sanket_ECGMonitorActivity_dll1(JNIEnv *env, jobject obj,jdoubleArray  array1, jdoubleArray  array2, double ac1) {
  // static double a_b_ecg_data[2196];
  static double a_b_ecg2_data[ecg_len2];
//  double* a_b_ecg_data  = (*env)->GetDoubleArrayElements(env,array1,NULL);
  int ecg_size[1];
  ecg_size[0] = 400;//
  // ecg_len1 = 500;
  int b_ecg_size[1];
  b_ecg_size[0] = ecg_size[0];
  int loop_ub;
  int loop_ub2;
  int i13;
  loop_ub = ecg_size[0];
  loop_ub2 = ecg_size[0];
  // start
  double ecg_data[] = {0};
//int ecg_size[1];
  double ecg2_data[] = {0};
  int ecg2_size[1];
  double accuracy;

  double l4s_data[] = {0};
  int l4s_size[2];
  double l4s2_data[] = {0};
  int l4s2_size[2];
  double l4s3_data[] = {0};
  int l4s3_size[2];
  double *ecg3;
  emxArray_real_T *qrs_i_raw;
  emxArray_real_T *qrs_i_raw2;
  emxArray_real_T *qrs_i_raw3;
  double *t1;
  double *t2;
  double *t3;
  emxArray_real_T *rrinterval1;
  emxArray_real_T *rrinterval2;
  emxArray_real_T *rrinterval3;
//
//int b_ecg_size[1];

  emxArray_real_T *qrs_amp_raw;
  emxArray_real_T *noise;
  emxArray_real_T *sig_value;
  emxArray_real_T *qrs_amp_raw2;
  emxArray_real_T *noise2;
  emxArray_real_T *sig_value2;
  emxArray_real_T *qrs_amp_raw3;
  emxArray_real_T *noise3;
  static double locs_data[ecg_len2];
  int locs_size[2];
  static double f_y_data[ecg_len2];
  int b_ecg2_size[1];
// emxArray_real_T *noise2;
  static double locs2_data[ecg_len2];
  int y_size[2];
  int c_ecg_size[2];
  static double y_data[ecg_len2];
  double p[7];
  double expl_temp_data[49];
  double ecgx;
  double expl_temp;
  double mu[2];
  static double b_y_data[ecg_len2];
  int b_y_size[2];
  int n;
  static double ecgx_data[ecg_len2];
  int c_ecg2_size[2];
  int c_y_size[2];

  int len_qrs;
  int ixstart;
  int len_ecg;
  int d_ecg_size[1];
  static double b_locs_data[ecg_len2];
  int d_ecg2_size[1];
  int e_ecg_size[2];
  static double c_ecg_data[ecg_len2];
  static double d_ecg_data[ecg_len2];
  static double c_ecg2_data[ecg_len2];
//  emxArray_real_T *noise3;
  double ecg3_data[1];
  int ecg3_size[1];
  //double b_ecg3_data[1];
  int tmp_size[2];
  int b_tmp_size[2];
  boolean_T exitg3;
  signed char tmp_data[1];
  boolean_T exitg2;

  jdouble *srcArrayElems =
          (*env)->GetDoubleArrayElements(env, array1, NULL);

  jint nnn = (*env)->GetArrayLength(env, array1);

  jdoubleArray result = (*env)->NewDoubleArray(env, nnn);

  jdouble *destArrayElems =
          (*env)->GetDoubleArrayElements(env, result, NULL);


  for (int i = 0; i < ecg_len2; i++) {
    destArrayElems[i] = srcArrayElems[i];
    b_ecg_data[i] = destArrayElems[i];

  }

  jdouble *srcArrayElems2 =
          (*env)->GetDoubleArrayElements(env, array2, NULL);

  jint nnn2 = (*env)->GetArrayLength(env, array2);

  jdoubleArray result2 = (*env)->NewDoubleArray(env, nnn2);
  jdouble *destArrayElems2 =
          (*env)->GetDoubleArrayElements(env, result2, NULL);

  for (int i = 0; i < ecg_len2; i++) {
    destArrayElems2[i] = srcArrayElems2[i];
    b_ecg2_data[i] = destArrayElems2[i];

  }


  emxInit_real_T(&qrs_i_raw, 2);
  emxInit_real_T(&qrs_amp_raw, 2);
  emxInit_real_T(&noise, 2);
  emxInit_real_T(&sig_value, 2);
  pan_tompkin(b_ecg_data, b_ecg_size, qrs_amp_raw, qrs_i_raw, noise, sig_value, locs_data,
              locs_size);


  emxInit_real_T(&qrs_i_raw2, 2);
  emxInit_real_T(&qrs_amp_raw2, 2);
  emxInit_real_T(&noise2, 2);
  emxInit_real_T(&sig_value2, 2);

  pan_tompkin(b_ecg2_data, b_ecg_size, qrs_amp_raw2, qrs_i_raw2, noise2, sig_value2, locs2_data,
              locs_size);

  int c1 = 0;

  double z = 0;

  int index[] = {0};

  for(int x =0; x<400;x++)
  {
    b_ecg_data_new[x] = b_ecg_data[x];
    b_ecg2_data_new[x] = b_ecg2_data[x];
  }

  for (int x = 1; x < 4 ; x++) {



    z = (qrs_i_raw->data[x] - qrs_i_raw2->data[x]);



    if (z > 0) {


      index[x] = round((qrs_i_raw2->data[x - 1] + qrs_i_raw2->data[x]) / 2);

      for (int y = 0; y < z ; y++) {
        b_ecg2_data[index[x] + y] =  b_ecg2_data_new[index[x]] ;//-  (qrs_amp_raw2->data[x] *140);//b_ecg2_data[(int) (index[x]) - 2];
      }

      int c2x = 1;



      for (int x2 = index[x] + z+1 ; x2 < 400 ; x2++) {
        b_ecg2_data[x2] = b_ecg2_data_new[index[x] + c2x];
        c2x++;
      }
      c2x= 0;

//        for (int x = 0; x < 5; x++)
//          qrs_i_raw2->data[x] = qrs_i_raw2->data[x] + z;

//        pan_tompkin(b_ecg2_data, b_ecg_size, qrs_amp_raw2, qrs_i_raw2, noise2, sig_value, locs2_data,
//                    locs_size);




    }
    else {
      z = fabs(z);

      index[x] = (int) (round(qrs_i_raw->data[x - 1] + qrs_i_raw->data[x]) / 2);



      for (int y = 0; y < z ; y++) {

        b_ecg_data[index[x] + y] = b_ecg_data_new[index[x]] ;//-  (qrs_amp_raw->data[x] * 140);//b_ecg_data[(int) (index[x]) - 2];
      }

      int c2x = 1;


      for (int x2 = index[x] + z+1; x2 < 400 ; x2++) {
        b_ecg_data[x2] = b_ecg_data_new[index[x] + c2x];
        c2x++;
      }

//        pan_tompkin(b_ecg_data, b_ecg_size, qrs_amp_raw, qrs_i_raw, noise,sig_value, locs_data,
//                    locs_size);


    }
  }
//    for (int y = 0; y < len_qrs - 1; y++) {
//
//
//      if (qrs_i_raw->data[y] != qrs_amp_raw2->data[y])
//        okcount++ ;
//
//    }
//    if (okcount != 0)
//      okflag = 0;
//    else okflag = 1;


  int z1=0;
  if (qrs_i_raw->size[1] >= qrs_i_raw2->size[1]) {
    len_qrs = qrs_i_raw->size[1];
    z1 = len_qrs-1;
    c1 = qrs_i_raw->data[0] - 2;
    c2 = (qrs_i_raw->data[3] + 5);

  } else {
    len_qrs = qrs_i_raw2->size[1];
    z1 = len_qrs-1;
    c1 = qrs_i_raw2->data[0] - 2;
    c2 = (qrs_i_raw2->data[3] + 5);
  }

  /*
  for(int x=1;x<10;x++)
    if (abs((qrs_i_raw->data[x] - qrs_i_raw2->data[x]))<5)
      z1++;
  else
      break;
  */



//
//
//  double t11[100];
//
//  for ( int x=0; x< c2;x=x+100)
//  {
//    for ( int y=0; y< 100 ; y++)
//    {
//      t11[y] = b_ecg2_data[x+y] ;
//    }
//    double *t21 = cleanup(t11);
//    for ( int y=0; y< 100 ; y++)
//    {
//      b_ecg2_data[x+y] = t21[y] ;
//
//    }
//  }
//
//  double t12[100];
//
//  for ( int x=0; x< c2;x=x+100)
//  {
//    for ( int y=0; y< 100 ; y++)
//    {
//      t12[y] = b_ecg_data[x+y] ;
//    }
//    double *t22 = cleanup(t12);
//    for ( int y=0; y< 100 ; y++)
//    {
//      b_ecg_data[x+y] = t22[y] ;
//
//    }
//  }

  for (int x = c1; x < c2; x++) {
    b_ecg3_data[x] = b_ecg2_data[x] - b_ecg_data[x];
    l_avr[x] = -0.5 * ( b_ecg_data[x] + b_ecg2_data[x]);
    l_avf[x] = b_ecg2_data[x] - (0.5 * b_ecg_data[x]);
    l_avl[x] = b_ecg_data[x] - (0.5*b_ecg2_data[x] );

  }

//  for (int x = c3; x < ecg_len2; x++) {
//    b_ecg3_data[x] = 0;//b_ecg2_data[x] - b_ecg_data[x];
//    l_avr[x] = 0;//-0.5 * ( b_ecg_data[x] + b_ecg2_data[x]);
//    l_avf[x] = 0;//b_ecg2_data[x] - (0.5 * b_ecg_data[x]);
//    l_avl[x] = 0;//b_ecg_data[x] - (0.5*b_ecg2_data[x] );
//  }


//  double t11[100];
//
//  for ( int x=0; x< c2;x=x+100)
//  {
//    for ( int y=0; y< 100 ; y++)
//    {
//      t11[y] = b_ecg2_data[x+y] ;
//    }
//    double *t21 = cleanup(t11);
//    for ( int y=0; y< 100 ; y++)
//    {
//      b_ecg2_data[x+y] = t21[y] ;
//
//    }
//  }
//
//  double t12[100];
//
//  for ( int x=0; x< c2;x=x+100)
//  {
//    for ( int y=0; y< 100 ; y++)
//    {
//      t12[y] = b_ecg_data[x+y] ;
//    }
//    double *t22 = cleanup(t12);
//    for ( int y=0; y< 100 ; y++)
//    {
//      b_ecg_data[x+y] = t22[y] ;
//
//    }
//  }

/*
  for (int x = 0; x < c2; x++) {
    b_ecg3_data[x] = b_ecg2_data[x] - b_ecg_data[x];
    l_avr[x] = -0.5 * ( b_ecg_data[x] + b_ecg2_data[x]);
    l_avf[x] = b_ecg2_data[x] - (0.5 * b_ecg_data[x]);
    l_avl[x] = b_ecg_data[x] - (0.5*b_ecg2_data[x] );
  }

*/
  int c_pos =0;
  for (int x = c1; x < c2; x++) {

    g_l4s3_data[c_pos] = b_ecg3_data[x];
    g_l4s2_data[c_pos] = b_ecg2_data[x];
    g_l4s_data[c_pos] = b_ecg_data[x];
    l_avr[c_pos] = l_avr[x];
    l_avf[c_pos] = l_avf[x];
    l_avl[c_pos] = l_avl[x];
    c_pos++;

  }

  jdoubleArray l_l4s_data = (*env)->NewDoubleArray(env,ecg_len2);
  double temp[ecg_len2] = {0};

  for(int x=0;x<ecg_len2;x++)
    temp[x] = -1* b_ecg_data[x];

  (*env)->SetDoubleArrayRegion(env, l_l4s_data, 0, ecg_len2, temp);

  return l_l4s_data; //*g_l4s_data;

}



JNIEXPORT jint JNICALL Java_com_agatsa_sanket_ECGMonitorActivity_dll8(JNIEnv *env, jobject obj)
{
  return gNoiseValue;

  //return pos3Value;
}


JNIEXPORT jdoubleArray  JNICALL Java_com_agatsa_sanket_ECGMonitorActivity_dll12(JNIEnv *env, jobject obj)
{
  jdoubleArray l_int_data = (*env)->NewDoubleArray(env,50);

  for(int x=0;x<50;x++)
    g_rrinterval_data[x] = fabs(rrinterval_array[x]);

  (*env)->SetDoubleArrayRegion(env, l_int_data, 0, 50, g_rrinterval_data);

  return l_int_data; //*g_l4s_data;

}


JNIEXPORT jdoubleArray  JNICALL Java_com_agatsa_sanket_ECGMonitorActivity_dll2(JNIEnv *env, jobject obj)
{
  jdoubleArray l_l4s_data = (*env)->NewDoubleArray(env,500);

  for(int x=0;x<500;x++)
    g_l4s_data[x] = -1* g_l4s_data[x];

  (*env)->SetDoubleArrayRegion(env, l_l4s_data, 0, 500, g_l4s_data);

  return l_l4s_data; //*g_l4s_data;

}


JNIEXPORT jdoubleArray  JNICALL Java_com_agatsa_sanket_ECGMonitorActivity_dll11(JNIEnv *env, jobject obj)
{
  jdoubleArray l_l4s_data = (*env)->NewDoubleArray(env,500);

  double a[500];
  for(int x=0;x<500;x++)
    a[x] = g_l4s_data[x];//((g_l4s_data[x]-g_l4s2_data[x]));

  emxArray_real_T *qrs_amp_raw;
  emxArray_real_T *qrs_i_raw;
  emxArray_real_T *noise;
  emxArray_real_T * sig_value;
  emxInit_real_T(&qrs_i_raw, 2);
  emxInit_real_T(&qrs_amp_raw, 2);
  emxInit_real_T(&noise, 2);
  emxInit_real_T(&sig_value, 2);
  static double locs_data[ecg_len1];
  int b_ecg_size[1];
  int locs_size[2];
  b_ecg_size[0] = 500;
  pan_tompkin(a, b_ecg_size, qrs_amp_raw, qrs_i_raw, noise, sig_value, locs_data,
              locs_size);
  int c1 = 0;
  for (int x = qrs_i_raw->data[0]-5; x < qrs_i_raw->data[4]; x++)
  {
    a[c1] = g_l4s_data[x];
    c1++;
  }

  (*env)->SetDoubleArrayRegion(env, l_l4s_data, 0, 500, a);

  return l_l4s_data; //*g_l4s_data;

}




JNIEXPORT jdoubleArray  JNICALL Java_com_agatsa_sanket_ECGMonitorActivity_dll3(JNIEnv *env, jobject obj)
{
  jdoubleArray l_l4s2_data = (*env)->NewDoubleArray(env,401);

  double a[401];
  for(int x=0;x<401;x++)
    a[x] = g_l4s2_data[x];//((g_l4s_data[x]-g_l4s2_data[x]));

  emxArray_real_T *qrs_amp_raw;
  emxArray_real_T *qrs_i_raw;
  emxArray_real_T *noise;
  emxArray_real_T *sig_value;
  emxInit_real_T(&qrs_i_raw, 2);
  emxInit_real_T(&qrs_amp_raw, 2);
  emxInit_real_T(&noise, 2);
  emxInit_real_T(&sig_value, 2);
  static double locs_data[ecg_len1];
  int b_ecg_size[1];
  int locs_size[2];
  b_ecg_size[0] = 401;
  pan_tompkin(a, b_ecg_size, qrs_amp_raw, qrs_i_raw, noise, sig_value, locs_data,
              locs_size);
  int c1 = 0;

  int n1 = 0;
  if (qrs_i_raw->data[4] == 0) n1 = qrs_i_raw->data[3];
  else n1 = qrs_i_raw->data[4];

  for (int x = qrs_i_raw->data[0]-5; x < n1; x++)
  {
    a[c1] = g_l4s2_data[x];
    c1++;
  }

  (*env)->SetDoubleArrayRegion(env, l_l4s2_data, 0, 401, a);

  return l_l4s2_data; //*g_l4s_data;

}


JNIEXPORT jdoubleArray  JNICALL Java_com_agatsa_sanket_ECGMonitorActivity_dll4(JNIEnv *env, jobject obj)
{
  jdoubleArray l_l4s3_data = (*env)->NewDoubleArray(env,401);
  double a[401];


  double t1[100];

  for ( int x=0; x< 401;x=x+100)
  {
    for ( int y=0; y< 100 ; y++)
    {
      t1[y] = g_l4s3_data[x+y] ;
    }
    double *t2 = cleanup(t1);
    for ( int y=0; y< 100 ; y++)
    {
      g_l4s3_data[x+y] = t2[y] ;

    }
  }
  int count1 = 0;
  double temp1[1800] ;
  double temp2[1800] ;

  for(int y=0;y<1700;y=y+401)
  {
    for(int x=0;x<401;x++)
    {

      temp1[count1] = g_l4s3_data[x];
      count1++;
    }


  }





  filterECG128Hz(temp1,temp2);
  for ( int x=0;x<401;x++)
    g_l4s3_data[x] = temp2[x];

  for(int x=0;x<401;x++)
    a[x] = g_l4s3_data[x];//((g_l4s_data[x]-g_l4s2_data[x]));

  //
  emxArray_real_T *qrs_amp_raw;
  emxArray_real_T *qrs_i_raw;
  emxArray_real_T *noise;
  emxArray_real_T *sig_value;
  emxInit_real_T(&qrs_i_raw, 2);
  emxInit_real_T(&qrs_amp_raw, 2);
  emxInit_real_T(&noise, 2);
  emxInit_real_T(&sig_value, 2);
  static double locs_data[ecg_len1];
  int b_ecg_size[1];
  int locs_size[2];
  b_ecg_size[0] = 401;
  pan_tompkin(a, b_ecg_size, qrs_amp_raw, qrs_i_raw, noise, sig_value, locs_data,
              locs_size);
  int c1 = 0;
  int n1 = 0;
  if (qrs_i_raw->data[4] == 0) n1 = qrs_i_raw->data[3];
  else n1 = qrs_i_raw->data[4];

  if (qrs_i_raw->data[3] == 0) n1 = qrs_i_raw->data[2];
  else n1 = qrs_i_raw->data[3];



  for (int x =  qrs_i_raw->data[0] -5 ; x < qrs_i_raw->data[3]; x++)
  {
    a[c1] = a[x];

    c1++;
  }
  for (int x =qrs_i_raw->data[2]  ; x < 401; x++)
  {
    // a[x] = 0;
  }
  //
  (*env)->SetDoubleArrayRegion(env, l_l4s3_data, 0, 401,g_l4s3_data );

  // (*env)->SetDoubleArrayRegion(env, l_l4s3_data, 0, 401, a);
  return l_l4s3_data; //*

}
JNIEXPORT jdoubleArray  JNICALL Java_com_agatsa_sanket_ECGMonitorActivity_dllavr(JNIEnv *env, jobject obj)
{
  jdoubleArray l_l4avl_data = (*env)->NewDoubleArray(env,401);
  double a[401];

  double t1[100];

  for ( int x=0; x< 401;x=x+100)
  {
    for ( int y=0; y< 100 ; y++)
    {
      t1[y] = l_avr[x+y] ;
    }
    double *t2 = cleanup(t1);
    for ( int y=0; y< 100 ; y++)
    {
      l_avr[x+y] = t2[y] ;

    }
  }



  int count1 = 0;
  double temp1[1800] ;
  double temp2[1800] ;

  for(int y=0;y<1700;y=y+401)
  {
    for(int x=0;x<401;x++)
    {

      temp1[count1] = l_avr[x];
      count1++;
    }


  }

  filterECG128Hz(temp1,temp2);
  for ( int x=0;x<401;x++)
    l_avr[x] = temp2[x];

  for(int x=0;x<401;x++)
    a[x] = l_avr[x];//((g_l4s_data[x]-g_l4s2_data[x]));




  emxArray_real_T *qrs_amp_raw;
  emxArray_real_T *qrs_i_raw;
  emxArray_real_T *noise;
  emxArray_real_T *sig_value;
  emxInit_real_T(&qrs_i_raw, 2);
  emxInit_real_T(&qrs_amp_raw, 2);
  emxInit_real_T(&noise, 2);
  emxInit_real_T(&sig_value, 2);
  static double locs_data[ecg_len1];
  int b_ecg_size[1];
  int locs_size[2];
  b_ecg_size[0] = 401;
  pan_tompkin(a, b_ecg_size, qrs_amp_raw, qrs_i_raw, noise, sig_value, locs_data,
              locs_size);
  int c1 = 0;
  int n1 = 0;

  for (int x = qrs_i_raw->data[0]+2; x < qrs_i_raw->data[3]; x++)
  {
    a[c1] = a[x];
    c1++;
  }

  for (int x = c1; x < 401; x++)
  {
    //  a[x] = 0;
  }



  (*env)->SetDoubleArrayRegion(env, l_l4avl_data, 0, 401, a);
  return l_l4avl_data; //*

}
JNIEXPORT jdoubleArray  JNICALL Java_com_agatsa_sanket_pdf_FileOperationsSingle_dllavf(JNIEnv *env, jobject obj, jdoubleArray  array1)
{
    jdouble *srcArrayElems =
            (*env)->GetDoubleArrayElements(env, array1, NULL);

    jint nnn = (*env)->GetArrayLength(env, array1);



    jdoubleArray result = (*env)->NewDoubleArray(env, nnn);

    jdouble *destArrayElems =
            (*env)->GetDoubleArrayElements(env, result, NULL);

    int len =nnn;// sizeof(array1);


    jdoubleArray l_l4avf_data = (*env)->NewDoubleArray(env,len);
  double a[nnn];
  double t1[100];
  for ( int x=0; x<nnn;x=x+100)
  {
    for ( int y=0; y< 100 ; y++)
    {
      t1[y] = srcArrayElems[x+y] ;
    }
    double *t2 = cleanup2(t1);
    for ( int y=0; y< 100 ; y++)
    {
        srcArrayElems[x+y] = t2[y] ;
    }
  }


//  int count1 = 0;
//  double temp1[1800] ;
//  double temp2[1800] ;
//
//  for(int y=0;y<1700;y=y+401)
//  {
//    for(int x=0;x<401;x++)
//    {
//      __// android_log_print(// android_log_VERBOSE, APPNAME, "l_avf[x]:%f counter %d", l_avf[x],x);
//      temp1[count1] = l_avf[x];
//      count1++;
//    }
//
//
//  }
//
//  filterECG128Hz(temp1,temp2);
//  for ( int x=0;x<401;x++)
//    l_avf[x] = temp2[x];
//
//
    int x;
  for(x=0;x<nnn;x++)
    a[x] = srcArrayElems[x];//((g_l4s_data[x]-g_l4s2_data[x]));

    //Code by Abhilash


//
//  emxArray_real_T *qrs_amp_raw;
//  emxArray_real_T *qrs_i_raw;
//  emxArray_real_T *noise;
//  emxArray_real_T *sig_value;
//  emxInit_real_T(&qrs_i_raw, 2);
//  emxInit_real_T(&qrs_amp_raw, 2);
//  emxInit_real_T(&noise, 2);
//  emxInit_real_T(&sig_value, 2);
//  static double locs_data[ecg_len1];
//  int b_ecg_size[1];
//  int locs_size[2];
//  b_ecg_size[0] = 401;
//  pan_tompkin(a, b_ecg_size, qrs_amp_raw, qrs_i_raw, noise, sig_value, locs_data,
//              locs_size);
//  int c1 = 0;
//  int n1 = 0;
//  if (qrs_i_raw->data[4] == 0) n1 = qrs_i_raw->data[3];
//  else n1 = qrs_i_raw->data[4];
//
//
//  for (int x = qrs_i_raw->data[0]-5; x < qrs_i_raw->data[3]; x++)
//  {
//    a[c1] = a[x];
//    c1++;
//  }
//
//
//  for (int x = c1; x < 401; x++)
//  {
//    //  a[x] = 0;
//  }
//

  (*env)->SetDoubleArrayRegion(env, l_l4avf_data, 0, len, a);
  return l_l4avf_data;
}
JNIEXPORT jdoubleArray  JNICALL Java_com_agatsa_sanket_ECGMonitorActivity_dllavl(JNIEnv *env, jobject obj) {
  jdoubleArray l_l4avl_data = (*env)->NewDoubleArray(env, 401);
  double a[401];

  double t1[100];

  for ( int x=0; x< 401;x=x+100)
  {
    for ( int y=0; y< 100 ; y++)
    {
      t1[y] = l_avl[x+y] ;
    }
    double *t2 = cleanup(t1);
    for ( int y=0; y< 100 ; y++)
    {
      l_avl[x+y] = t2[y] ;

    }
  }



  int count1 = 0;
  double temp1[1800] ;
  double temp2[1800] ;

  for(int y=0;y<1700;y=y+401)
  {
    for(int x=0;x<401;x++)
    {

      temp1[count1] = l_avl[x];
      count1++;
    }


  }

  filterECG128Hz(temp1,temp2);
  for ( int x=0;x<401;x++)
    l_avl[x] = temp2[x];


  for (int x = 0; x < 401; x++)
    a[x] = l_avl[x];//((g_l4s_data[x]-g_l4s2_data[x]));

  emxArray_real_T *qrs_amp_raw;
  emxArray_real_T *qrs_i_raw;
  emxArray_real_T *noise;
  emxArray_real_T *sig_value;
  emxInit_real_T(&qrs_i_raw, 2);
  emxInit_real_T(&qrs_amp_raw, 2);
  emxInit_real_T(&noise, 2);
  emxInit_real_T(&sig_value, 2);
  static double locs_data[ecg_len1];
  int b_ecg_size[1];
  int locs_size[2];
  b_ecg_size[0] = 401;
  pan_tompkin(a, b_ecg_size, qrs_amp_raw, qrs_i_raw, noise, sig_value, locs_data,
              locs_size);
  int c1 = 0;
  int n1 = 0;
  if (qrs_i_raw->data[4] == 0) n1 = qrs_i_raw->data[3];
  else n1 = qrs_i_raw->data[4];

  for (int x = qrs_i_raw->data[0]-5; x < qrs_i_raw->data[3]; x++)
  {
    a[c1] = a[x];
    c1++;
  }

  for (int x = c1; x < 401; x++)
  {
    //  a[x] = 0;
  }


  (*env)->SetDoubleArrayRegion(env, l_l4avl_data, 0, 401, a);
  return l_l4avl_data; //*

}



JNIEXPORT jdoubleArray  JNICALL Java_com_agatsa_sanket_ECGMonitorActivity_dll5(JNIEnv *env, jobject obj)
{
  jdoubleArray l_rrinterval1 = (*env)->NewDoubleArray(env,20);
  double a[20];
  for( int x=0;x<20;x++)
    a[x] = g_rrinterval1[x];
  (*env)->SetDoubleArrayRegion(env, l_rrinterval1, 0, 20, a);
  return l_rrinterval1;//*g_rrinterval1;

  //return pos3Value;
}

JNIEXPORT jdoubleArray  JNICALL Java_com_agatsa_sanket_ECGMonitorActivity_dll6(JNIEnv *env, jobject obj)
{
  jdoubleArray l_rrinterval2 = (*env)->NewDoubleArray(env,20);
  double a[20];
  for( int x=0;x<20;x++)
    a[x] = g_rrinterval2[0].data[x];
  (*env)->SetDoubleArrayRegion(env, l_rrinterval2, 0, 20, a);
  return l_rrinterval2;//*g_rrinterval1;

  //return pos3Value;
}

JNIEXPORT jdoubleArray JNICALL Java_com_agatsa_sanket_ECGMonitorActivity_dll7(JNIEnv *env, jobject obj)
{
  jdoubleArray l_rrinterval3 = {0} ;

  return l_rrinterval3;
}


JNIEXPORT jdouble JNICALL
Java_com_agatsa_sanket_pdf_FileOperationsSingle_getFinaly(JNIEnv *env, jclass type, jint x) {

  // TODO
  return y[x];
  //return 1.0;

}
JNIEXPORT jint JNICALL
Java_com_agatsa_sanket_ECGMonitorActivity_getAvgPr(JNIEnv *env, jclass type) {

  // TODO
  return AvgPRVal;
  //return 1.0;

}
JNIEXPORT jint JNICALL
Java_com_agatsa_sanket_ECGMonitorActivity_getAvgHr(JNIEnv *env, jclass type) {

  // TODO
  return hr;
  //return 1.0;

}
JNIEXPORT jstring JNICALL
Java_com_agatsa_sanket_ECGMonitorActivity_batSer(JNIEnv *env, jclass type) {

  // TODO


  return (*env)->NewStringUTF(env, "0000180F-0000-1000-8000-00805F9B34FB");
}
JNIEXPORT jstring JNICALL
Java_com_agatsa_sanket_blobstorage_MyBlobStorage_blobSanket(JNIEnv *env, jclass type) {

  // TODO


  return (*env)->NewStringUTF(env, "DefaultEndpointsProtocol=http;AccountName=sanketblob;AccountKey=5Vdyu/qQPpawaQfPzLCzbM0wgHKUz6FPpQ1kWaVzN24RBDHgjQt5r2nxQwVjpI1xjQDnjrbiSAYgOCLMHN/b9g==");
}

JNIEXPORT jstring JNICALL
Java_com_agatsa_sanket_ECGMonitorActivity_batChar(JNIEnv *env, jclass type) {

  // TODO


  return (*env)->NewStringUTF(env, "00002a19-0000-1000-8000-00805f9b34fb");
}
JNIEXPORT jstring JNICALL
Java_com_agatsa_sanket_ECGMonitorActivity_eSer(JNIEnv *env, jclass type) {

  // TODO


  return (*env)->NewStringUTF(env, "0000FFB1-0000-1000-8000-00805F9B34FB");
}
JNIEXPORT jstring JNICALL
Java_com_agatsa_sanket_ECGMonitorActivity_eChar(JNIEnv *env, jclass type) {

  // TODO


  return (*env)->NewStringUTF(env, "0000FFB2-0000-1000-8000-00805F9B34FB");
}
JNIEXPORT jstring JNICALL
Java_com_agatsa_sanket_ECGMonitorActivity_bSer(JNIEnv *env, jclass type) {

  // TODO


  return (*env)->NewStringUTF(env, "0000180F-0000-1000-8000-00805F9B34FB");
}
JNIEXPORT jstring JNICALL
Java_com_agatsa_sanket_ECGMonitorActivity_bChar(JNIEnv *env, jclass type) {

  // TODO


  return (*env)->NewStringUTF(env, "00002a19-0000-1000-8000-00805f9b34fb");
}
JNIEXPORT jstring JNICALL
Java_com_agatsa_sanket_ECGMonitorActivity_eDes(JNIEnv *env, jclass type) {

  // TODO


  return (*env)->NewStringUTF(env, "00002902-0000-1000-8000-00805f9b34fb");
}


JNIEXPORT jint JNICALL
Java_com_agatsa_sanket_ECGMonitorActivity_getAvgQt(JNIEnv *env, jclass type) {

  // TODO
  return AvgQTVal;
  //return 1.0;

}
JNIEXPORT jint JNICALL
Java_com_agatsa_sanket_ECGMonitorActivity_getAvgQtc(JNIEnv *env, jclass type) {

  // TODO
  return AvgQTcVal;
  //return 1.0;

}
JNIEXPORT jint JNICALL
Java_com_agatsa_sanket_ECGMonitorActivity_getAvgQrs(JNIEnv *env, jclass type) {

  // TODO
  return AvgQRSVal;
  //return 1.0;

}
JNIEXPORT jdouble JNICALL
Java_com_agatsa_sanket_ECGMonitorActivity_getVoltage(JNIEnv *env, jclass type) {

  // TODO
  return sig_val1;
  //return 1.0;

}
//void ResetBDAC(void)
JNIEXPORT jint JNICALL Java_com_agatsa_sanket_activity_EcgGraphPlottingActivity_ResetBDAC(JNIEnv *env, jobject obj)
{
   RRPeakVal = 0;
    hr = 0;
    PRVal=0;
    QRSVal = 0;
    QTcVal = 0;
    QTVal = 0;
}
JNIEXPORT jint JNICALL Java_com_agatsa_sanket_activity_EcgGraphPlottingActivity_BeatDetectAndClassify(JNIEnv *env, jobject obj, jint ecgSample){

  counter +=1;
//if (counter < 100) // Discard first 540 samples
//		return NOTREADY;

  if (length>=WINDOW_LENGTH)
    length=0;
  else{
    PQRSSampl1[length]=ecgSample;//(double)(((double)(ecgSample)));// (double)(((double)(ecgSample))/1000);
    PQRSSampl2[length]= ecgSample;//(double)(((double)(ecgSample)));//(double)(((double)(ecgSample))/1000);
    length +=1;
    counter =101;
    return NOTREADY;
  }

  double PQRSLeft_Minima  = 10000;
  double PQRSRight_Minima = 10000;
  double PQRSLeft_Maxima  = 100;
  double PQRSRight_Maxima = 100;
  double PQRSLeft_Minima1 = 10000;
  double PQRSRight_Minima1= 10000;
  double Ist_Maxima       = 10000;
  double IInd_Maxima      = 10000;
  double IIIrd_Maxima     = 10000;
  int   flagnotpresent    = 0;
  int   FlagPQRSLeft      = 0;
  int   FlagPQRSRight     = 0;
  double RRPeak           = 0;
  double RRPeakValInSecs  = 0;
//double QTcvalue         = 0;
//double QTcval1          = 0;
//double QTcval2          = 0;
//double m;
  int pos4_P              = 0;
  int pos4_T              = 0;
  int pos3_Q              = 0;
  int pos3_S              = 0;
  int pos4_P_start        = 0;
  int pos4_T_end          = 0;
  int pos3_Q2             = 0;
  int pos3_S2             = 0;
  int pos4_P2             = 0;
  int pos4_T2             = 0;
  int pos4_P2_start       = 0;
  int pos4_T2_end         = 0;
  int pos_P_Q             = 0;
  int pos_Q_R             = 0;
  int pos_R_S             = 0;
  int pos_S_T             = 0;
  int pos_Q_R_S           = 0;
  int pos_P_R             = 0;
  int pos_Q_T             = 0;
  int pos_RRPeak          = 0;
//int QT_Diff             = 0;
  int pos_P               = 0;

  int   pos1              = 0;
  int   pos2              = 0;
  int   pos3              = 0;
  int   d                 = 0;
  int   e                 = 0;
  int   f                 = 0;
  int   g                 = 0;
  int   h                 = 0;
  int   i                 = 0;
  int   l                 = 0;
//int flag_sqrt           = 0;

  for ( d = 0; d < WINDOW_LENGTH; d++){
    if (Ist_Maxima > PQRSSampl1[d]){
//Once we have the 1st maxima we store it...
      Ist_Maxima = PQRSSampl1[d];
//... as well as its location
      pos1 = d;
//          printf("pos1 %d\n",d);
    }
  }
// Update the location in which Maxima was found with 100
  pos1Value = PQRSSampl1[pos1];
  PQRSSampl1[pos1]  = 10000;



//Depending on the mentioned conditions replace the values in the immediate next position in which 1st Maxima was found by 100 as well.
  if ((pos1+1) < WINDOW_LENGTH)
    PQRSSampl1[(pos1+1)] = 10000;

  if ((pos1+2) < WINDOW_LENGTH)
    PQRSSampl1[(pos1+2)] = 10000;

  if ((pos1+3) < WINDOW_LENGTH)
    PQRSSampl1[(pos1+3)] = 10000;

  if ((pos1-1) >= 0)
    PQRSSampl1[(pos1-1)] = 10000;

  if ((pos1-2) >= 0)
    PQRSSampl1[(pos1-2)] = 10000;

  if ((pos1-3) >= 0)
    PQRSSampl1[(pos1-3)] = 10000;

  for ( d = 0; d < WINDOW_LENGTH; d++){
    if (IInd_Maxima > PQRSSampl1[d]){
      IInd_Maxima = PQRSSampl1[d];
      pos2 = d;
//          printf("d %d\n",d);
    }
  }

  pos2Value = PQRSSampl1[pos2];
  PQRSSampl1[pos2]  = 10000;

  if ((pos2+1) < WINDOW_LENGTH)
    PQRSSampl1[(pos2+1)] = 10000;

  if ((pos2+2) < WINDOW_LENGTH)
    PQRSSampl1[(pos2+2)] = 10000;

  if ((pos2+3) < WINDOW_LENGTH)
    PQRSSampl1[(pos2+3)] = 10000;

  if ((pos2-1) >= 0)
    PQRSSampl1[(pos2-1)] = 10000;

  if ((pos2-2) >= 0)
    PQRSSampl1[(pos2-2)] = 10000;

  if ((pos2-3) >= 0)
    PQRSSampl1[(pos2-3)] = 10000;

  for ( d = 0; d < WINDOW_LENGTH; d++){
    if (IIIrd_Maxima > PQRSSampl1[d]){
//Once we have the 1st maxima we store it...
      IIIrd_Maxima = PQRSSampl1[d];
//... as well as its location
      pos3 = d;
//            printf("d %d\n",d);
    }
  }
// Update the location in which Maxima was found with 100

  pos3Value = PQRSSampl1[pos3];
  PQRSSampl1[pos3]  = 100;

  if ((pos3+1) < WINDOW_LENGTH)
    PQRSSampl1[(pos3+1)] = 100;

  if ((pos3+2) < WINDOW_LENGTH)
    PQRSSampl1[(pos3+2)] = 100;

  if ((pos3+3) < WINDOW_LENGTH)
    PQRSSampl1[(pos3+3)] = 100;

  if ((pos3-1) >= 0)
    PQRSSampl1[(pos3-1)] = 100;

  if ((pos3-2) >= 0)
    PQRSSampl1[(pos3-2)] = 100;

  if ((pos3-3) >= 0)
    PQRSSampl1[(pos3-3)] = 100;



  int lowest = least(pos1, pos2, pos3);
//    printf("lowest of three %d",lowest);


  int nextLowest = 0;

  if(lowest == pos1) {
    if(pos2 > pos3)
      nextLowest = pos3;
    else
      nextLowest = pos2;

  }else if(lowest == pos2) {

    if(pos1 > pos3)
      nextLowest = pos3;
    else
      nextLowest = pos1;

  }else {
    if(pos1 > pos2)
      nextLowest = pos2;
    else
      nextLowest = pos1;
  }




/*
PQRSSampl1[pos2]       = 100;
if ((pos2+1) < WINDOW_LENGTH)
PQRSSampl1[(pos2+1)] = 100;

if ((pos2-1) >= 0)
PQRSSampl1[(pos2-1)] = 100;

for (d = 0; d<WINDOW_LENGTH; d++){
  if (IIIrd_Maxima < PQRSSampl1[d]){
      IIIrd_Maxima = PQRSSampl1[d];
      pos3 = d;
  }
}*/

  if((nextLowest - lowest) > 50)
    flagnotpresent = 1;
  else {
    flagnotpresent = 0;
    RRPeak         = 0;
  }

  pos2 = nextLowest;
  pos1 = lowest;

// This means more than one R-Peaks are obtained in the sample
  if (flagnotpresent == 1){
// Start of POS2 Calculation
//==========================================//
// Create the window having 20 Samples to Left and 20
// Samples to Right of the Peak R-Value.
    if (((pos2-20) >= 0) && ((pos2+20) < WINDOW_LENGTH)){
      FlagPQRSLeft  = 1;
      PQRSLeft[19]  = PQRSSampl2[pos2-1];
      PQRSLeft[18]  = PQRSSampl2[pos2-2];
      PQRSLeft[17]  = PQRSSampl2[pos2-3];
      PQRSLeft[16]  = PQRSSampl2[pos2-4];
      PQRSLeft[15]  = PQRSSampl2[pos2-5];
      PQRSLeft[14]  = PQRSSampl2[pos2-6];
      PQRSLeft[13]  = PQRSSampl2[pos2-7];
      PQRSLeft[12]  = PQRSSampl2[pos2-8];
      PQRSLeft[11]  = PQRSSampl2[pos2-9];
      PQRSLeft[10]  = PQRSSampl2[pos2-10];
      PQRSLeft[9]   = PQRSSampl2[pos2-11];
      PQRSLeft[8]   = PQRSSampl2[pos2-12];
      PQRSLeft[7]   = PQRSSampl2[pos2-13];
      PQRSLeft[6]   = PQRSSampl2[pos2-14];
      PQRSLeft[5]   = PQRSSampl2[pos2-15];
      PQRSLeft[4]   = PQRSSampl2[pos2-16];
      PQRSLeft[3]   = PQRSSampl2[pos2-17];
      PQRSLeft[2]   = PQRSSampl2[pos2-18];
      PQRSLeft[1]   = PQRSSampl2[pos2-19];
      PQRSLeft[0]   = PQRSSampl2[pos2-20];

      PQRSRight[19] = PQRSSampl2[pos2+1];
      PQRSRight[18] = PQRSSampl2[pos2+2];
      PQRSRight[17] = PQRSSampl2[pos2+3];
      PQRSRight[16] = PQRSSampl2[pos2+4];
      PQRSRight[15] = PQRSSampl2[pos2+5];
      PQRSRight[14] = PQRSSampl2[pos2+6];
      PQRSRight[13] = PQRSSampl2[pos2+7];
      PQRSRight[12] = PQRSSampl2[pos2+8];
      PQRSRight[11] = PQRSSampl2[pos2+9];
      PQRSRight[10] = PQRSSampl2[pos2+10];
      PQRSRight[9]  = PQRSSampl2[pos2+11];
      PQRSRight[8]  = PQRSSampl2[pos2+12];
      PQRSRight[7]  = PQRSSampl2[pos2+13];
      PQRSRight[6]  = PQRSSampl2[pos2+14];
      PQRSRight[5]  = PQRSSampl2[pos2+15];
      PQRSRight[4]  = PQRSSampl2[pos2+16];
      PQRSRight[3]  = PQRSSampl2[pos2+17];
      PQRSRight[2]  = PQRSSampl2[pos2+18];
      PQRSRight[1]  = PQRSSampl2[pos2+19];
      PQRSRight[0]  = PQRSSampl2[pos2+20];

// Find the positions of Q and S Respectively to the
// Left and Right, by finding the Minima Points
        /*__android_log_print(ANDROID_LOG_ERROR, APPNAME, "PQRSLeft_Maxima %f", PQRSLeft_Maxima);
        __android_log_print(ANDROID_LOG_ERROR, APPNAME, "PQRSRight_Maxima %f", PQRSRight_Maxima);*/
      for (e = 10; e < 20; e++){
          /*__android_log_print(ANDROID_LOG_ERROR, APPNAME, "e %d", e);
          __android_log_print(ANDROID_LOG_ERROR, APPNAME, "PQRSLeft e %f", PQRSLeft[e]);*/
        if (PQRSLeft_Maxima < PQRSLeft[e]){

          PQRSLeft_Maxima = PQRSLeft[e];
          pos3_Q2 = e;
        }
        if (PQRSRight_Maxima < PQRSRight[e]){
            /*__android_log_print(ANDROID_LOG_ERROR, APPNAME, "e %d", e);
            __android_log_print(ANDROID_LOG_ERROR, APPNAME, "PQRSRight e %f", PQRSRight[e]);*/
          PQRSRight_Maxima = PQRSRight[e];
          pos3_S2 = e;
        }
      }

        /*__android_log_print(ANDROID_LOG_ERROR, APPNAME, "pos3_Q2 %d", pos3_Q2);
        __android_log_print(ANDROID_LOG_ERROR, APPNAME, "pos3_S2 %d", pos3_S2);*/

// Now, find the Positions of P and T to the Left and
// Right, by finding the Maxima Points
      for (f = 0; f < pos3_Q2; f++){
        if (PQRSLeft_Maxima < PQRSLeft[f]){
          PQRSLeft_Maxima = PQRSLeft[f];
          pos4_P2 = f;
        }
      }
      for (g = 0; g < pos3_S2; g++){
        if (PQRSRight_Maxima < PQRSRight[g]){
          PQRSRight_Maxima = PQRSRight[g];
          pos4_T2 = g;
        }
      }
        /*__android_log_print(ANDROID_LOG_ERROR, APPNAME, "pos4_P2 %d", pos4_P2);
        __android_log_print(ANDROID_LOG_ERROR, APPNAME, "pos4_T2 %d", pos4_T2);*/

// After, this find the positions of Start of P and End
// of T Segment, by finding the Minima Points
      for (h = 0; h < pos4_P2; h++){
        if (PQRSLeft_Minima1 > PQRSLeft[h]){
          PQRSLeft_Minima1 = PQRSLeft[h];
          pos4_P2_start = h;
        }
      }
      for (i = 0; i < pos4_T2; i++){
        if (PQRSRight_Minima1 > PQRSRight[i]){
          PQRSRight_Minima1 = PQRSRight[i];
          pos4_T2_end = i;
        }
      }
        /*__android_log_print(ANDROID_LOG_ERROR, APPNAME, "pos4_P2_start %d", pos4_P2_start);
        __android_log_print(ANDROID_LOG_ERROR, APPNAME, "pos4_T2_end %d", pos4_T2_end);*/
    }//End if (((pos2-20) >= 0) && ((pos2+20) < WINDOW_LENGTH)){

// Create the window having 10 Samples to Left and 10
// Samples to Right of the Peak R-Value. This is if, 20
// Samples to the Left and Right are not present
    if (FlagPQRSLeft == 0){
      if ((pos2-10) >= 0 && (pos2+10) <= WINDOW_LENGTH){
        PQRSLeft[9]   =PQRSSampl2[pos2-1];
        PQRSLeft[8]   =PQRSSampl2[pos2-2];
        PQRSLeft[7]   =PQRSSampl2[pos2-3];
        PQRSLeft[6]   =PQRSSampl2[pos2-4];
        PQRSLeft[5]   =PQRSSampl2[pos2-5];
        PQRSLeft[4]   =PQRSSampl2[pos2-6];
        PQRSLeft[3]   =PQRSSampl2[pos2-7];
        PQRSLeft[2]   =PQRSSampl2[pos2-8];
        PQRSLeft[1]   =PQRSSampl2[pos2-9];
        PQRSLeft[0]   =PQRSSampl2[pos2-10];

        PQRSRight[9]  =PQRSSampl2[pos2+1];
        PQRSRight[8]  =PQRSSampl2[pos2+2];
        PQRSRight[7]  =PQRSSampl2[pos2+3];
        PQRSRight[6]  =PQRSSampl2[pos2+4];
        PQRSRight[5]  =PQRSSampl2[pos2+5];
        PQRSRight[4]  =PQRSSampl2[pos2+6];
        PQRSRight[3]  =PQRSSampl2[pos2+7];
        PQRSRight[2]  =PQRSSampl2[pos2+8];
        PQRSRight[1]  =PQRSSampl2[pos2+9];
        PQRSRight[0]  =PQRSSampl2[pos2+10];

// Find the positions of Q and S Respectively to the
// Left and Right, by finding the Minima Points
        for (e = 0; e< 10; e++){
          if (PQRSLeft_Maxima < PQRSLeft[e]){
            PQRSLeft_Maxima = PQRSLeft[e];
            pos3_Q2 = e;
          }
          if (PQRSRight_Maxima < PQRSRight[e]){
            PQRSRight_Maxima = PQRSRight[e];
            pos3_S2 = e;
          }
        }
         /* __android_log_print(ANDROID_LOG_ERROR, APPNAME, "pos3_Q2 %d", pos3_Q2);
          __android_log_print(ANDROID_LOG_ERROR, APPNAME, "pos3_S2 %d", pos3_S2);*/

// Now, find the Positions of P and T to the Left and
// Right, by finding the Maxima Points
        for (f = 0; f < pos3_Q2; f++){
          if (PQRSLeft_Maxima < PQRSLeft[f]){
            PQRSLeft_Maxima = PQRSLeft[f];
            pos4_P2 = f;
          }
        }
        for (g = 0; g < pos3_S2; g++){
          if (PQRSRight_Maxima > PQRSRight[g]){
            PQRSRight_Maxima = PQRSRight[g];
            pos4_T2 = g;
          }
        }
          /*__android_log_print(ANDROID_LOG_ERROR, APPNAME, "pos4_P2 %d", pos4_P2);
          __android_log_print(ANDROID_LOG_ERROR, APPNAME, "pos4_T2 %d", pos4_T2);*/
      }// End if ((pos2-10) >= 0 && (pos2+10) <= WINDOW_LENGTH)
    }
    sVal=PQRSRight_Maxima;
//      __android_log_print(ANDROID_LOG_ERROR, APPNAME, "PQRSRight_Maxima %d", PQRSRight_Maxima);
// End if (FlagPQRSLeft == 0){
// End of POS2 Calculation
//=================================================//

    PQRSLeft_Minima     = 10000;
    PQRSRight_Minima    = 10000;
    PQRSLeft_Maxima     = 100;
    PQRSRight_Maxima    = 100;
    PQRSLeft_Minima1    = 10000;
    PQRSRight_Minima1   = 10000;
// Create the window having 20 Samples to Left and 20
// Samples to Right of the Peak R-Value.
    if ((pos1-20) >= 0 && (pos1+20) <= WINDOW_LENGTH){
      FlagPQRSRight = 1;
      PQRSLeft[19]  = PQRSSampl2[pos1-1];
      PQRSLeft[18]  = PQRSSampl2[pos1-2];
      PQRSLeft[17]  = PQRSSampl2[pos1-3];
      PQRSLeft[16]  = PQRSSampl2[pos1-4];
      PQRSLeft[15]  = PQRSSampl2[pos1-5];
      PQRSLeft[14]  = PQRSSampl2[pos1-6];
      PQRSLeft[13]  = PQRSSampl2[pos1-7];
      PQRSLeft[12]  = PQRSSampl2[pos1-8];
      PQRSLeft[11]  = PQRSSampl2[pos1-9];
      PQRSLeft[10]  = PQRSSampl2[pos1-10];
      PQRSLeft[9]   = PQRSSampl2[pos1-11];
      PQRSLeft[8]   = PQRSSampl2[pos1-12];
      PQRSLeft[7]   = PQRSSampl2[pos1-13];
      PQRSLeft[6]   = PQRSSampl2[pos1-14];
      PQRSLeft[5]   = PQRSSampl2[pos1-15];
      PQRSLeft[4]   = PQRSSampl2[pos1-16];
      PQRSLeft[3]   = PQRSSampl2[pos1-17];
      PQRSLeft[2]   = PQRSSampl2[pos1-18];
      PQRSLeft[1]   = PQRSSampl2[pos1-19];
      PQRSLeft[0]   = PQRSSampl2[pos1-20];

      PQRSRight[19] =PQRSSampl2[pos1+1];
      PQRSRight[18] =PQRSSampl2[pos1+2];
      PQRSRight[17] =PQRSSampl2[pos1+3];
      PQRSRight[16] =PQRSSampl2[pos1+4];
      PQRSRight[15] =PQRSSampl2[pos1+5];
      PQRSRight[14] =PQRSSampl2[pos1+6];
      PQRSRight[13] =PQRSSampl2[pos1+7];
      PQRSRight[12] =PQRSSampl2[pos1+8];
      PQRSRight[11] =PQRSSampl2[pos1+9];
      PQRSRight[10] =PQRSSampl2[pos1+10];
      PQRSRight[9]  =PQRSSampl2[pos1+11];
      PQRSRight[8]  =PQRSSampl2[pos1+12];
      PQRSRight[7]  =PQRSSampl2[pos1+13];
      PQRSRight[6]  =PQRSSampl2[pos1+14];
      PQRSRight[5]  =PQRSSampl2[pos1+15];
      PQRSRight[4]  =PQRSSampl2[pos1+16];
      PQRSRight[3]  =PQRSSampl2[pos1+17];
      PQRSRight[2]  =PQRSSampl2[pos1+18];
      PQRSRight[1]  =PQRSSampl2[pos1+19];
      PQRSRight[0]  =PQRSSampl2[pos1+20];

// Find the positions of Q and S Respectively to the
// Left and Right, by finding the Minima Points
      for (e = 10; e < 20; e++){
        if (PQRSLeft_Maxima < PQRSLeft[e]){
          PQRSLeft_Maxima = PQRSLeft[e];
          pos3_Q = e;
        }
        if (PQRSRight_Maxima < PQRSRight[e]){
          PQRSRight_Maxima = PQRSRight[e];
          pos3_S = e;
        }
      }
        /*__android_log_print(ANDROID_LOG_ERROR, APPNAME, "pos3_Q %d", pos3_Q);
        __android_log_print(ANDROID_LOG_ERROR, APPNAME, "pos3_S %d", pos3_S);*/
// Now, find the Positions of P and T to the Left and
// Right, by finding the Maxima Points
      for (f = 0; f < pos3_Q; f++){
        if (PQRSLeft_Maxima < PQRSLeft[f]){
          PQRSLeft_Maxima = PQRSLeft[f];
          pos4_P = f;
        }
      }
      for (g = 0; g < pos3_S; g++){
        if (PQRSRight_Maxima < PQRSRight[g]){
          PQRSRight_Maxima = PQRSRight[g];
          pos4_T = g;
        }
      }
        /*__android_log_print(ANDROID_LOG_ERROR, APPNAME, "pos4_P %d", pos4_P);
        __android_log_print(ANDROID_LOG_ERROR, APPNAME, "pos4_T %d", pos4_T);*/

// After, this find the positions of Start of P and End
// of T Segment, by finding the Minima Points
      for (h = 0; h < pos4_P; h++){
        if (PQRSLeft_Minima1 > PQRSLeft[h]){
          PQRSLeft_Minima1 = PQRSLeft[h];
          pos4_P_start = h;
        }
      }
      for (i = 0; i < pos4_T; i++){
        if (PQRSRight_Minima1 > PQRSRight[i]){
          PQRSRight_Minima1 = PQRSRight[i];
          pos4_T_end = i;
        }
      }
    }// End if ((pos1-20) >= 1 && (pos1+20) <= Window_Size){
      /*__android_log_print(ANDROID_LOG_ERROR, APPNAME, "pos4_P_start %d", pos4_P_start);
      __android_log_print(ANDROID_LOG_ERROR, APPNAME, "pos4_T_end %d", pos4_T_end);*/

// Create the window having 10 Samples to Left and 10
// Samples to Right of the Peak R-Value.This is if, 20
// Samples to the Left and Right are not present
    if (FlagPQRSRight == 0){
      if ((pos1-10) >= 1 && (pos1+10) <= WINDOW_LENGTH){
        PQRSLeft[9]  =PQRSSampl2[pos1-1];
        PQRSLeft[8]  =PQRSSampl2[pos1-2];
        PQRSLeft[7]  =PQRSSampl2[pos1-3];
        PQRSLeft[6]  =PQRSSampl2[pos1-4];
        PQRSLeft[5]  =PQRSSampl2[pos1-5];
        PQRSLeft[4]  =PQRSSampl2[pos1-6];
        PQRSLeft[3]  =PQRSSampl2[pos1-7];
        PQRSLeft[2]  =PQRSSampl2[pos1-8];
        PQRSLeft[1]  =PQRSSampl2[pos1-9];
        PQRSLeft[0]  =PQRSSampl2[pos1-10];

        PQRSRight[9] =PQRSSampl2[pos1+1];
        PQRSRight[8] =PQRSSampl2[pos1+2];
        PQRSRight[7] =PQRSSampl2[pos1+3];
        PQRSRight[6] =PQRSSampl2[pos1+4];
        PQRSRight[5] =PQRSSampl2[pos1+5];
        PQRSRight[4] =PQRSSampl2[pos1+6];
        PQRSRight[3] =PQRSSampl2[pos1+7];
        PQRSRight[2] =PQRSSampl2[pos1+8];
        PQRSRight[1] =PQRSSampl2[pos1+9];
        PQRSRight[0] =PQRSSampl2[pos1+10];

// Find the positions of Q and S Respectively to the
// Left and Right, by finding the Minima Points
        for (e = 0; e < 10; e++){
          if (PQRSLeft_Maxima < PQRSLeft[e]){
            PQRSLeft_Maxima = PQRSLeft[e];
            pos3_Q = e;
          }
          if (PQRSRight_Maxima < PQRSRight[e]){
            PQRSRight_Maxima = PQRSRight[e];
            pos3_S = e;
          }
        }
         /* __android_log_print(ANDROID_LOG_ERROR, APPNAME, "pos3_Q %d", pos3_Q);
          __android_log_print(ANDROID_LOG_ERROR, APPNAME, "pos3_S %d", pos3_S);*/
// Now, find the Positions of P and T to the Left and
// Right, by finding the Maxima Points
        for (f = 0; f < pos3_Q; f++){
          if (PQRSLeft_Maxima < PQRSLeft[f]){
            PQRSLeft_Maxima = PQRSLeft[f];
            pos4_P = f;
          }
        }
        for (g = 0; g < pos3_S; g++){
          if (PQRSRight_Maxima < PQRSRight[g]){
            PQRSRight_Maxima = PQRSRight[g];
            pos4_T = g;
          }
        }

          /*__android_log_print(ANDROID_LOG_ERROR, APPNAME, "pos4_P %d", pos4_P);
          __android_log_print(ANDROID_LOG_ERROR, APPNAME, "pos4_T %d", pos4_T);*/
      }// End if ((pos1-10) >= 1 && (pos1+10) <= WINDOW_LENGTH){
    }// End if (FlagPQRSRight == 0){

// Ok, now, Calculate the P-Q, Q-R, R-S And S-T
// Intervals. For 20 Length Window and 10 Length Window
// Cases
    if (FlagPQRSRight == 1){
//        __android_log_print(ANDROID_LOG_ERROR, APPNAME, "FlagPQRSRight %s", "In FlagPQRSRight ");
      pos_P_Q = pos3_Q - pos4_P;//pos4_P_start;
      pos_Q_R = 20-pos3_Q;
      pos_R_S = 20-pos3_S;
      pos_S_T = pos3_S - pos4_T;//pos4_T_end;//pos4_T;
      pos_Q_R_S = (pos_Q_R + pos_R_S);
      pos_P_R   = pos_P_Q + pos_Q_R;
      pos_Q_T   = pos_Q_R_S + pos_S_T;
      pos_P     = pos4_P - pos4_P_start;
    }

// Ok, now, Calculate the P-Q, Q-R, R-S And S-T Intervals
    if (FlagPQRSRight == 0){
      if (FlagPQRSLeft == 1){
          /*__android_log_print(ANDROID_LOG_ERROR, APPNAME, "FlagPQRSLeft %s", "In FlagPQRSLeft ");
          __android_log_print(ANDROID_LOG_ERROR, APPNAME, "pos3_Q2 %d", pos3_Q2);
          __android_log_print(ANDROID_LOG_ERROR, APPNAME, "pos4_P2 %d", pos4_P2);*/
        pos_P_Q = pos3_Q2 - pos4_P2;//pos4_P2_start;
        pos_Q_R = 20-pos3_Q2;
        pos_R_S = 20-pos3_S2;
        pos_S_T = pos3_S2 - pos4_T2;//pos4_T2_end;//pos4_T2;
        pos_Q_R_S = (pos_Q_R + pos_R_S);
        pos_P_R   = pos_P_Q + pos_Q_R;
        pos_Q_T   = pos_Q_R_S + pos_S_T;
        pos_P     = pos4_P - pos4_P_start;
      }
      else{
         /* __android_log_print(ANDROID_LOG_ERROR, APPNAME, "FlagPQRSLeft %s", "In FlagPQRSLeft's else part");
          __android_log_print(ANDROID_LOG_ERROR, APPNAME, "pos3_Q %d", pos3_Q);
          __android_log_print(ANDROID_LOG_ERROR, APPNAME, "pos4_P %d", pos4_P);*/
        pos_P_Q = pos3_Q - pos4_P;
        pos_Q_R = 10 - pos3_Q;
//pos_Q_R = pos3_Q + 1;
        pos_R_S = 10 - pos3_S;
//pos_R_S = pos3_S + 1;
        pos_S_T = pos3_S - pos4_T;
        pos_Q_R_S = (pos_Q_R + pos_R_S);
        pos_P_R   = pos_P_Q + pos_Q_R;
        pos_Q_T   = pos_Q_R_S + pos_S_T;// + 3;
        pos_P     = pos4_P;
      }
    }

    if (pos1 > pos2)
      pos_RRPeak = pos1 - pos2;
    else
      pos_RRPeak = pos2 - pos1;

     /* __android_log_print(ANDROID_LOG_ERROR, APPNAME, "pos_P_Q %d", pos_P_Q);
      __android_log_print(ANDROID_LOG_ERROR, APPNAME, "pos_Q_R %d", pos_Q_R);
      __android_log_print(ANDROID_LOG_ERROR, APPNAME, "pos_P_R %d", pos_P_R);*/

// There can be two cases here. if there are 3 peaks in
// the RRPeakValWindow, and the Peaks are not consecutive, but a
// R Peak inbetween interleaves them.
//if (pos_RRPeak > 140)
//    RRPeak = (15000/pos_RRPeak);
//else
    RRPeak = (60*200)/pos_RRPeak;//(7500/pos_RRPeak);

    RRPeakVal = (int)RRPeak;



    if (RRPeakVal >= 160)
      RRPeakVal = (RRPeakVal >> 1);

//    if (RRPeakVal < 30)
//      RRPeakVal = (RRPeakVal << 1);


//      __android_log_print(2, APPNAME, "pos1 %d", pos1);
//      __android_log_print(2, APPNAME, "pos2 %d", pos2);
//      __android_log_print(2, APPNAME, "pos_RRPeak %d", pos_RRPeak);
//      __android_log_print(2, APPNAME, "RRPeak %d", (int)RRPeak);
//      __android_log_print(2, APPNAME, "RRPeakVal %d", RRPeakVal);



    RRPeakValInSecs    = (double)(RRPeakVal/60);
    IntervalInSecs     = (double)(RRPeakValInSecs/pos_RRPeak);
//	hr = RRPeakVal  ;//60/(RRPeakValInSecs * 0.04);//60/ IntervalInSecs;//   1000 * (double)(0.0182 * RRPeakVal );
    hr = RRPeakVal;//RRPeak;
    QRS                = (double)(pos_Q_R_S);///2;
    PR                 = (double)(0.0142 * pos_P_R);///2;
    QT                 = (double)(0.0142 * pos_Q_T);///2;
    Ps                  = (double)(IntervalInSecs * pos_P);

//===============Rajiv Start

//QTc = QT;
//RRPeakValInSecs    = (double)(RRPeakVal/60);

//QTc   = QT + (1.75*(RRPeakVal - 60));


//RRPeakValInSecs = (pos_RRPeak * 0.0182);
/*
 if (RRPeakValInSecs < 1){
    RRPeakValInSecs = (RRPeakValInSecs * 100);
    flag_sqrt = 1;
 }

 for(m=0;m<RRPeakValInSecs;m=m+n_sqrt)
 {
  if((m*m)>RRPeakValInSecs)
  {
   m=m-n_sqrt;  // This if() is used to calculate the final value as soon as the square of the number exceeds
   break;  // the number then we deduct the value exceeded and stop the procedure using break; this is our final value which is stored in m;
  }
 }

 if (flag_sqrt)
 m = m/10;

 QTc  = (double)(QT/m);
*/

//===============Rajiv end

/*
 if (RRPeakValInSecs > 1)
     RRPeakValInSecs = RRPeakValInSecs -1;
 else
     RRPeakValInSecs = 1 - RRPeakValInSecs;

 QTcvalue  = (RRPeakValInSecs/2);
 QTcval2   = QTcvalue;
 QTcvalue  = 1 + QTcvalue;
 QTcval1   = ((RRPeakValInSecs * RRPeakValInSecs)/8);
 QTcvalue  = QTcvalue - QTcval1;
 QTcvalue  = QTcvalue + (QTcval1 * QTcval2);

 QTc       = (double)(QT/QTcvalue);
 */

//      __android_log_print(ANDROID_LOG_ERROR, APPNAME, "PR %f", PR);

    QRSVal             = (int)((1000*((QRS/10)+0.7)*0.04)); // In milliseconds, QRSD_Duration
    PRVal              = (int)(1000*PR);  // In milliseconds, PR_Interval
    PVal               = RRPeakVal;//(int)(1000*P);   // P_Duration
    QTVal              = (int)(1000*QT);  // QT_Interval
//QTcVal             = (int)(1000*QTc);  // QT_Interval
// HODGES REgression Formulae for QT Correction Value/QTc Value Calculation.
    QTcVal             = QTVal + (1.75*(RRPeakVal - 60));
//      __android_log_print(ANDROID_LOG_ERROR, APPNAME, "PR value * 1000 %d", PRVal);




    if (PRVal >= 260)
      PRVal = (PRVal >> 1);

    if (PRVal < 80)
      PRVal = (PRVal << 1);

//	if (QTVal >= 480)
//		QTVal = (QTVal >> 1);

//	if (QTVal < 150)
//			QTVal = (QTVal << 1);

    doubleAvgRRPeakVal += (double)RRPeakVal;
    doubleAvgPRVal     += PRVal;
    doubleAvgQTVal     += QTVal;
    doubleAvgQTcVal    += QTcVal;
    doubleAvgQRSVal    += QRSVal;

    iter += 1;

    AvgRRPeakVal = (int)(doubleAvgRRPeakVal/iter);
//hr = AvgRRPeakVal* 0.12;//AvgRRPeakVal*0.132;//
    AvgPRVal     = (int)(doubleAvgPRVal/iter);
    AvgQTVal     = (int)(doubleAvgQTVal/iter);
    AvgQTcVal    = (int)(doubleAvgQTcVal/iter);
    AvgQRSVal    = (int)(doubleAvgQRSVal/iter);

    if (iter==16){
      iter =1;
      doubleAvgRRPeakVal = 0;
      doubleAvgPRVal     = 0;
      doubleAvgQTVal     = 0;
      doubleAvgQTcVal    = 0;
      doubleAvgQRSVal    = 0;
    }

/*
 doubleAvgRRPeakVal += (double)RRPeakVal;
 doubleAvgPRVal     += PR;
 doubleAvgQTVal     += QT;
 doubleAvgQTcVal    += QTc;
 doubleAvgQRSVal    += QRS;

 doubleAvgRRPeakVal = (double)(doubleAvgRRPeakVal/iter);
 doubleAvgPRVal     = (double)(doubleAvgPRVal/iter);
 doubleAvgQTVal     = (double)(doubleAvgQTVal/iter);
 doubleAvgQTcVal    = (double)(doubleAvgQTcVal/iter);
 doubleAvgQRSVal    = (double)(doubleAvgQRSVal/iter);
 */

/*
 if (indexval!=2){
 doubleAvgRRPeakVal[indexval] = RRPeakVal;
 doubleAvgPRVal[indexval]     = PRVal;
 doubleAvgQTVal[indexval]     = QTVal;
 doubleAvgQTcVal[indexval]    = QTcVal;
 doubleAvgQRSVal[indexval]    = QRSVal;
 if ((doubleAvgQRSVal[(indexval+1)%3] - QRSVal) > 20)
     QRSVal = (doubleAvgQRSVal[(indexval+1)%3]+QRSVal)/2;



 }else
     indexval =0;


     indexval +=1;
 AvgRRPeakVal = (int)RRPeakVal;//((doubleAvgRRPeakVal[0] + doubleAvgRRPeakVal[1] + doubleAvgRRPeakVal[2])/3);
 AvgPRVal     = (int)((doubleAvgPRVal[0] + doubleAvgPRVal[1] + doubleAvgPRVal[2])/3);
 AvgQTVal     = (int)((doubleAvgQTVal[0] + doubleAvgQTVal[1] + doubleAvgQTVal[2])/3);
 AvgQTcVal    = (int)((doubleAvgQTcVal[0] + doubleAvgQTcVal[1] + doubleAvgQTcVal[2])/3);
 AvgQRSVal    = (int)((doubleAvgQRSVal[0] + doubleAvgQRSVal[1] + doubleAvgQRSVal[2])/3);
 //QRSVal       = AvgQRSVal;
 //PRVal        = AvgPRVal;
 //QTVal        = AvgQTVal;
 //QTcVal       = AvgQTcVal;

 //iter += 1;

*/


  }// End if (flagnotpresent == 1){
  else{
    RRPeak    =0;
    RRPeakVal =0;
  }
  POS_QV = PRVal;
  POS_TV = pos4_T2;

/*    __android_log_print(ANDROID_LOG_ERROR, APPNAME, "Heart Rate %d", (int)hr);
    __android_log_print(ANDROID_LOG_ERROR, APPNAME, "PR %d", PRVal);
    __android_log_print(ANDROID_LOG_ERROR, APPNAME, "QT %d", QTVal);
    __android_log_print(ANDROID_LOG_ERROR, APPNAME, "QTc %d", QTcVal);
    __android_log_print(ANDROID_LOG_ERROR, APPNAME, "QRS %d", QRSVal);*/

  return RRPeakVal;
}
JNIEXPORT jint JNICALL Java_com_agatsa_sanket_activity_EcgGraphPlottingActivity_fdatumglobal(JNIEnv *env, jobject obj)
{
  //return fdatumGlobal;
  return (int)hr;
}
JNIEXPORT jint JNICALL Java_com_agatsa_sanket_activity_EcgGraphPlottingActivity_gPRInterval(JNIEnv *env, jobject obj)
{
  //return g_PR_Interval;
  return PRVal;

}
JNIEXPORT jint JNICALL Java_com_agatsa_sanket_activity_EcgGraphPlottingActivity_gQTInterval(JNIEnv *env, jobject obj)
{
  //return g_QT_Interval;
  return QTVal;

}
JNIEXPORT jint JNICALL Java_com_agatsa_sanket_activity_EcgGraphPlottingActivity_gQTcInterval(JNIEnv *env, jobject obj)
{
  //return g_QTc_Interval;

  return QTcVal;
}
JNIEXPORT jint JNICALL Java_com_agatsa_sanket_activity_EcgGraphPlottingActivity_QRSD(JNIEnv *env, jobject obj)
{
  //return _QRSD;
  return QRSVal;


}
JNIEXPORT jint JNICALL Java_com_agatsa_sanket_ECGMonitorActivity_RPeaks(JNIEnv *env, jobject obj)
{
  //return _QRSD;
  return g_peak_cnt;


}
JNIEXPORT jint JNICALL Java_com_agatsa_sanket_ECGMonitorActivity_AvgRR(JNIEnv *env, jobject obj)
{
  //return _QRSD;
  return g_avg_RPeak_Interval;


}
JNIEXPORT jdouble JNICALL Java_com_agatsa_sanket_ECGMonitorActivity_PR(JNIEnv *env, jobject obj)
{
  //return _QRSD;
  double avg;
  for(int  i = 0 ;i<g_peak_cnt;i++){
    avg+=pqrst[i].PR;
  }
  avg = 6.67*(avg/g_peak_cnt);
  return avg;


}
JNIEXPORT jdouble JNICALL Java_com_agatsa_sanket_ECGMonitorActivity_QT(JNIEnv *env, jobject obj)
{
  //return _QRSD;
  double avg;
  for(int  i = 0 ;i<g_peak_cnt;i++){
    avg+=pqrst[i].QT;
  }
  avg = 6.67*(avg/g_peak_cnt);
  return avg;



}
JNIEXPORT jdouble JNICALL Java_com_agatsa_sanket_ECGMonitorActivity_QTC(JNIEnv *env, jobject obj)
{
//  //return _QRSD;
//  double avg;
//  for(int  i = 0 ;i<g_peak_cnt;i++){
//    avg+=pqrst[i].PR;
//  }
//  avg = avg/g_peak_cnt;
//  return avg;



    return 0;
}
JNIEXPORT jdouble JNICALL Java_com_agatsa_sanket_ECGMonitorActivity_QRS(JNIEnv *env, jobject obj)
{
  //return _QRSD;
  double avg;
  for(int  i = 0 ;i<g_peak_cnt;i++){
    avg+=pqrst[i].QRS;
  }
  avg = 6.67*(avg/g_peak_cnt);
  return avg;


}

JNIEXPORT jint JNICALL Java_com_agatsa_sanket_ECGMonitorActivity_CheckForEcg(JNIEnv *env, jobject obj)
{
  jstring out = NULL,tmp;
  int n = 0;
  if (g_ecg_normal)
  {
    //printf("\n\n ** NORMAL ECG **\n");
    out = "NORMAL ECG";
    n=0;
  }
  else
  {
    //printf("\n\n ** ECG NOT NORMAL : ");
    out = "ECG NOT NORMAL : ";
    n = 1;
    if (g_stemi5_detected)
    {
      out = "ECG NOT NORMAL : STEMI5 DETECTED ";
      n = 2;
  //    printf("STEMI5 DETECTED **\n");
    }
    else if (g_stemi2_detected)
    {
      out = "ECG NOT NORMAL : STEMI2 DETECTED ";
      n = 3;
    //  printf("STEMI2 DETECTED **\n");
    }
    else if (g_stemi1_detected)
    {
      out = "ECG NOT NORMAL : STEMI1 DETECTED ";
      n= 4;
      //printf("STEMI1 DETECTED **\n");
    }
    else if (g_schema1_detected)
    {
      out = "ECG NOT NORMAL : CHEMA1 DETECTED ";
      n = 5;
     // printf("SCHEMA1 DETECTED **\n");
    }
    else if (g_schemapeaked_detected)
    {
      out = "ECG NOT NORMAL : SCHEMAPEAKED DETECTED ";
      n = 6;
     // printf("SCHEMAPEAKED DETECTED **\n");
    }
  }

  return n;

}



/* End of code generation (dll1.c) */