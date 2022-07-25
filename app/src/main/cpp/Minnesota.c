/**
 * Created on 28-January-2020
 * @author Vikas Pal
 * This file is used in to do the calculations that are to be done in the Minnesota and generate
 * array
 */
#include <string.h>
#include "Minnesota.h"
#include <stdio.h>
#include "math.h"
#include <stdlib.h>
#include <jni.h>
#include <android/log.h>

#define NOTFOUND -1
#define NOTREADY 33
#define PI 3.1415926535
#define APPNAME "Minnesota Agatsa"
#define WINDOW_SIZE 600   // Window Size update from 400 to 800 By Saurabh
                            // Again Window Size update from 800 to 600 By Saurabh

/**
 * Created on 28-January-2020
 * @author Vikas Pal
 * This Struct is defined in order to hold the values of the angles and positions simultaneously in order
 * to make the checks based on the same. This will also be beneficial in the correction of the R-Peaks in case
 * more R-peaks are detected than the maximum upper limit that we have defined for the algorithm
 */
struct POSITION_WITH_ANGLE {
    int position; // The position of the R-peak in the algorithm
    double angle; // The angle of the position with the simultaneous left and right positions
};
int RSkipValue = 0;
int lengthOfRDetected=0;
int MNC[65]; // array to return the minnesota code
int mncArrayLength = 0; // length of the the minnesota code array
static double PQRSMNCSampl1[6000]; // Array to store the original ecg value
static double PQRSMNCSampl2[6000]; // Array to store 1-order replica of the ecg value
static double PQRSMNCSampl3[6000]; // Array to store 2-order replica of the ecg value
int start = 0; // The position value of the starting index
int endP = 0; // The index of the array where P-Peak Ends
int endS = 0; // The index of the array where S-Peak Ends
int endQ = 0; // The index of the array where Q-Peak Ends
int startQ = 0; // The index of the array where Q-Peak Starts


int pos1 = 0; // The temporary position value - ambiguous declaration
int pos2 = 0; // The temporary position value - ambiguous declaration
int pos3 = 0; // The temporary position value - ambiguous declaration

int ans = 0;
int d = 0;
int e = 0;
int f = 0;
int g = 0;
int h = 0;
int l = 0;


int pr = 0; // The value of the PR in point sizes that was later converted in the PR duration is seconds


double PQRSLeft_Minima = 0.0;
double PQRSRight_Minima = 0.0;
double PQRSLeft_Maxima = 0.0;
double PQRSRight_Maxima = 0.0;

double I_Maxima = 0.0;


double positionRSlopes[100] = {0.0};
int positionRPeaks[100] = {0}; // Array for holding the R-peaks
int positionQPeaks[100] = {0}; // Array for holding the Q-peaks
int positionSPeaks[100] = {0}; // Array for holding the S-peaks
int positionPPeaks[100] = {0}; // Array for holding the P-peaks

int flag = 1;
//double sum=0.0, average; // sum and average variable defined in the application
double const value = 10.19999999999999998; // The value of the filter determined
double ANGLE_SLOPE_CHECK_CONSTANT = 0.06; // The slope check constant for the leads, it will defined as per the leads
double RRPeaksArrayInSeconds[99] = {0};

/**
 *  The array for holding the position and angle of the detected R-peaks
 */
struct POSITION_WITH_ANGLE positionAngle[100];
/**
 * The array for holding the corrected position and angle of the detected R-peaks
 */
struct POSITION_WITH_ANGLE correctedPositionAngle[100];

// To determine whether the function is to be run for the STRESS or ECG
int STRESS_OR_ECG = 0;
// The total Lead Size of the array that has to be processed
int totalLeadSize = 1500; // reduce from 2500 to 1500 By Saurabh

// The variable used for the cleanup function
double b_x[5];
double xtmp;
double y[100];
static double ecg[6000] = {0};




