//
// Created by agatsa on 2020-01-28.
//

#ifndef VIKASAGATSAANDROID_MINNESOTA_H
#define VIKASAGATSAANDROID_MINNESOTA_H

int MinnesotaChecker(double ecgValue, int leadChannel, int leadSize);
int sortArray(int size);
int peakSelecter(int leadSize);
int findQPeak (int leadSize, int posR, int checklowest);
double printAngle(int ax, int bx , int cx, double ay, double by, double cy);
double lengthSquare(int ax, double ay, int bx, double by);
int startQDetection(int posQ);
int endSDetection(int posS);
int findSPeak (int leadSize, int posR, int checklowest);
int checkIsLowest(int posTobeChecked);
int checkIsHighest(int posTobeChecked, int limit);
int peakSelecterNew(int leadSize);
double convertToMm(double points);
int findClosest(double arr[], int n, double target);
int checkEndOfRWhenSDetected(int posS, int posR,double amplitudeS );
int checkStartofRWhenQDetected(int posR, int posQ, int startQ ,double amplitudeQ);
int calculateOtherValues(int size);
double* cleanup(double ECG[]);
struct POSITION_WITH_ANGLE peakSelectorPositionAngle(int leadSize);
double averageArray(double array[],int arraySize);
void ButterworthLowpassFilter0100SixthOrder(const double src[], double dest[], int size);

#endif //VIKASAGATSAANDROID_MINNESOTA_H