int MinnesotaChecker(double ecgValue, int leadChannel, int leadSize) {
    // check if the length of the global array for the ecg value is greater than the window size or leadSize
    __android_log_print(ANDROID_LOG_INFO, APPNAME, "\n The ecgValues are %lf and leadSize is %d and leadChannel is %d", ecgValue,leadSize,leadChannel);

    if(mncArrayLength > leadSize-2) {
        mncArrayLength =0;                               //mncArrayLength =mncArrayLength+1;
    } else {
//       mncArrayLength  // length of the the minnesota code array
        PQRSMNCSampl1[mncArrayLength] = (double)ecgValue;
        __android_log_print(ANDROID_LOG_INFO, APPNAME, "\n Minnesota Code Array are %lf \n", PQRSMNCSampl1);
        PQRSMNCSampl2[mncArrayLength] = (double)ecgValue;
        PQRSMNCSampl3[mncArrayLength] = (double)ecgValue;
        mncArrayLength++;
//        __android_log_print(ANDROID_LOG_INFO, APPNAME, "\n mncArrayLength is %d", mncArrayLength);
        return NOTREADY;
    }
    // __android_log_print(ANDROID_LOG_INFO, APPNAME, "\n Minnesota Code Array are [Outside the condition] %lf \n", PQRSMNCSampl1);

    if(leadChannel == 1) { // lead 1
        ANGLE_SLOPE_CHECK_CONSTANT = 0.06;
    } else if (leadChannel == 2) { // lead 2
        ANGLE_SLOPE_CHECK_CONSTANT = 0.06;
    } else if (leadChannel == 3) { // lead v1
        ANGLE_SLOPE_CHECK_CONSTANT = 0.10;
    } else if (leadChannel == 4) { // lead v2
        ANGLE_SLOPE_CHECK_CONSTANT = 0.06;
    } else if (leadChannel == 5) { // lead v3
        ANGLE_SLOPE_CHECK_CONSTANT = 0.06;
    } else if (leadChannel == 6) { // lead v4
        ANGLE_SLOPE_CHECK_CONSTANT = 0.06;
    } else if (leadChannel == 7) { // lead v5
        ANGLE_SLOPE_CHECK_CONSTANT = 0.06;
    } else { // lead v6
        ANGLE_SLOPE_CHECK_CONSTANT = 0.06;
    }

    // Initialize all the values in the array to zero
    for (int i=0; i<65;i++) {
        MNC[i] = 0;
//        __android_log_print(ANDROID_LOG_INFO, APPNAME, "\n The minnesota code is", MNC[i]);
    }


    // Check if the ecg is for Ventricular Fibrilation

    for (int i=0;i<leadSize-3;i++) {
        if(PQRSMNCSampl1[i] != PQRSMNCSampl1[i+1]) {
            flag = 0;
        }
    }


    // This check indicates that the array obtained is a straight line and flag is used to return the same
    if(flag == 1) {
        return 821; // It is for Ventricular Fibrilation
    }

    for(int i = 0; i < 8; i++) {
        struct POSITION_WITH_ANGLE positionStruct = peakSelectorPositionAngle(leadSize);
        if(positionStruct.angle != -1) {
            positionStruct.position = positionStruct.position+RSkipValue;
            positionStruct.angle = positionStruct.angle;
            // check if position struct is already in the array
            int isInArray = 0;
            for(int j =0; j < lengthOfRDetected; j++) {
                if(correctedPositionAngle[j].position == positionStruct.position) {
                    isInArray = 1;
                }
            }
            if(isInArray == 0) {
                correctedPositionAngle[lengthOfRDetected] = positionStruct;
                lengthOfRDetected++;
            }
        }
    }
    __android_log_print(ANDROID_LOG_INFO, APPNAME, "\nRSkipValue is %d and lengthOfDetected is %d",RSkipValue,lengthOfRDetected);

    if(RSkipValue >= totalLeadSize-WINDOW_SIZE && lengthOfRDetected !=0){
        sortArray(lengthOfRDetected);
        for(int i=0; i<lengthOfRDetected; i++){
            positionRPeaks[i] = correctedPositionAngle[i].position;
//            __android_log_print(ANDROID_LOG_ERROR, APPNAME, "\nPosition of the R-peaks for position: i %d %d", i,  positionRPeaks[i]);
        }
        printf("\n *****************");
//      __android_log_print(ANDROID_LOG_ERROR, APPNAME,
//      "\nPosition of the R-peaks for position: i %d %d", i,  positionRPeaks[i]);
        __android_log_print(ANDROID_LOG_INFO, APPNAME, "\nlengthOfRDetected is %d",lengthOfRDetected);

        int nn150 = calculateOtherValues(lengthOfRDetected); // Next Target  calculateOtherValues(lengthOfRDetected) || Saurabh

        __android_log_print(ANDROID_LOG_INFO, APPNAME, "\n Value of nn150 is %d ", nn150);
        __android_log_print(ANDROID_LOG_INFO, APPNAME, "\n Value of STRESS_OR_ECG is %d ", STRESS_OR_ECG);

//  [ @ author-Saurabh] [ CODE WILL RETURN BASED ON THE nn150 FILTERED VALUE WHICH COMES FROM calculateOtherValues FUNCTION ]

        if(STRESS_OR_ECG == 0){
            if(nn150>=0 && nn150<=4){
                return 67;              // Note:- Dual Chamber Minnesota code .[Self define].
                                        // [Decimal ascii Value of C is 67 so minnesota code 67 By Saurabh]
            }
            else if(nn150 > 20) { // in case of ECG
                return 831;     // Atrial Fibrillation code is 831
            } else if (nn150 <=20 && nn150 >= 16) {
                return 833;     // intermittent Atrial Fibrillation code is 833
            } else if(nn150>=10 && nn150<=14) {
                return 52; // Possible Anteriolateral M.I. // login Written By Saurabh
            }else{
                return -1;
            }
        } else {
            if(nn150 > 30) { // in case of ECG
                return 831;     // // Atrial Fibrillation code is 831
            } else if (nn150 <=30 && nn150 >= 25) {
                return 833;         // intermittent Atrial Fibrillation code is 833
            } else {
                return -1;
            }
        }
    } else {
        return -1;
    }
    return 0;
}

/**
 *
 */
struct POSITION_WITH_ANGLE peakSelectorPositionAngle(int leadSize) {
    __android_log_print(ANDROID_LOG_INFO, APPNAME,"\nleadSize is under peakSelectorPositionAngle Function %d", leadSize);
    struct POSITION_WITH_ANGLE rpostion;
    int position = 0; int control = 0;
    JMP:
    I_Maxima = 0.0;
    control++;
    for(int i =0; i< leadSize; i++) {
        if(PQRSMNCSampl1[i] > I_Maxima && PQRSMNCSampl1[i] != 1000000 && i!= NOTFOUND) {
            I_Maxima = PQRSMNCSampl1[i];
            position = i;
        }
    }

    //    printf("\nPosition before angle %d", position);
    int positionRLeft = position-5;
    int positionRRight = position+5;
    if(positionRRight <= leadSize-1 && positionRLeft >= 0) {
        if(checkIsHighest(position,5)) {
            double angle = printAngle(position, positionRLeft, positionRRight, PQRSMNCSampl3[position] , PQRSMNCSampl3[positionRLeft], PQRSMNCSampl3[positionRRight]);
//            __android_log_print(ANDROID_LOG_ERROR, APPNAME,"\nPosition @%d", position+RSkipValue);
//            __android_log_print(ANDROID_LOG_ERROR, APPNAME,"\nangle %lf", angle);

            if(angle > ANGLE_SLOPE_CHECK_CONSTANT) {
                for(int i = position-10; i < position+10 ; i++) {
                    PQRSMNCSampl1[i] = 1000000;
                }
                // Control check for nearby values
                if(control < 5) {
                    goto JMP;
                }
            } else {
                for(int i = position-10; i < position+10 ; i++) {
                    PQRSMNCSampl1[i] = 1000000;
                }
            }

            if(position != 0) {
                if (angle <= ANGLE_SLOPE_CHECK_CONSTANT) {
                    rpostion.angle = angle;
                    rpostion.position = position;
                    return rpostion;
                } else {
                    rpostion.angle = -1;
                    rpostion.position = 0;
                    return rpostion;
                }
            } else {
                rpostion.angle = -1;
                rpostion.position = 0;
                return rpostion;
            }
        } else {
            for(int i = position-10; i< position+10 ; i++) {
                PQRSMNCSampl1[i] = 1000000;
            }
            if(control < 5) {
                goto JMP;
            } else {
                rpostion.angle = -1;
                rpostion.position = 0;
                return rpostion;
            }
        }
    } else {
        if(positionRLeft<0) {
            for(int i = position; i>=0; i--) {
                PQRSMNCSampl1[i] = 1000000;
            }
        }

        if(positionRRight>leadSize) {
            for(int i = position; i<=leadSize;i++) {
                PQRSMNCSampl1[i] = 1000000;
            }
        }
        rpostion.angle = -1;
        rpostion.position = 0;
//        __android_log_print(ANDROID_LOG_INFO, APPNAME,"\nrpostion is %d",rpostion);
        return rpostion;
    }
}

/**
 * This function is used to sort the Array that has the values for the angles and positions of the
 * incorporated leads
 */
int sortArray(int size) {
    struct POSITION_WITH_ANGLE temp;
    for(int i=0; i<size; i++) {
        /** Place currently selected element array[i]
         * to its correct place.
         */
        for(int j=i+1; j<size; j++) {
            /*
             * Swap if currently selected array element
             * is not at its correct position.
             */
            if(correctedPositionAngle[i].position > correctedPositionAngle[j].position) {
                temp              = correctedPositionAngle[i];
                correctedPositionAngle[i] = correctedPositionAngle[j];
                correctedPositionAngle[j] = temp;
            }
        }
    }
//    __android_log_print(ANDROID_LOG_ERROR, APPNAME,"\nElements of array in ascending order: ");
    return 0;
}

/**
  * The function to find the Q-Peak using the position for the R-Peak and the unnecessary leadSize
  *
 **/
double lengthSquare(int ax, double ay, int bx, double by) {
    double xDiff = ax - bx;
    double yDiff = ay - by;
    return xDiff*xDiff + yDiff*yDiff;
}

double printAngle(int ax, int bx , int cx, double ay, double by, double cy) {
    // Square of lengths be a2, b2, c2
    double a2 = lengthSquare(bx,by,cx,cy);
    double b2 = lengthSquare(ax,ay,cx,cy);
    double c2 = lengthSquare(ax,ay,bx,by);

    // length of sides be a, b, c
    double a = sqrt(a2);
    double b = sqrt(b2);
    double c = sqrt(c2);

    // From Cosine law
    double alpha = acos((b2 + c2 - a2)/(2*b*c));
    double betta = acos((a2 + c2 - b2)/(2*a*c));
    double gamma = acos((a2 + b2 - c2)/(2*a*b));

    // Converting to degree
    alpha = alpha * 180 / PI;
    betta = betta * 180 / PI;
    gamma = gamma * 180 / PI;
    return alpha;
}

int checkIsLowest(int posTobeChecked) {
    double valueAtPos = PQRSMNCSampl3[posTobeChecked];
    double valueAtPreviousPos = PQRSMNCSampl3[posTobeChecked-1];
    double valueAtNextPos = PQRSMNCSampl3[posTobeChecked+1];

    if(valueAtPos< valueAtPreviousPos && valueAtPos < valueAtNextPos) {
        return 1;
    } else {
        return 0;
    }
}

int checkIsHighest(int posTobeChecked, int limit) {
    double valueAtPos = PQRSMNCSampl3[posTobeChecked];
    double valueAtPreviousPos = PQRSMNCSampl3[posTobeChecked-limit];
    double valueAtNextPos = PQRSMNCSampl3[posTobeChecked+limit];

    if(valueAtPos > valueAtPreviousPos && valueAtPos > valueAtNextPos) {
        return 1;
    } else {
        return 0;
    }
}



// Method to compare which one is the more close.
// We find the closest by taking the difference
// between the target and both values. It assumes
// that val2 is greater than val1 and target lies
// between these two.
int calculateOtherValues(int size) {
    __android_log_print(ANDROID_LOG_INFO, APPNAME, "\n CalculateOtherValues size %d",size);

    double sum = 0.0, mRR = 0.0, sdNN = 0.0, pNN = 0.0;
    for(int i=0; i<size-1; i++) {
        // 1000/200

        RRPeaksArrayInSeconds[i] = 5 * (positionRPeaks[i+1] - positionRPeaks[i]);
        printf("\nPeaks difference in ms %lf", RRPeaksArrayInSeconds[i]);
        __android_log_print(ANDROID_LOG_INFO, APPNAME, "\n Peaks difference in ms of RRPeaksArrayInSeconds is %lf", RRPeaksArrayInSeconds[i]);

    }

    for(int i=0; i<size-2;i++) {
        sum = sum + RRPeaksArrayInSeconds[i];
    }

    mRR = sum/size;
    printf("\nSum of the peaks %lf", sum);
    printf("\nmRR of the peaks %lf", mRR);
    __android_log_print(ANDROID_LOG_INFO, APPNAME, "\n ecg sum is %lf And mRR value is %lf", sum,mRR);
//    __android_log_print(ANDROID_LOG_DEBUG, "LOG_TAG", "\n this is log sum messge %lf \n",sum);
    sum = 0.0;
    for(int i=0; i<size-2;i++) {
        sum += ((RRPeaksArrayInSeconds[i]-mRR) * (RRPeaksArrayInSeconds[i]-mRR));
    }
    sdNN = sqrt(sum/size);
    double cvRR = sdNN * 100 / mRR;
    printf("\nCVRR of the peaks is %lf and RRPeaksArrayInSeconds is %lf", cvRR);
//    __android_log_print(ANDROID_LOG_INFO, APPNAME, "\n CVRR of the peaks is %lf and RRPeaksArrayInSeconds is %lf", cvRR,sum);
    int co = 0.0;
    for(int i=0; i<size-2; i++) {
        if(fabs(RRPeaksArrayInSeconds[i+1] - RRPeaksArrayInSeconds[i]) > 50) {
            co++;
        }
    }
    pNN = (co * 100) / (size);
    printf("\npNN of the peaks %lf", pNN);
//    __android_log_print(ANDROID_LOG_INFO, APPNAME, "\n pNN of the peaks is %lf", pNN);

    int nn150 = 0;
    for (int i = 0; i < size - 2; i++) {
        //__android_log_print(ANDROID_LOG_ERROR, APPNAME,"\nDifference in RR for nn150: %lf" , (RRPeaksArrayInSeconds[i + 1] - RRPeaksArrayInSeconds[i]));
        if (fabs(RRPeaksArrayInSeconds[i + 1] - RRPeaksArrayInSeconds[i]) > 150) {
            __android_log_print(ANDROID_LOG_INFO, APPNAME, "\n inside condition nn150 of the peaks %d", nn150);
            nn150++;

        }
    }
    __android_log_print(ANDROID_LOG_INFO, APPNAME, "\nnn150 of the peaks %d", nn150);
    return nn150;
}

double* cleanup(double ECG[]) {
    double b_y[100];
    double ytemp[100];
    memset(&b_y[0], 0, 100U * sizeof(double));
    memset(&ytemp[0], 0, 100U * sizeof(double));

    for (int i = 0; i < 5; i++) {
        b_x[i] = ECG[i];
    }

    for (int i = 0; i < 2; i++) {
        xtmp = b_x[i];
        b_x[i] = b_x[4 - i];
        b_x[4 - i] = xtmp;
    }

    for (int i = 0; i < 2; i++) {
        b_y[i] = 0.0;
        for (int j = 0; j < 5; j++) {
            b_y[i] += value * b_x[j];
        }
    }

    /*  Compute the steady state output */
    memset(&ytemp[0], 0, 100U * sizeof(double));
    for (int i = 0; i < 5; i++) {
        for (int j = i; j + 1 < 101; j++) {
            ytemp[j] += value * ECG[j - i];
        }
    }

    memcpy(&b_y[2], &ytemp[4], 96U * sizeof(double));

    /*  Compute the transient off */
    for (int i = 0; i < 5; i++) {
        b_x[i] = ECG[95 + i];
    }

    for (int i = 0; i < 2; i++) {
        xtmp = b_x[i];
        b_x[i] = b_x[4 - i];
        b_x[4 - i] = xtmp;
    }

    for (int i = 0; i < 2; i++) {
        b_y[98 + i] = 0.0;
        for (int j = 0; j < 5; j++) {
            b_y[98 + i] += value * b_x[j];
        }
    }

    memcpy(&y[0], &b_y[0], 100U * sizeof(double));
    for (int x=0;x<100;x++)
        ECG[x] = y[x];

    return ECG;
}



// 0.08*Sampling Frequency
// Please note that the values defined here should not be changed without proper research on the
// LOW PASS FILTER as these are the standard values generated after researching on the LOW PASS FILTERS

JNIEXPORT jint JNICALL
Java_com_example_minnesotaproject_MainActivity_FilterLeads(JNIEnv *env, jclass clazz,
                                                           jdouble ecg_sample, jint lead_channel,
                                                           jint lead_size, jint stress_orecg) {

    __android_log_print(ANDROID_LOG_INFO, APPNAME, "\n Lead_size is %d and lead channel is %d and stress_orecg is %d and ecg_sample is %lf", lead_size,lead_channel,stress_orecg,ecg_sample);


    if(mncArrayLength > lead_size-2) {
        mncArrayLength = 0;                         //mncArrayLength =mncArrayLength+1;
    } else {
        ecg[mncArrayLength] = (double) ecg_sample;
        mncArrayLength++;
//        __android_log_print(ANDROID_LOG_INFO, APPNAME, "\n ECG in double is %lf",ecg);
        return NOTREADY;

    }


    totalLeadSize = lead_size;
    double filteredECG[6000] = {0};
    double t1[100];
    for ( int x=0; x< lead_size;x=x+100) {
        for ( int y=0; y< 100 ; y++) {
            t1[y] = ecg[x+y] ;
        }
        double *t2 = cleanup(t1);
        for ( int y=0; y< 100 ; y++) {
            filteredECG[x+y] = t2[y] ;
        }
    }

    STRESS_OR_ECG = stress_orecg;

    int hasAFIBCondition = -1;
    for (int i=0; i< totalLeadSize;i+=300) {
        RSkipValue = i;
        for(int j=0; j< WINDOW_SIZE; j++) {
            hasAFIBCondition = MinnesotaChecker(filteredECG[i+j]*-3, lead_channel ,lead_size);

            __android_log_print(ANDROID_LOG_INFO, APPNAME, "\n Minnesota Code is %d",hasAFIBCondition);
        }
    }
    return hasAFIBCondition;

}

JNIEXPORT jdoubleArray JNICALL
Java_com_example_minnesotaproject_MainActivity_getRRIntervalArray(JNIEnv *env, jclass clazz) {

    int length = lengthOfRDetected;
    jdoubleArray RRIntIntervalArray = (*env)->NewDoubleArray(env,length);
    (*env)->SetDoubleArrayRegion(env, RRIntIntervalArray, 0, length, RRPeaksArrayInSeconds);

    return RRIntIntervalArray;
}

JNIEXPORT jint JNICALL
Java_com_example_minnesotaproject_MainActivity_ResetMinnesota(JNIEnv *env, jclass clazz) {
    mncArrayLength = 0;
    lengthOfRDetected = 0;
    for(int i = 0 ; i < 100; i++) {
        positionRPeaks[i] = 0;
        correctedPositionAngle[i].position = 0;
        correctedPositionAngle[i].angle = -1;

    }
    memset(ecg, 0, 6000);
    memset(positionRPeaks, 0, 100);
    memset(RRPeaksArrayInSeconds, 0, 99);
    return 0;
}