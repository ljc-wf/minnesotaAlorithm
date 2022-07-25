/*****************************************************************************
FILE:  bdac.cpp
AUTHOR:	Patrick S. Hamilton
REVISED:	5/13/2002
  ___________________________________________________________________________

bdac.cpp: Beat Detection And Classification
Copywrite (C) 2001 Patrick S. Hamilton

This file is free software; you can redistribute it and/or modify it under
the terms of the GNU Library General Public License as published by the Free
Software Foundation; either version 2 of the License, or (at your option) any
later version.

This software is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU Library General Public License for more
details.

You should have received a copy of the GNU Library General Public License along
with this library; if not, write to the Free Software Foundation, Inc., 59
Temple Place - Suite 330, Boston, MA 02111-1307, USA.

You may contact the author by e-mail (pat@eplimited.edu) or postal mail
(Patrick Hamilton, E.P. Limited, 35 Medford St., Suite 204 Somerville,
MA 02143 USA).  For updates to this software, please visit our website
(http://www.eplimited.com).
  __________________________________________________________________________

bdac.cpp contains functions for handling Beat Detection And Classification.
The primary function calls a qrs detector.  When a beat is detected it waits
until a sufficient number of samples from the beat have occurred.  When the
beat is ready, BeatDetectAndClassify passes the beat and the timing
information on to the functions that actually classify the beat.

Functions in bdac.cpp require functions in the following files:
		qrsfilt.cpp
		qrsdet.cpp
		classify.cpp
		rythmchk.cpp
		noisechk.cpp
		analbeat.cpp
		match.cpp
		postclas.cpp

 __________________________________________________________________________

	Revisions:
		5/13/02:
			Encapsulated down sampling from input stream to beat template in
			the function DownSampleBeat.

			Constants related to time are derived from SAMPLE_RATE in qrsdet
         and BEAT_SAMPLE_RATE in bcac.h.

*******************************************************************************/
#include "bdac.h"
#include <jni.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define ECG_BUFFER_LENGTH	1000	// Should be long enough for a beat
// plus extra space to accommodate
// the maximum detection delay.
#define BEAT_QUE_LENGTH	10			// Length of que for beats awaiting
// classification.  Because of
// detection delays, Multiple beats
// can occur before there is enough data
// to classify the first beat in the que.
#define WINDOW_LENGTH 400
#define WINDOW_LENGTH_R 100
#define WINDOW_LENGTH_QS 10
#define MAX_AVG        10
#define NOTREADY 33
static int    length  =0;
static int    counter =0;
int    RRPeakVal=0;
double PQRSSampl1[WINDOW_LENGTH];
double PQRSSampl2[WINDOW_LENGTH];
double PQRSLeft[WINDOW_LENGTH_R];
double PQRSRight[WINDOW_LENGTH_R];
double RRPeakValInSecs;
double IntervalInSecs;
double QRS;
double PR;
double QT;
double QTc;
double P;
double hr;

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
int    AvgPRVal;
int    AvgQTVal;
int    AvgQTcVal;
int    AvgQRSVal;

// Internal function prototypes.

void DownSampleBeat(int *beatOut, int *beatIn) ;

// External function prototypes.


// rahul added start

int BeatBuffer_f[BEATLGTH];
int BeatBuffer_f2[BEATLGTH];
int BeatBuffer_der[BEATLGTH];      //150points
int BeatBuffer_der2[BEATLGTH];      //150points


int BeatBuffer_averder[BEATLGTH];
int BeatBuffer_aver[BEATLGTH];
int central_point_QRS;
int g_QRSOnset = 0;
int g_QRSOffset = 0;
int g_QTc_Interval ;
extern int _QRSD;		// QRS Duration
int _RR;
int rr = 0;


int g_pWave_type = 0;  /////  0 unsure 1 biphasic  2 positive 3 negative
int g_tWave_type = 0;   ///   0 unsure 1 positive 2 negative
int g_num_narrowP = 0;   ///  count the number of narrow P, if narrow p waves takes a large amount, I will check whether it is biphasic.

int g_filtered_signal[ECG_BUFFER_LENGTH];
int g_averaged_signal[ECG_BUFFER_LENGTH];
int g_averaged_diffed_signal[ECG_BUFFER_LENGTH];

int g_QRSonst_average;
int g_QRS_num;
int g_QRSonset_sum;
float **f_data;
int Data_index = 0;
int pos1Value=0;
int pos2Value=0;
int pos3Value=0;

extern int g_Iso_point;
extern int g_Iso_point_off;
extern int fdatumGlobal;

int g_P_Duration ;
int g_PR_Interval ;
int g_QT_Interval ;
int g_rr_lasttime;
int central_point_QRS;

int SlopeMethod(int *data,int startpoint,int overpoint);


// added rahul end


int QRSDet( int datum, int init ) ;
int NoiseCheck(int datum, int delay, int RR, int beatBegin, int beatEnd) ;
int Classify(int *newBeat,int rr, int noiseLevel, int *fidAdj, int init) ;
int GetDominantType(void) ;
int GetBeatEnd(int type) ;
int GetBeatBegin(int type) ;
int gcd(int x, int y) ;

// Global Variables

int ECGBuffer[ECG_BUFFER_LENGTH], ECGBufferIndex = 0 ;  // Circular data buffer.
int BeatBuffer[BEATLGTH] ;
int BeatQue[BEAT_QUE_LENGTH], BeatQueCount = 0 ;  // Buffer of detection delays.
int RRCount = 0 ;
int InitBeatFlag = 1 ;
jint GBeatType = 0;
jint GBeatMatch = 0;


int _ADCUnit=48;;    // ADC units per mV
int _ADCZero=128;    // ADC zero level
int _sampleRate=300; // ECG sampling rate
int _sampleCount = 0, _currQRSSample, _prevQRSSample, _lastHRUpdateSample;
double _dmhr, _ihr, _ihrlast, _mhr, _tol, _heartRate;
int sampleRate = 300;

int sVal;
// added rahul start
/*
JNIEXPORT jint JNICALL Java_com_example_secgv1_SanketECGGraph_fnBeatDetectorInit(JNIEnv *env, jobject obj,jint ADCUnit, jint ADCZero, jint sampleRate)
{
	_sampleRate = sampleRate; // 300Hz (OESA beat detector currently assumes 300Hz(
	_ADCUnit = ADCUnit; // 50 units per mV
	_ADCZero = ADCZero; // 128. (8bit unsigned data,where 128 = 0mV)
	
	// Initialise variables used for heart rate calculation
	_sampleCount = 0;
	_prevQRSSample = -9999;
	_lastHRUpdateSample = 0;
	_mhr = 70.0;
	_tol = 10.0;
	_heartRate = 0.;

	// Reset the beat detector
	// ResetBDAC();

}
*/
int max(int num1, int num2)
{
	/* local variable declaration */
	int result;

	if (num1 > num2)
		result = num1;
	else
		result = num2;

	return result;
}

int min(int num1, int num2)
{
	int result;

	if (num1 < num2)
		result = num1;
	else
		result = num2;
	return result;
}


int GetMaxPoint(int *data, int start, int end)
{
	int i;
	int max = data[start];
	int maxpoint = start;
	for (i=start;i<=end;i++)
	{
		if (data[i] > max)
		{
			maxpoint = i;
			max = data[i];
		}
	}
	return maxpoint;
}
int GetMinPoint(int *data, int start, int end)
{
	int i;
	int min = data[start];
	int minpoint = start;
	for (i=start;i<=end;i++)
	{
		if (data[i] < min)
		{
			minpoint = i;
			min = data[i];
		}
	}
	return minpoint;
}



int GetZeropoint_right(int *data,int start, int end)
{
	int i;
	for (i=start;i<= end;i++)
	{
		if (data[i]*data[i-1]<=0)
		{
			return i;
		}
	}
	return -1;
}
int GetZeropoint_left(int *data,int start, int end)
{
	int i;
	for (i=start;i >= end;i--)
	{
		if (data[i]*data[i+1]<=0)
		{
			return i;
		}
	}
	return -1;
}


int Getlocalmin_right(int *data, int start, int end)
{
	int i;

	int localmin_point = -1;
	for (i = start ;i < end;i++)
	{
		if (data[i] < data[i - 1]&&data[i] < data[i-2]&&data[i]<data[i-3]&&
			data[i] <= data[i+1]&&data[i]<=data[i+2])
		{
			localmin_point = i;
			break;
		}
	}
	return localmin_point;
}
int Getlocalmin_right_w(int *data, int start, int end)
{
	int i;

	int localmin_point = -1;
	for (i = start ;i < end;i++)
	{
		if (data[i] < data[i - 1]&&data[i] < data[i-2]&&data[i]<data[i-3]&&
			data[i] <= data[i+1]&&data[i]<=data[i+2]&&data[i]<data[i+3])
		{
			localmin_point = i;
			break;
		}
	}
	return localmin_point;
}
int Getlocalmin_right_T(int *data, int start, int end)
{
	int i;

	int localmin_point = -1;
	for (i = start ;i < end;i++)
	{
		if (data[i] < data[i - 1]&&data[i] < data[i-2]&&data[i]<data[i-3]&&data[i] < data[i-4]&&data[i]<data[i-5]&&
			data[i] <= data[i+1]&&data[i]<=data[i+2]&&data[i]<=data[i+3]&&data[i]<=data[i+4]&&data[i]<=data[i+5])
		{
			localmin_point = i;
			break;
		}
	}
	return localmin_point;
}
int Getlocalmin_left_w(int *data, int start, int end)
{
	int i;

	int localmin_point = -1;
	for (i = start;i > end;i--)
	{
		if (data[i] <= data[i - 1]&&data[i] <= data[i-2]&&data[i] < data[i-3]&&
			data[i] < data[i+1]&&data[i]<data[i+2]&&data[i]<data[i+3])
		{
			localmin_point = i;
			break;
		}
	}
	return localmin_point;
}


int Getlocalmin_left(int *data, int start, int end)
{
	int i;

	int localmin_point = -1;
	for (i = start;i > end;i--)
	{
		if (data[i] <= data[i - 1]&&data[i] <= data[i-2]&&
			data[i] < data[i+1]&&data[i]<data[i+2]&&data[i]<data[i+3])
		{
			localmin_point = i;
			break;
		}
	}
	return localmin_point;
}
int Getlocalmax_right(int *data, int start, int end)
{
	int i;

	int localmax_point = -1;
	for (i = start;i < end;i++)
	{
		if (data[i] > data[i - 1]&&data[i] > data[i-2]&&data[i]>data[i-3]&&
			data[i] >= data[i+1]&&data[i]>=data[i+2])
		{
			localmax_point = i;
			break;
		}
	}
	return localmax_point;
}
int Getlocalmax_right_w(int *data, int start, int end)
{
	int i;

	int localmax_point = -1;
	for (i = start;i < end;i++)
	{
		if (data[i] > data[i - 1]&&data[i] > data[i-2]&&data[i]>data[i-3]&&
			data[i] >= data[i+1]&&data[i]>=data[i+2]&&data[i]>data[i+3])
		{
			localmax_point = i;
			break;
		}
	}
	return localmax_point;
}
int Getlocalmax_right_T(int *data, int start, int end)
{
	int i;

	int localmax_point = -1;
	for (i = start;i < end;i++)
	{
		if (data[i] > data[i - 1]&&data[i] > data[i-2]&&data[i]>data[i-3]&&data[i] > data[i-4]&&data[i]>data[i-5]&&
			data[i] >= data[i+1]&&data[i]>=data[i+2]&&data[i]>=data[i+3]&&data[i]>=data[i+5]&&data[i]>=data[i+4])
		{
			localmax_point = i;
			break;
		}
	}
	return localmax_point;
}
int Getlocalmax_left(int *data, int start, int end)
{
	int i;

	int localmax_point = -1;
	for (i = start;i > end;i--)
	{
		if (data[i] >= data[i - 1]&&data[i] >= data[i-2]&&
			data[i] > data[i+1]&&data[i]>data[i+2]&&data[i]>data[i+3])
		{
			localmax_point = i;
			break;
		}
	}
	return localmax_point;
}
int Getlocalmax_left_w(int *data, int start, int end)
{
	int i;

	int localmax_point = -1;
	for (i = start;i > end;i--)
	{
		if (data[i] >= data[i - 1]&&data[i] >= data[i-2]&&data[i] > data[i-3]&&
			data[i] > data[i+1]&&data[i]>data[i+2]&&data[i]>data[i+3])
		{
			localmax_point = i;
			break;
		}
	}
	return localmax_point;
}
void CheckLocalmaxormin(int *data,int *start,int *end)
{
	//	int i;
	int localmax_right,localmax_left,localmin_right,localmin_left;
	if (data[(*start + *end)/2] < 0)  // negative wave
	{
		localmin_left = Getlocalmin_left_w(data, *end, *start); // find the localmin from right to left
		localmin_right = Getlocalmin_right_w(data, *start, *end); // find the localmin from left to right
		if (abs(localmin_right - localmin_left) <= 2)
		{
			;
		}
		else
		{
			int localmax = Getlocalmax_right(data, localmin_right, localmin_left);
			if (localmax == -1)
			{
				return;
			}
			if (data[localmin_left] > data[localmin_right])
			{
				*end = localmax;
			}
			else
			{
				*start = localmax;
			}
		}
	}
	else
	{
		localmax_left = Getlocalmax_left_w(data, *end, *start);
		localmax_right = Getlocalmax_right_w(data, *start, *end);
		if ( abs(localmax_right - localmax_left) <= 2)
		{
			;
		}
		else
		{
			int localmin = Getlocalmin_right(data, localmax_right, localmax_left);
			if (localmin == -1)
			{
				return;
			}
			if (data[localmax_left] > data[localmax_right])
			{
				*start = localmin;
			}
			else
			{
				*end = localmin;
			}
		}
	}

}


int CheckTheNearestZero()

{
	int zeropoint_left = 0;
	int zeropoint_right = 0;
	int zeropoint_right_2 = 0;
	int dist = 0;

	int max_left = BeatBuffer_f[g_Iso_point];
	int min_left = 0;
	int max_right = 0;
	int min_right = 0;
	int max_height_right_2 = 0;

	int min_point_left = g_Iso_point;
	int min_point_right = g_Iso_point;
	int max_point_left = g_Iso_point;
	int max_point_right = g_Iso_point;
	int max_height_right = 0;
	int max_height_left = 0;
	int max_point = GetMaxPoint(BeatBuffer_aver,g_Iso_point - 30,g_Iso_point);
	int min_point = GetMinPoint(BeatBuffer_aver,g_Iso_point - 30,g_Iso_point);
	int max_dis = BeatBuffer_aver[max_point] - BeatBuffer_aver[min_point];
	int max_point_f = GetMaxPoint(BeatBuffer_f,g_Iso_point - 30, g_Iso_point);
	int min_point_f = GetMinPoint(BeatBuffer_f, g_Iso_point - 30, g_Iso_point);
	int max_dis_f = BeatBuffer_f[max_point_f] - BeatBuffer_f[min_point_f];

	///   to find the nearest zero point on the right
	for (dist = 0;dist < 20;dist++)
	{
		if (BeatBuffer_f[g_Iso_point + dist] > max_right)
		{
			max_right = BeatBuffer_f[g_Iso_point + dist];
			max_point_right = g_Iso_point + dist;
		}
		if (BeatBuffer_f[g_Iso_point + dist] < min_right)
		{
			min_right = BeatBuffer_f[g_Iso_point + dist];
			min_point_right = g_Iso_point + dist;
		}
		if (BeatBuffer_f[g_Iso_point + dist]*BeatBuffer_f[g_Iso_point + dist - 1] <= 0)
		{
			zeropoint_right = g_Iso_point + dist;
			break;
		}
	}
	//// to find the further zero point on the right
	if (zeropoint_right != 0)
	{
		for (dist=3;dist <= 25;dist++)
		{
			if (abs(BeatBuffer_f[zeropoint_right + dist]) > max_height_right_2)
			{
				max_height_right_2 = abs(BeatBuffer_f[zeropoint_right + dist]);
			}

			if (BeatBuffer_f[zeropoint_right + dist]*BeatBuffer_f[zeropoint_right + dist + 1] <= 0)
			{
				zeropoint_right_2 = zeropoint_right + dist;
				break;
			}
		}


	}
	if (zeropoint_right_2 == 0)
	{
		return 0;
	}
	dist = 0;
	////   to find the nearest zero point on the left
	for (;dist < 20;dist++)
	{
		if (BeatBuffer_f[g_Iso_point - dist] > max_left)
		{
			max_left = BeatBuffer_f[g_Iso_point - dist];
			max_point_left = g_Iso_point - dist;
		}
		if (BeatBuffer_f[g_Iso_point - dist] < min_left)
		{
			min_left = BeatBuffer_f[g_Iso_point - dist];
			min_point_left = g_Iso_point - dist;
		}
		if (BeatBuffer_f[g_Iso_point - dist]*BeatBuffer_f[g_Iso_point - dist + 1] <= 0)
		{
			zeropoint_left = g_Iso_point - dist;
			break;
		}
	}

	if (zeropoint_left == 0)
	{
		return 0;
	}
	////////      it's possible that there is a local peak which is nearly to be zero between the right and the left zero point
	if(abs(zeropoint_left - zeropoint_right) > 6)
	{
		int medium = (zeropoint_left + zeropoint_right)/2;
		int i;
		if (BeatBuffer_f[medium] > 0)  ////  if all points are positive, go to find the local min
		{
			for(i = zeropoint_left + 2;i < zeropoint_right - 1;i++)
			{
				if(BeatBuffer_f[i] < BeatBuffer_f[i - 1]&&BeatBuffer_f[i] <= BeatBuffer_f[i + 1]&&BeatBuffer_f[i] <= BeatBuffer_f[i + 2])
				{
					if(BeatBuffer_f[i] < 15)
						zeropoint_left = i;
					break;
				}
			}
		}
		if(BeatBuffer_f[medium] < 0)
		{
			for(i = zeropoint_left + 2;i < zeropoint_right - 1;i++)
			{
				if(BeatBuffer_f[i] > BeatBuffer_f[i - 1]&&BeatBuffer_f[i] >= BeatBuffer_f[i + 1]&&BeatBuffer_f[i] >= BeatBuffer_f[i + 2])
				{
					if(BeatBuffer_f[i] > -15)
						zeropoint_left = i;
					break;
				}
			}

		}
	}
	int i;
	int max_height = 0;
	for (i = zeropoint_left + 1;i < zeropoint_right - 1;i++)
	{
		if (abs(BeatBuffer_f[i]) > max_height)
		{
			max_height = abs(BeatBuffer_f[i]);
		}
	}
	max_point_f = GetMaxPoint(BeatBuffer_f,g_Iso_point - 30, zeropoint_right);
	min_point_f = GetMinPoint(BeatBuffer_f, g_Iso_point - 30, zeropoint_right);
	if (BeatBuffer_f[(zeropoint_right + zeropoint_left)/2] > 0)
	{
		int localmin_left = Getlocalmin_left(BeatBuffer_f,(zeropoint_right + zeropoint_left)/2, g_Iso_point - 20);
		int zeropoint_left_2 = GetZeropoint_left(BeatBuffer_f,localmin_left,g_Iso_point - 20);
		if (zeropoint_left_2 <= zeropoint_left&&zeropoint_left - zeropoint_left_2 <= 5)
		{
			int maxwavepoint = GetMaxPoint(BeatBuffer_f,g_Iso_point - 30,zeropoint_left_2);
			int minwavepoint = GetMinPoint(BeatBuffer_f,g_Iso_point - 30,zeropoint_left_2);
			if (max(abs(BeatBuffer_f[minwavepoint]),BeatBuffer_f[maxwavepoint]) >= 2*abs(BeatBuffer_f[localmin_left]))
			{
				return zeropoint_left;
			}
		}
	}
	if (BeatBuffer_f[(zeropoint_right + zeropoint_left)/2] < 0)
	{
		int localmax_left = Getlocalmax_left(BeatBuffer_f,(zeropoint_left + zeropoint_right)/2, g_Iso_point - 20);
		int zeropoint_left_2 = GetZeropoint_left(BeatBuffer_f,localmax_left,g_Iso_point - 20);
		if (zeropoint_left_2 <= zeropoint_left&&zeropoint_left - zeropoint_left_2 <= 5)
		{
			int maxwavepoint = GetMaxPoint(BeatBuffer_f,g_Iso_point - 30,zeropoint_left_2);
			int minwavepoint = GetMinPoint(BeatBuffer_f,g_Iso_point - 30,zeropoint_left_2);
			if (max(abs(BeatBuffer_f[minwavepoint]),BeatBuffer_f[maxwavepoint]) >= 2*abs(BeatBuffer_f[localmax_left]))
			{
				return zeropoint_left;
			}
		}
	}

	/////////////////////  TO judge the max_height wave is created by gibbs ring
	if (BeatBuffer_f[(zeropoint_right + zeropoint_left)/2] > 0)  ///  if the wave is positive
	{
		int i;
		int max_point = zeropoint_left ;
		int max_value = BeatBuffer_aver[zeropoint_left];
		for (i = zeropoint_left + 1;i <= zeropoint_right;i++)
		{
			if (BeatBuffer_aver[i] > max_value)
			{
				max_value = BeatBuffer_aver[i];
				max_point = i;
			}
		}
		float rate_1 = (float)min(abs(BeatBuffer_aver[zeropoint_left] - max_value),abs(BeatBuffer_aver[zeropoint_right] - max_value))/max_dis;
		float rate_2 = (float)max_height/max_dis_f;
		if (max_point == zeropoint_right ||max_point == zeropoint_left || rate_1*2<rate_2||abs(max_point - max_point_f)>=2)
		{
			return zeropoint_left;
		}
	}
	if (BeatBuffer_f[(zeropoint_right + zeropoint_left)/2] < 0) // if the wave is negative
	{
		int i;
		int min_point = zeropoint_left ;
		int min_value = BeatBuffer_aver[zeropoint_left];
		for (i = zeropoint_left;i <= zeropoint_right ;i++)
		{
			if (BeatBuffer_aver[i] < min_value)
			{
				min_value = BeatBuffer_aver[i];
				min_point = i;
			}
		}
		float rate_1 = (float)min(BeatBuffer_aver[zeropoint_left] - min_value, BeatBuffer_aver[zeropoint_right] - min_value)/max_dis;     //   the peak of this wave divided by the max to min distance in last 30 points from ISO
		float rate_2 = (float)max_height/max_dis_f;
		if ((min_point == zeropoint_right)||min_point == zeropoint_left|| rate_1*2< rate_2||abs(min_point - min_point_f) >=2)
		{
			return zeropoint_left;
		}
	}
	if (abs(zeropoint_right - g_Iso_point) > 4)
	{
		return zeropoint_left;
	}
	if (max_height > 10&&max_height_right_2 > 30)
	{
		return zeropoint_right;
	}
	else
		return zeropoint_left;

}

int CheckBiphasicWave(int *QRSonset,int *Pstart,int *Pend, int Polarity)

{
	int endpoint = g_Iso_point;
	int num_local_minormax = 0; /// zero point between min and max points
	int zeropointnum = 0;
	int startpoint = *QRSonset - 30;
	if (startpoint < 13)
	{
		startpoint = 13;
	}

	int i = startpoint;
	int maximum,max_point;
	int minimum,min_point;
	max_point = endpoint;
	min_point = endpoint;

	minimum = BeatBuffer_aver[min_point];
	maximum = BeatBuffer_aver[max_point];

	int max_f,min_f;
	int max_point_f,min_point_f;
	max_point_f = endpoint;
	min_point_f = endpoint;
	max_f = BeatBuffer_f[max_point_f];
	min_f = BeatBuffer_f[min_point_f];

	for (;i <= endpoint; i++)
	{
		if (BeatBuffer_aver[i] > maximum)
		{
			maximum = BeatBuffer_aver[i];
			max_point = i;
		}
		if (BeatBuffer_aver[i] < minimum)
		{
			minimum = BeatBuffer_aver[i];
			min_point = i;
		}
	}
	i = startpoint;
	for (;i <= endpoint; i++)
	{
		if (BeatBuffer_f[i] > max_f)
		{
			max_f = BeatBuffer_f[i];
			max_point_f = i;
		}
		if (BeatBuffer_f[i] < min_f)
		{
			min_f = BeatBuffer_f[i];
			min_point_f = i;
		}
	}

	int zeropoint_maxleft = GetZeropoint_left(BeatBuffer_f,max_point_f - 1,startpoint - 5);
	int zeropoint_maxright = GetZeropoint_right(BeatBuffer_f,max_point_f + 1, g_Iso_point + 1);
	int zeropoint_minleft = GetZeropoint_left(BeatBuffer_f,min_point_f - 1,startpoint - 5);
	int zeropoint_minright = GetZeropoint_right(BeatBuffer_f,min_point_f + 1, g_Iso_point + 1);
	/////  it's possible that the maxpoint or minpoint is not the peak;
	if (zeropoint_maxleft != -1&&zeropoint_maxright != -1)
	{
		int dis = max(abs(BeatBuffer_aver[zeropoint_maxleft] - BeatBuffer_aver[max_point_f]),abs(BeatBuffer_aver[zeropoint_maxright] - BeatBuffer_aver[max_point_f]));

		if (BeatBuffer_aver[max_point_f] > (BeatBuffer_aver[zeropoint_maxleft] + dis/2)&&BeatBuffer_aver[max_point_f] > (BeatBuffer_f[zeropoint_maxright] + dis/2))
		{
			;
		}
		else
		{
			int min_dis = min(BeatBuffer_aver[max_point_f] - BeatBuffer_aver[zeropoint_maxleft], BeatBuffer_aver[max_point_f] - BeatBuffer_aver[zeropoint_maxright]);
			int max_f_2 = 0;
			int max_point_f_2 = 0;
			for (i = *QRSonset - 30;i <= g_Iso_point; i ++)
			{
				if(i>=zeropoint_maxleft&&i<=zeropoint_maxright)
					continue;
				if (BeatBuffer_f[i] >= max_f_2)
				{
					max_f_2 = BeatBuffer_f[i];
					max_point_f_2 = i;
				}
			}
			int zeropoint_maxleft_2 = GetZeropoint_left(BeatBuffer_f,max_point_f - 1,startpoint - 5);
			int zeropoint_maxright_2 = GetZeropoint_right(BeatBuffer_f,max_point_f + 1, g_Iso_point + 1);
			if(zeropoint_maxleft_2 != -1&&zeropoint_maxright_2 != -1)
			{
				int dis_2 = max(abs(BeatBuffer_aver[zeropoint_maxleft_2] - BeatBuffer_aver[max_point_f_2]),abs(BeatBuffer_aver[zeropoint_maxright_2] - BeatBuffer_aver[max_point_f_2]));
				int min_dis_2 = min(BeatBuffer_aver[max_point_f_2] - BeatBuffer_aver[zeropoint_maxleft_2], BeatBuffer_aver[max_point_f_2] - BeatBuffer_aver[zeropoint_maxright_2]);
				if (zeropoint_maxright_2 - zeropoint_maxleft_2 > 5 && min_dis_2 > min_dis)
				{
					zeropoint_maxleft = zeropoint_maxleft_2;
					zeropoint_maxright = zeropoint_maxright_2;
					max_point_f = max_point_f_2;
				}
			}
		}
	}
	if (zeropoint_minleft != -1&&zeropoint_minright != -1)
	{
		//int dis = max(abs(BeatBuffer_aver[zeropoint_minleft] - BeatBuffer_aver[min_point_f]),abs(BeatBuffer_aver[zeropoint_minright] - BeatBuffer_aver[min_point_f]));
		//if (BeatBuffer_aver[min_point_f] < (BeatBuffer_aver[zeropoint_minleft] - dis/2)&&BeatBuffer_aver[min_point_f] < (BeatBuffer_aver[zeropoint_minright] - dis/2))
		//{
		//	;
		//}
		//else
		{
			int min_dis = min((BeatBuffer_aver[zeropoint_minleft+1] - BeatBuffer_aver[min_point_f]),(BeatBuffer_aver[zeropoint_minright-1] - BeatBuffer_aver[min_point_f]));
			int min_f_2 = 0;
			int min_point_f_2 = 0;
			for (i = *QRSonset - 30;i <= g_Iso_point; i++)
			{
				if (i>=zeropoint_minleft&&i<=zeropoint_minright)
				{
					continue;
				}
				if (BeatBuffer_f[i] <= min_f_2)
				{
					min_f_2 = BeatBuffer_f[i];
					min_point_f_2 = i;
				}
			}
			int zeropoint_minleft_2 = GetZeropoint_left(BeatBuffer_f,min_point_f_2 - 1,startpoint - 5);
			int zeropoint_minright_2 = GetZeropoint_right(BeatBuffer_f,min_point_f_2 + 1, g_Iso_point + 1);
			if (zeropoint_minright_2 != -1&&zeropoint_minleft_2 != -1)
			{
				int dis_2 = max(abs(BeatBuffer_aver[zeropoint_minleft_2] - BeatBuffer_aver[min_point_f_2]),abs(BeatBuffer_aver[zeropoint_minright_2] - BeatBuffer_aver[min_point_f_2]));
				int min_dis_2 = min((BeatBuffer_aver[zeropoint_minleft_2] - BeatBuffer_aver[min_point_f_2]),(BeatBuffer_aver[zeropoint_minright_2] - BeatBuffer_aver[min_point_f_2]));
				if ((zeropoint_minright_2 - zeropoint_minleft_2) > 5&&min_dis_2 > min_dis)
				{
					zeropoint_minleft = zeropoint_minleft_2;
					zeropoint_minright = zeropoint_minright_2;
					min_point_f = min_point_f_2;
				}
			}

		}
	}


	if (Polarity == 3)
	{  ///  the main wave should include the max or min wave in filtered result
		////  find the zero points beside the max and min wave



		int localmin_maxright = 0;
		int localmin_maxleft = 0;
		int localmax_minright = 0;
		int localmax_minleft = 0;
		int localmin_maxright_rightzero = 0;
		int minamp_maxpoint = 0;
		int minamp_minpoint = 0;
		float rate_height_width;
		if (zeropoint_minleft == -1||zeropoint_minright == -1) ///  then the max is the main part of P wave
		{
			if (zeropoint_maxright - zeropoint_maxleft == 0)
			{
				return 3;
			}
			rate_height_width = (float)(BeatBuffer_aver[max_point_f] - max(BeatBuffer_aver[zeropoint_maxleft],BeatBuffer_aver[zeropoint_maxright]))/(zeropoint_maxright - zeropoint_maxleft);
			if (zeropoint_maxleft <= startpoint + 3) // there's no biphasic on the left, so check the right side
			{
				localmin_maxright = Getlocalmin_right(BeatBuffer_f,zeropoint_maxright,g_Iso_point + 1);
				if (localmin_maxright == -1)  ///  there is no localminpoint on the right 
				{
					*Pstart = zeropoint_maxleft;
					*Pend = zeropoint_maxright;
					return 0;
				}
				//  check the right side
				int zeropoint_right = GetZeropoint_right(BeatBuffer_f,localmin_maxright,g_Iso_point + 1);
				if (zeropoint_right == -1)  ////  there is no zero point, then look for local max
				{
					zeropoint_right = Getlocalmax_right(BeatBuffer_f,localmin_maxright,g_Iso_point + 1);
					if (zeropoint_right == -1) // still fail
					{
						*Pstart = zeropoint_maxleft;
						*Pend = (localmin_maxright + zeropoint_maxright)/2;
						return 0;
					}
					else
					{
						if (zeropoint_right - zeropoint_maxright == 0)
						{
							*Pstart = zeropoint_maxleft;
							*Pend = (localmin_maxright + zeropoint_maxright)/2;
							return 0;
						}
						if (BeatBuffer_aver[localmin_maxright] < BeatBuffer_aver[zeropoint_right]&&
							BeatBuffer_aver[localmin_maxright] < BeatBuffer_aver[zeropoint_maxright])
						{
							if ((float)(min(BeatBuffer_aver[zeropoint_right],BeatBuffer_aver[zeropoint_maxright - 1]) -
										BeatBuffer_aver[localmin_maxright])/(zeropoint_right - zeropoint_maxright) >= rate_height_width*0.4)
							{
								*Pstart = zeropoint_maxleft;
								*Pend = zeropoint_right;
								return 1;
							}
							else
							{
								*Pstart = zeropoint_maxleft;
								*Pend = (localmin_maxright + zeropoint_maxright)/2;
								return 0;
							}
						}
						else
						{
							*Pstart = zeropoint_maxleft;
							*Pend = (localmin_maxright + zeropoint_maxright)/2;
							return 0;
						}
					}
				}
				else  //   if there is a zeropoint_right
				{
					if (zeropoint_right - zeropoint_maxright == 0)
					{
						*Pstart = zeropoint_maxleft;
						*Pend = (localmin_maxright + zeropoint_maxright)/2;
						return 0;
					}
					if (BeatBuffer_aver[localmin_maxright] < BeatBuffer_aver[zeropoint_right]&&
						BeatBuffer_aver[localmin_maxright] < BeatBuffer_aver[zeropoint_maxright])
					{
						if ((float)(min(BeatBuffer_aver[zeropoint_right],BeatBuffer_aver[zeropoint_maxright - 1])
									- BeatBuffer_aver[localmin_maxright])/(zeropoint_right - zeropoint_maxright) >= rate_height_width*0.4)
						{
							*Pstart = zeropoint_maxleft;
							*Pend = zeropoint_right;
							return 1;
						}
						else
						{
							*Pstart = zeropoint_maxleft;
							*Pend = (localmin_maxright + zeropoint_maxright)/2;
							return 0;
						}
					}
					else
					{
						*Pstart = zeropoint_maxleft;
						*Pend = (localmin_maxright + zeropoint_maxright)/2;
						return 0;
					}
				}
			}
			else if(zeropoint_maxright > g_Iso_point - 3)
			{
				localmin_maxleft = Getlocalmin_left(BeatBuffer_f,zeropoint_maxleft,*QRSonset - 35);
				if (localmin_maxleft == -1)
				{
					*Pstart = zeropoint_maxleft;
					*Pend = zeropoint_maxright;
					return 0;
				}
				int zeropoint_left = GetZeropoint_left(BeatBuffer_f,localmin_maxleft,*QRSonset - 40);
				if (zeropoint_left == -1)
				{
					zeropoint_left = Getlocalmax_left(BeatBuffer_f,localmin_maxleft,*QRSonset - 40);
					if (zeropoint_left == -1)
					{
						*Pstart = (zeropoint_maxleft + localmin_maxleft)/2;
						*Pend = zeropoint_maxright;
						return 0;
					}
				}
				/////////  two else situation can be combined
				if (zeropoint_maxleft - zeropoint_left == 0)
				{
					*Pstart = (zeropoint_maxleft + localmin_maxleft)/2;
					*Pend = zeropoint_maxright;
					return 0;
				}
				if (BeatBuffer_aver[localmin_maxleft] < BeatBuffer_aver[zeropoint_left]&&
					BeatBuffer_aver[localmin_maxleft] < BeatBuffer_aver[zeropoint_maxleft])
				{
					if ((float)min(BeatBuffer_aver[zeropoint_left]-BeatBuffer_aver[localmin_maxleft],BeatBuffer_aver[localmin_maxleft + 3]
																									 - BeatBuffer_aver[localmin_maxleft])/(zeropoint_maxleft - zeropoint_left) > 0.5*rate_height_width)
					{
						*Pstart = zeropoint_left;
						*Pend = zeropoint_maxright;
						return 1;
					}
				}
				*Pstart = (zeropoint_maxleft + localmin_maxleft)/2;
				*Pend = zeropoint_maxright;
				return 0;

			}
			else
			{
				int zeropoint_right;
				int zeropoint_left;
				bool b_leftbiphasic = false;
				bool b_rightbiphasic = false;
				localmin_maxright = Getlocalmin_right(BeatBuffer_f,zeropoint_maxright,g_Iso_point + 1);
				localmin_maxleft = Getlocalmin_left(BeatBuffer_f,zeropoint_maxleft,*QRSonset - 35);
				if (localmin_maxright != -1)
				{
					zeropoint_right = GetZeropoint_right(BeatBuffer_f,localmin_maxright,g_Iso_point + 1);
					if (zeropoint_right == -1)  ////  there is no zero point, then look for local max
					{
						zeropoint_right = Getlocalmax_right(BeatBuffer_f,localmin_maxright,g_Iso_point + 1);
					}
					if (zeropoint_right != -1)
					{
						if (BeatBuffer_aver[localmin_maxright] < BeatBuffer_aver[zeropoint_right])
						{
							if (zeropoint_right == zeropoint_maxright)
							{
								b_rightbiphasic = false;
							}
							else
							{
								if ((float)(min(BeatBuffer_aver[zeropoint_right],BeatBuffer_aver[localmin_maxright-3]) -
											BeatBuffer_aver[localmin_maxright])/(zeropoint_right - zeropoint_maxright) >= rate_height_width*0.4&&abs(zeropoint_right - zeropoint_maxleft) <= 18)
								{
									b_rightbiphasic = true;
								}
							}
						}
					}
				}
				if (localmin_maxleft != -1)
				{
					zeropoint_left = GetZeropoint_left(BeatBuffer_f,localmin_maxleft,*QRSonset - 35);
					if (zeropoint_left == -1)
					{
						zeropoint_left = Getlocalmax_left(BeatBuffer_f,localmin_maxleft,*QRSonset - 35);
					}
					if (zeropoint_left != -1)
					{
						if (BeatBuffer_aver[localmin_maxleft] < BeatBuffer_aver[zeropoint_left])
						{
							if (zeropoint_maxleft == zeropoint_left)
							{
								b_leftbiphasic = false;
							}
							else
							{
								if ((float)min(BeatBuffer_aver[zeropoint_left]-BeatBuffer_aver[localmin_maxleft],BeatBuffer_aver[zeropoint_maxleft+1] - BeatBuffer_aver[localmin_maxleft])/(zeropoint_maxleft - zeropoint_left) >= 0.4*rate_height_width&&abs(zeropoint_left - zeropoint_maxright) <= 18)
								{
									b_leftbiphasic = true;
								}
							}
						}
					}
				}
				if (b_leftbiphasic == true&&b_rightbiphasic == true)
				{
					if(min(BeatBuffer_aver[zeropoint_left]-BeatBuffer_aver[localmin_maxleft],BeatBuffer_aver[zeropoint_maxleft+1] - BeatBuffer_aver[localmin_maxleft])/(zeropoint_maxleft - zeropoint_left) >
					   (min(BeatBuffer_aver[zeropoint_right],BeatBuffer_aver[zeropoint_maxright-1]) - BeatBuffer_aver[localmin_maxright])/(zeropoint_right - zeropoint_maxright))
					{
						b_rightbiphasic = false;
					}
					else
					{
						b_leftbiphasic = false;
					}
				}
				if (b_leftbiphasic)
				{
					*Pstart = zeropoint_left;
					*Pend = zeropoint_maxright;
					return 1;
				}
				else if (b_rightbiphasic)
				{
					*Pstart = zeropoint_maxleft;
					*Pend = zeropoint_right;
					return 1;
				}
				else
				{
					*Pstart = zeropoint_maxleft;
					*Pend = zeropoint_maxright;


					if (localmin_maxright != -1)
					{
						if (localmin_maxright > zeropoint_maxright)
						{
							if (localmin_maxright - zeropoint_maxright <= 3)
							{
								*Pend = localmin_maxright;
							}
							else
								*Pend = (localmin_maxright + zeropoint_maxright + 1)/2;
						}
						if (zeropoint_maxright > localmin_maxright)
						{
							if (zeropoint_maxright - localmin_maxright <= 3)
							{
								*Pend = zeropoint_right;
							}
							else
								*Pend = (localmin_maxright + zeropoint_maxright + 1)/2;

						}

					}
					if (localmin_maxleft != -1)
					{
						if (zeropoint_maxleft > localmin_maxleft)
						{
							if (zeropoint_maxleft - localmin_maxleft <= 3)
							{
								*Pstart = localmin_maxleft;
							}
							else
								*Pstart = (localmin_maxleft + zeropoint_maxleft + 1)/2;
						}
						if (zeropoint_maxleft < localmin_maxleft)
						{
							if(localmin_maxleft - zeropoint_maxleft <= 3)
								*Pstart = zeropoint_maxleft;
							else
								*Pstart = (localmin_maxleft + zeropoint_maxleft + 1)/2;
						}

					}
					return 0;
				}
			}

		}
		if (zeropoint_maxleft == -1||zeropoint_maxright == -1)
		{
			int zeropoint_right;
			int zeropoint_left;
			bool b_leftbiphasic = false;
			bool b_rightbiphasic = false;
			localmax_minright = Getlocalmax_right(BeatBuffer_f,zeropoint_minright,g_Iso_point+1);
			localmax_minleft = Getlocalmax_left(BeatBuffer_f,zeropoint_minleft,*QRSonset - 35);
			int max_positivewave;
			int min_negativewave;
			max_positivewave = GetMaxPoint(BeatBuffer_aver,zeropoint_maxleft,zeropoint_maxright);
			min_negativewave = GetMinPoint(BeatBuffer_aver,zeropoint_minleft,zeropoint_minright);
			int positive_negative_or = 0;   // 1 positive  2  negative   3  or unsure
			int height_maxpeak = BeatBuffer_aver[max_positivewave] - max(BeatBuffer_aver[zeropoint_maxleft + 1],BeatBuffer_aver[zeropoint_maxright]);
			int height_minpeak = min(BeatBuffer_aver[zeropoint_minleft+1],BeatBuffer_aver[zeropoint_minright-1]) - BeatBuffer_aver[min_negativewave];


			float rate_heightwidth_max,rate_heightwidth_min;

			if (zeropoint_maxright - zeropoint_maxleft >= 6)
			{
				rate_heightwidth_max = (float)height_maxpeak/(zeropoint_maxright - zeropoint_maxleft);
			}
			else
				rate_heightwidth_max = 0;
			if (zeropoint_minright - zeropoint_minleft >= 6)
			{
				rate_heightwidth_min = (float)height_minpeak/(zeropoint_minright - zeropoint_minleft);
			}
			else
				rate_heightwidth_min = 0;
			rate_height_width = (float)(min(BeatBuffer_aver[zeropoint_minleft+1],BeatBuffer_aver[zeropoint_minright-1]) - BeatBuffer_aver[min_point_f])/(zeropoint_minright - zeropoint_minleft);
			if (localmax_minright != -1)
			{
				zeropoint_right = GetZeropoint_right(BeatBuffer_f,localmax_minright,g_Iso_point + 1);
				if (zeropoint_right == -1)  ////  there is no zero point, then look for local max
				{
					zeropoint_right = Getlocalmin_right(BeatBuffer_f,localmax_minright,g_Iso_point+1);
					if (zeropoint_right == localmax_minright)
					{
						zeropoint_right = -1;
					}
				}
				if (zeropoint_right - zeropoint_maxright == 0)
				{
					b_rightbiphasic = false;
				}
				else
				{
					if (zeropoint_right != -1)
					{
						if (BeatBuffer_aver[localmax_minright] > BeatBuffer_aver[zeropoint_right]&&
							BeatBuffer_aver[localmax_minright] > BeatBuffer_aver[zeropoint_maxright])
						{
							if ((float)abs(max(BeatBuffer_aver[zeropoint_right],BeatBuffer_aver[zeropoint_maxright]) - BeatBuffer_aver[localmax_minright])/(zeropoint_right - zeropoint_maxright) > rate_heightwidth_min*0.5)
							{
								b_rightbiphasic = true;
							}
						}
					}
				}
			}
			if (localmax_minleft != -1)
			{
				zeropoint_left = GetZeropoint_left(BeatBuffer_f,localmax_minleft,*QRSonset - 35);
				if (zeropoint_left == -1)
				{
					zeropoint_left = Getlocalmax_left(BeatBuffer_f,localmax_minleft,*QRSonset - 35);
				}
				if (zeropoint_left != -1)
				{
					if (zeropoint_minleft == zeropoint_left)
					{
						b_leftbiphasic = false;
					}
					else
					{
						if (BeatBuffer_aver[localmax_minleft] > BeatBuffer_aver[zeropoint_left]&&
							BeatBuffer_aver[localmax_minleft] > BeatBuffer_aver[zeropoint_minleft])
						{
							if ((float)min(BeatBuffer_aver[localmax_minleft]-BeatBuffer_aver[zeropoint_left],BeatBuffer_aver[localmax_minleft] - BeatBuffer_aver[zeropoint_minleft+1])/(zeropoint_minleft - zeropoint_left) > 0.5*rate_heightwidth_min)
							{
								b_leftbiphasic = true;
							}
						}
					}
				}
			}
			if(b_leftbiphasic)
			{
				if (zeropoint_minright - zeropoint_left >= 20)
				{
					b_leftbiphasic = false;
				}
			}
			if (b_rightbiphasic)
			{
				if (zeropoint_right - zeropoint_minleft >= 20)
				{
					b_rightbiphasic = false;
				}
			}
			if (b_leftbiphasic == true&&b_rightbiphasic == true)
			{
				if((float)min(BeatBuffer_aver[localmax_minleft+1]-BeatBuffer_aver[zeropoint_left],BeatBuffer_aver[localmax_minleft] - BeatBuffer_aver[zeropoint_minleft+1])/(zeropoint_minleft - zeropoint_left) >
				   (float)abs(max(BeatBuffer_aver[zeropoint_right],BeatBuffer_aver[zeropoint_maxright-1]) - BeatBuffer_aver[localmax_minright])/(zeropoint_right - zeropoint_maxright))
				{
					b_rightbiphasic = false;
				}
				else
				{
					b_leftbiphasic = false;
				}
			}

			if (b_leftbiphasic)
			{
				*Pstart = zeropoint_left;
				*Pend = zeropoint_minright;
				return 1;
			}
			else if (b_rightbiphasic)
			{
				*Pstart = zeropoint_minleft;
				*Pend = zeropoint_right;
				return 1;
			}
			else
			{
				*Pstart = zeropoint_minleft;
				*Pend = zeropoint_minright;
				if (localmax_minright != -1)
				{
					*Pend = (localmax_minright + zeropoint_minright + 1)/2;
				}
				if (localmax_minleft != -1)
				{
					*Pstart = (localmax_minleft + zeropoint_minleft + 1)/2;
				}
				return 0;
			}
		}
		else
		{
			////  first find the main wave from two waves
			int max_positivewave;
			int min_negativewave;
			max_positivewave = GetMaxPoint(BeatBuffer_aver,zeropoint_maxleft,zeropoint_maxright);
			min_negativewave = GetMinPoint(BeatBuffer_aver,zeropoint_minleft,zeropoint_minright);
			int positive_negative_or = 0;   // 1 positive  2  negative   3  or unsure
			int height_maxpeak = BeatBuffer_aver[max_positivewave] - max(BeatBuffer_aver[zeropoint_maxleft],BeatBuffer_aver[zeropoint_maxright]);
			int height_minpeak = min(BeatBuffer_aver[zeropoint_minleft],BeatBuffer_aver[zeropoint_minright]) - BeatBuffer_aver[min_negativewave];


			float rate_heightwidth_max,rate_heightwidth_min;
			if (abs(*QRSonset - zeropoint_maxright) <= 5)
			{
				height_maxpeak = 0;
				rate_heightwidth_max = 0;
			}
			else
			{
				if (zeropoint_maxright - zeropoint_maxleft >= 5)
				{
					rate_heightwidth_max = (float)height_maxpeak/(zeropoint_maxright - zeropoint_maxleft);
				}
				else
				{
					height_maxpeak = 0;
					rate_heightwidth_max = 0;
				}
			}
			if (abs(*QRSonset - zeropoint_minright) <= 5)
			{
				height_minpeak = 0;
				rate_heightwidth_min = 0;
			}
			else
			{
				if (zeropoint_minright - zeropoint_minleft >= 5)
				{
					rate_heightwidth_min = (float)height_minpeak/(zeropoint_minright - zeropoint_minleft);
				}
				else
				{
					height_minpeak = 0;
					rate_heightwidth_min = 0;
				}
			}
			if (rate_heightwidth_max > 2*rate_heightwidth_min&&rate_heightwidth_min > 0.4)
			{
				positive_negative_or = 1;
			}
			else if (rate_heightwidth_min > 2*rate_heightwidth_min&&rate_heightwidth_max > 0.4)
			{
				positive_negative_or = 2;
			}
			else if (height_maxpeak > height_minpeak*2)
			{
				positive_negative_or = 1;
				////   positive is the main wave
			}
			else if (height_minpeak > 2*height_maxpeak)
			{
				positive_negative_or = 2;
				//////    negative is the main wave
			}
			else
			{
				if (*QRSonset - zeropoint_maxleft <= 12|| *QRSonset - zeropoint_maxleft >= 25)
				{
					positive_negative_or = 2;
					///////   negative
				}
				else if(*QRSonset - zeropoint_minleft <= 12 || *QRSonset - zeropoint_minleft >= 25)
				{
					positive_negative_or = 1;
					///      positive
				}
				else if (height_maxpeak > height_minpeak)
				{
					positive_negative_or = 1;
					////    positive
				}
				else
				{
					positive_negative_or = 2;
					/////   negative
				}

			}
			if (positive_negative_or == 1)  ////   the positive wave is the main wave
			{
				int zeropoint_right = zeropoint_maxright;
				int zeropoint_left = zeropoint_maxleft;
				bool b_leftbiphasic = false;
				bool b_rightbiphasic = false;

				localmin_maxright = Getlocalmin_right(BeatBuffer_f,zeropoint_maxright,g_Iso_point + 1);
				localmin_maxleft = Getlocalmin_left(BeatBuffer_f,zeropoint_maxleft,*QRSonset - 35);
				if (localmin_maxright != -1)
				{
					zeropoint_right = GetZeropoint_right(BeatBuffer_f,localmin_maxright,g_Iso_point);
					if (zeropoint_right == -1)  ////  there is no zero point, then look for local max
					{
						zeropoint_right = Getlocalmax_right(BeatBuffer_f,localmin_maxright,g_Iso_point + 1);

					}
					if (zeropoint_right != -1)
					{
						if (zeropoint_right - zeropoint_maxright == 0)
						{
							b_rightbiphasic = false;
						}
						else
						{
							if (abs(zeropoint_maxright - localmin_maxright) <= 3)
							{
								int peakpoint = Getlocalmin_right(BeatBuffer_aver,zeropoint_maxright - 1,zeropoint_right);
								int peak_center_s = BeatBuffer_aver[peakpoint];
								int peak_left_s = BeatBuffer_aver[peakpoint - 3];
								int peak_right_s = BeatBuffer_aver[peakpoint + 3];
								int min_dis_s = min(abs(peak_center_s - peak_left_s),abs(peak_right_s - peak_center_s));
								int peak_center_m = BeatBuffer_aver[max_positivewave];
								int peak_left_m = BeatBuffer_aver[max_positivewave - 3];
								int peak_right_m = BeatBuffer_aver[max_positivewave + 3];
								int min_dis_m = min(abs(peak_right_m - peak_center_m),abs(peak_left_m - peak_center_m));
								if (peakpoint == -1)
								{
									b_rightbiphasic = false;
								}
								else if (abs(zeropoint_maxright - zeropoint_right)*1.5 <= abs(zeropoint_maxleft - zeropoint_maxright)&&2*min_dis_s >= min_dis_m)
								{
									b_rightbiphasic = true;
								}
							}
							else if (BeatBuffer_aver[localmin_maxright] < BeatBuffer_aver[zeropoint_right]&&
									 BeatBuffer_aver[localmin_maxright] < BeatBuffer_aver[zeropoint_maxright])
							{
								if ((float)(min(BeatBuffer_aver[zeropoint_right],BeatBuffer_aver[zeropoint_maxright - 1]) - BeatBuffer_aver[localmin_maxright])/(zeropoint_right - zeropoint_maxright) > rate_heightwidth_max*0.5)
								{
									b_rightbiphasic = true;
								}
							}
						}
					}
				}
				else
				{
					b_rightbiphasic = false;
				}
				if (localmin_maxleft != -1)
				{
					zeropoint_left = GetZeropoint_left(BeatBuffer_f,localmin_maxleft,*QRSonset - 35);
					if (zeropoint_left == -1)
					{
						zeropoint_left = Getlocalmax_left(BeatBuffer_f,localmin_maxleft,*QRSonset - 35);

					}
					if (zeropoint_left != -1)
					{
						if (abs(zeropoint_maxleft - localmin_maxleft) <= 3)
						{
							int peakpoint = Getlocalmin_left(BeatBuffer_aver,zeropoint_maxleft + 1,zeropoint_left);
							int peak_center_s = BeatBuffer_aver[peakpoint];
							int peak_left_s = BeatBuffer_aver[peakpoint - 3];
							int peak_right_s = BeatBuffer_aver[peakpoint + 3];
							int min_dis_s = min(abs(peak_center_s - peak_left_s),abs(peak_right_s - peak_center_s));
							int peak_center_m = BeatBuffer_aver[max_positivewave];
							int peak_left_m = BeatBuffer_aver[max_positivewave - 3];
							int peak_right_m = BeatBuffer_aver[max_positivewave + 3];
							int min_dis_m = min(abs(peak_right_m - peak_center_m),abs(peak_left_m - peak_center_m));
							if (peakpoint == -1)
							{
								b_leftbiphasic = false;
							}
							else if (abs(zeropoint_maxleft - zeropoint_left)*1.5 <= abs(zeropoint_maxleft - zeropoint_maxright)&&2*min_dis_s >= min_dis_m)
							{
								b_leftbiphasic = true;
							}
						}
						else if (BeatBuffer_aver[localmin_maxleft] < BeatBuffer_aver[zeropoint_left]&&
								 BeatBuffer_aver[localmin_maxleft] < BeatBuffer_aver[zeropoint_maxleft])
						{
							if (zeropoint_maxleft - zeropoint_left == 0)
							{
								b_leftbiphasic = false;
							}
							else
							{
								if (min(BeatBuffer_aver[zeropoint_left]-BeatBuffer_aver[localmin_maxleft],BeatBuffer_aver[zeropoint_maxleft] - BeatBuffer_aver[localmin_maxleft])/(zeropoint_maxleft - zeropoint_left) > 0.8*rate_heightwidth_max)
								{
									b_leftbiphasic = true;
								}
							}
						}
					}
				}
				else
					b_leftbiphasic = false;
				if (b_leftbiphasic)
				{
					if (zeropoint_maxright - zeropoint_left >= 20)
					{
						b_leftbiphasic = false;
					}
				}
				if (b_rightbiphasic)
				{
					if (zeropoint_right - zeropoint_maxleft >= 20)
					{
						b_rightbiphasic = false;
					}
				}
				if (zeropoint_right - zeropoint_maxright == 0)
				{
					b_rightbiphasic = false;
				}
				if (zeropoint_left == zeropoint_maxleft)
				{
					b_leftbiphasic = false;
				}
				if (b_leftbiphasic == true&&b_rightbiphasic == true)
				{
					if(min(BeatBuffer_aver[zeropoint_left]-BeatBuffer_aver[localmin_maxleft],BeatBuffer_aver[zeropoint_maxleft] - BeatBuffer_aver[localmin_maxleft])/(zeropoint_maxleft - zeropoint_left) >
					   (min(BeatBuffer_aver[zeropoint_right],BeatBuffer_aver[zeropoint_maxright]) - BeatBuffer_aver[localmin_maxright])/(zeropoint_right - zeropoint_maxright))
					{
						b_rightbiphasic = false;
					}
					else
					{
						b_leftbiphasic = false;
					}
				}
				if (b_leftbiphasic)
				{
					*Pstart = zeropoint_left;
					*Pend = zeropoint_maxright;
					return 1;
				}
				else if (b_rightbiphasic)
				{
					*Pstart = zeropoint_maxleft;
					*Pend = zeropoint_right;
					return 1;
				}
				else
				{
					*Pstart = zeropoint_maxleft;
					*Pend = zeropoint_maxright;
					if (localmin_maxright != -1&&abs(localmin_maxright - zeropoint_maxright)<=3)
					{
						*Pend = localmin_maxright ;
					}
					else if(localmin_maxright != -1)
					{
						*Pend = (localmin_maxright + zeropoint_maxright)/2;
					}
					else
					{
						*Pend = zeropoint_maxright;
					}
					if (localmin_maxleft != -1&&abs(localmin_maxleft - zeropoint_maxleft)<=3)
					{
						*Pstart = localmin_maxleft ;
					}
					else if(localmin_maxleft != -1)
					{
						*Pstart = (localmin_maxleft + zeropoint_maxleft)/2;
					}
					else
					{
						*Pstart = zeropoint_maxleft;
					}
					return 0;
				}
			}
			if (positive_negative_or == 2)
			{
				int zeropoint_right = zeropoint_minright;
				int zeropoint_left = zeropoint_minleft;
				bool b_leftbiphasic = false;
				bool b_rightbiphasic = false;
				localmax_minright = Getlocalmax_right(BeatBuffer_f,zeropoint_minright,g_Iso_point + 1);
				localmax_minleft = Getlocalmax_left(BeatBuffer_f,zeropoint_minleft,*QRSonset - 35);
				rate_height_width = (float)(min(BeatBuffer_aver[zeropoint_minleft],BeatBuffer_aver[zeropoint_minright]) - BeatBuffer_aver[min_point_f])/(zeropoint_minright - zeropoint_minleft);
				if (localmax_minright != -1)
				{
					zeropoint_right = GetZeropoint_right(BeatBuffer_f,localmax_minright,g_Iso_point + 1);
					if (zeropoint_right == -1)  ////  there is no zero point, then look for local max
					{
						zeropoint_right = Getlocalmin_right(BeatBuffer_f,localmax_minright,g_Iso_point + 1);

					}
					if (zeropoint_right != -1) //  it's possible that localmax_minright is so closed to the zeropointminright, so we can check whether there
						///   is a peak between zeropoint_minright - 1 and localmax_minright
					{
						if (abs(zeropoint_minright - localmax_minright) <= 3)
						{
							int peakpoint = Getlocalmax_right(BeatBuffer_aver,zeropoint_minright - 1,zeropoint_right);
							int peak_center_s = BeatBuffer_aver[peakpoint];
							int peak_left_s = BeatBuffer_aver[peakpoint - 3];
							int peak_right_s = BeatBuffer_aver[peakpoint + 3];
							int min_dis_s = min(abs(peak_center_s - peak_left_s),abs(peak_right_s - peak_center_s));
							int peak_center_m = BeatBuffer_aver[min_negativewave];
							int peak_left_m = BeatBuffer_aver[min_negativewave - 3];
							int peak_right_m = BeatBuffer_aver[min_negativewave + 3];
							int min_dis_m = min(abs(peak_right_m - peak_center_m),abs(peak_left_m - peak_center_m));
							if (peakpoint == -1)
							{
								b_rightbiphasic = false;
							}
							else if (abs(localmax_minright - zeropoint_right)*1.5 <= abs(localmax_minright - localmax_minleft)&&2*min_dis_s >= min_dis_m )
							{
								b_rightbiphasic = true;
							}
						}
						else if (BeatBuffer_aver[localmax_minright] > BeatBuffer_aver[zeropoint_right]&&
								 BeatBuffer_aver[localmax_minright] > BeatBuffer_aver[zeropoint_minright])
						{
							if (zeropoint_right == zeropoint_minright)
							{
								b_rightbiphasic = false;
							}
							else
							{
								if (abs(max(BeatBuffer_aver[zeropoint_right],BeatBuffer_aver[zeropoint_maxright]) - BeatBuffer_aver[localmax_minright])/(zeropoint_right - zeropoint_minright) > rate_heightwidth_min*0.8)
								{
									b_rightbiphasic = true;
								}
							}
						}
					}
				}
				if (localmax_minleft != -1)
				{
					zeropoint_left = GetZeropoint_left(BeatBuffer_f,localmax_minleft,*QRSonset - 35);
					if (zeropoint_left == -1)
					{
						zeropoint_left = Getlocalmax_left(BeatBuffer_f,localmax_minleft,*QRSonset - 35);

					}
					if (zeropoint_left != -1)
					{
						if (abs(zeropoint_minleft - localmax_minleft) <= 3)
						{
							int peakpoint = Getlocalmax_left(BeatBuffer_aver,zeropoint_minleft + 1,zeropoint_left);
							int peak_center_s = BeatBuffer_aver[peakpoint];
							int peak_left_s = BeatBuffer_aver[peakpoint - 3];
							int peak_right_s = BeatBuffer_aver[peakpoint + 3];
							int min_dis_s = min(abs(peak_center_s - peak_left_s),abs(peak_right_s - peak_center_s));
							int peak_center_m = BeatBuffer_aver[min_negativewave];
							int peak_left_m = BeatBuffer_aver[min_negativewave - 3];
							int peak_right_m = BeatBuffer_aver[min_negativewave + 3];
							int min_dis_m = min(abs(peak_right_m - peak_center_m),abs(peak_left_m - peak_center_m));

							if (peakpoint == -1)
							{
								b_leftbiphasic = false;
							}
							else if (abs(localmax_minleft - peakpoint)*1.5 <= abs(localmax_minright - localmax_minleft) && 2*min_dis_s >= min_dis_m )
							{
								b_leftbiphasic = true;
							}
						}
						else if (BeatBuffer_aver[localmax_minleft] > BeatBuffer_aver[zeropoint_left]&&
								 BeatBuffer_aver[localmax_minleft] > BeatBuffer_aver[zeropoint_minleft])
						{
							if (zeropoint_minleft == zeropoint_left)
							{
								b_leftbiphasic = false;
							}
							else
							{
								if (min(BeatBuffer_aver[localmax_minleft]-BeatBuffer_aver[zeropoint_left],BeatBuffer_aver[localmax_minleft] - BeatBuffer_aver[zeropoint_minleft + 1])/(zeropoint_minleft - zeropoint_left) > 0.5*rate_heightwidth_min)
								{
									b_leftbiphasic = true;
								}
							}

						}
					}
				}
				if(b_leftbiphasic)
				{
					if (zeropoint_minright - zeropoint_left >= 20)
					{
						b_leftbiphasic = false;
					}
				}
				if (b_rightbiphasic)
				{
					if (zeropoint_right - zeropoint_minleft >= 20)
					{
						b_rightbiphasic = false;
					}
				}
				if (zeropoint_right - zeropoint_minright == 0)
				{
					b_rightbiphasic = false;
				}
				if (zeropoint_minleft - zeropoint_left == 0)
				{
					b_leftbiphasic = false;
				}
				if (b_leftbiphasic == true&&b_rightbiphasic == true)
				{

					if(min(BeatBuffer_aver[localmax_minleft]-BeatBuffer_aver[zeropoint_left],BeatBuffer_aver[localmax_minleft] - BeatBuffer_aver[zeropoint_minleft + 1])/(zeropoint_minleft - zeropoint_left) >
					   abs(max(BeatBuffer_aver[zeropoint_right],BeatBuffer_aver[zeropoint_maxright - 1]) - BeatBuffer_aver[localmax_minright])/(zeropoint_right - zeropoint_minright))
					{
						b_rightbiphasic = false;
					}
					else
					{
						b_leftbiphasic = false;
					}
				}

				if (b_leftbiphasic)
				{
					*Pstart = zeropoint_left;
					*Pend = zeropoint_minright;
					return 1;
				}
				else if (b_rightbiphasic)
				{
					*Pstart = zeropoint_minleft;
					*Pend = zeropoint_right;
					return 1;
				}
				else
				{
					*Pstart = zeropoint_minleft;
					*Pend = zeropoint_minright;
					if (localmax_minright != -1)
					{
						if (abs(localmax_minright - zeropoint_minright) > 3)
						{
							*Pend = (localmax_minright + zeropoint_minright + 1)/2;
						}
						else
							*Pend = localmax_minright;
					}
					if(localmax_minleft != -1)
					{
						if (abs(localmax_minleft - zeropoint_minleft) > 3)
						{
							*Pstart = (localmax_minleft + zeropoint_minleft + 1)/2;
						}
						else
							*Pstart = localmax_minleft;
					}
					return 0;
				}
			}
		}

	}
	return 0;

}
int Find_T_Polarity(int *endbeat, int *QRSoffset, int *peakpoint)
{
	int endpoint;
	if (*QRSoffset < 60)
	{
		endpoint = *QRSoffset + 60;
	}
	else
		endpoint = *QRSoffset + 45;
	if (endpoint > *endbeat)
	{
		endpoint = *endbeat;
	}
	if (endpoint >= 135)
	{
		endpoint = 135;
	}
//	bool b_baseline = CheckBaseline(&endpoint,QRSoffset,peakpoint);
	//	int referencepoint = GetZeropoint_right(BeatBuffer_f,g_Iso_point_off,endpoint);
	int referencepoint = *QRSoffset + 15;
	if (referencepoint >= 135)
	{
		int i;
		int localmaxpoint_3 = -1;
		int localminpoint_3 = -1;
		for (i = referencepoint;i <= 144;i ++)
		{
			if (BeatBuffer_aver[i] >= BeatBuffer_aver[i + 1]&&BeatBuffer_aver[i] >= BeatBuffer_aver[i - 1]&&BeatBuffer_aver[i] >= BeatBuffer_aver[i + 2]&&
				BeatBuffer_aver[i] >= BeatBuffer_aver[i - 2]&&BeatBuffer_aver[i] >= BeatBuffer_aver[i + 3]&&BeatBuffer_aver[i] >= BeatBuffer_aver[i - 3]&&
				BeatBuffer_aver[i] >= BeatBuffer_aver[i + 4]&&BeatBuffer_aver[i] >= BeatBuffer_aver[i - 4]&&BeatBuffer_aver[i] >= BeatBuffer_aver[i + 5]&&
				BeatBuffer_aver[i] >= BeatBuffer_aver[i - 5])
			{
				localmaxpoint_3 = i;
				break;
			}
		}
		for (i = referencepoint;i <= 144;i ++)
		{
			if (BeatBuffer_aver[i] <= BeatBuffer_aver[i + 1]&&BeatBuffer_aver[i] <= BeatBuffer_aver[i - 1]&&BeatBuffer_aver[i] <= BeatBuffer_aver[i + 2]&&
				BeatBuffer_aver[i] <= BeatBuffer_aver[i - 2]&&BeatBuffer_aver[i] <= BeatBuffer_aver[i + 3]&&BeatBuffer_aver[i] <= BeatBuffer_aver[i - 3]&&
				BeatBuffer_aver[i] <= BeatBuffer_aver[i + 4]&&BeatBuffer_aver[i] <= BeatBuffer_aver[i - 4]&&BeatBuffer_aver[i] <= BeatBuffer_aver[i + 5]&&
				BeatBuffer_aver[i] <= BeatBuffer_aver[i - 5])
			{
				localminpoint_3 = i;
				break;
			}
		}
		if (localminpoint_3 == -1&&localmaxpoint_3 != -1)
		{
			*peakpoint = localmaxpoint_3;
			return POSITIVE;
		}
		else if (localminpoint_3 != -1&&localmaxpoint_3 == -1)
		{
			*peakpoint = localminpoint_3;
			return NEGATIVE;
		}
		else
		{
			*peakpoint = -1;
			return UNSURE;
		}
	}
	int localmaxpoint = 0;
	int localminpoint = 0;
	int localmaxpoint_2 = 0;
	int localminpoint_2 = 0;
	int i;
	bool b_short = false;


	for (i = referencepoint;i <= endpoint;i ++)
	{
		if (BeatBuffer_aver[i] >= BeatBuffer_aver[i + 1]&&BeatBuffer_aver[i] >= BeatBuffer_aver[i - 1]&&BeatBuffer_aver[i] >= BeatBuffer_aver[i + 2]&&
			BeatBuffer_aver[i] >= BeatBuffer_aver[i - 2]&&BeatBuffer_aver[i] >= BeatBuffer_aver[i + 3]&&BeatBuffer_aver[i] >= BeatBuffer_aver[i - 3]&&
			BeatBuffer_aver[i] >= BeatBuffer_aver[i + 4]&&BeatBuffer_aver[i] >= BeatBuffer_aver[i - 4]&&BeatBuffer_aver[i] > BeatBuffer_aver[i + 5]&&
			BeatBuffer_aver[i] >= BeatBuffer_aver[i - 5]&&BeatBuffer_aver[i] > BeatBuffer_aver[i + 6]&&BeatBuffer_aver[i] >= BeatBuffer_aver[i - 6]&&
			BeatBuffer_aver[i] > BeatBuffer_aver[i + 7]&&BeatBuffer_aver[i] >= BeatBuffer_aver[i - 7]&&BeatBuffer_aver[i] > BeatBuffer_aver[i + 8]&&
			BeatBuffer_aver[i] >= BeatBuffer_aver[i - 8]&&BeatBuffer_aver[i] > BeatBuffer_aver[i + 9]&&BeatBuffer_aver[i] >= BeatBuffer_aver[i - 9]&&
			BeatBuffer_aver[i] >= BeatBuffer_aver[i - 10]&&BeatBuffer_aver[i] > BeatBuffer_aver[i + 10]&&BeatBuffer_aver[i] > BeatBuffer_aver[i + 11]
			&&BeatBuffer_aver[i] > BeatBuffer_aver[i + 12]&&BeatBuffer_aver[i] > BeatBuffer_aver[i + 13]&&BeatBuffer_aver[i] > BeatBuffer_aver[i + 14]&&BeatBuffer_aver[i] > BeatBuffer_aver[i + 15])
		{
			if (i <= referencepoint + 5)
			{
				if (BeatBuffer_aver[i] > BeatBuffer_aver[referencepoint]&&BeatBuffer_aver[i] > BeatBuffer_aver[referencepoint + 1]&&BeatBuffer_aver[i] > BeatBuffer_aver[referencepoint + 2]&&BeatBuffer_aver[i] > BeatBuffer_aver[referencepoint + 3]&&BeatBuffer_aver[i] > BeatBuffer_aver[referencepoint + 4]&&BeatBuffer_aver[i] > BeatBuffer_aver[referencepoint + 5]&&BeatBuffer_aver[i] > BeatBuffer_aver[referencepoint + 6]&&BeatBuffer_aver[i] > BeatBuffer_aver[referencepoint + 7]&&
					BeatBuffer_aver[i] > BeatBuffer_aver[referencepoint + 8]&&BeatBuffer_aver[i] > BeatBuffer_aver[referencepoint + 9]&&BeatBuffer_aver[i] > BeatBuffer_aver[referencepoint + 10])
				{
					localmaxpoint = i;
					break;
				}
				else
					continue;
			}
			else
			{
				localmaxpoint = i;
				break;
			}
		}
	}
	for (i = referencepoint;i <= endpoint;i ++)
	{
		if (BeatBuffer_aver[i] <= BeatBuffer_aver[i + 1]&&BeatBuffer_aver[i] <= BeatBuffer_aver[i - 1]&&BeatBuffer_aver[i] <= BeatBuffer_aver[i + 2]&&
			BeatBuffer_aver[i] <= BeatBuffer_aver[i - 2]&&BeatBuffer_aver[i] < BeatBuffer_aver[i + 3]&&BeatBuffer_aver[i] < BeatBuffer_aver[i - 3]&&
			BeatBuffer_aver[i] < BeatBuffer_aver[i + 4]&&BeatBuffer_aver[i] <= BeatBuffer_aver[i - 4]&&BeatBuffer_aver[i] < BeatBuffer_aver[i + 5]&&
			BeatBuffer_aver[i] <= BeatBuffer_aver[i - 5]&&BeatBuffer_aver[i] < BeatBuffer_aver[i + 6]&&BeatBuffer_aver[i] <= BeatBuffer_aver[i - 6]&&
			BeatBuffer_aver[i] < BeatBuffer_aver[i + 7]&&BeatBuffer_aver[i] <= BeatBuffer_aver[i - 7]&&BeatBuffer_aver[i] < BeatBuffer_aver[i + 8]&&
			BeatBuffer_aver[i] <= BeatBuffer_aver[i - 8]&&BeatBuffer_aver[i] < BeatBuffer_aver[i + 9]&&BeatBuffer_aver[i] <= BeatBuffer_aver[i - 9]&&
			BeatBuffer_aver[i] <= BeatBuffer_aver[i - 10]&&BeatBuffer_aver[i] < BeatBuffer_aver[i + 10]&&BeatBuffer_aver[i] < BeatBuffer_aver[i + 11]
			&&BeatBuffer_aver[i] < BeatBuffer_aver[i + 12]&&BeatBuffer_aver[i] < BeatBuffer_aver[i + 13]&&BeatBuffer_aver[i] < BeatBuffer_aver[i + 14]&&BeatBuffer_aver[i] < BeatBuffer_aver[i + 15])
		{
			if (i <= referencepoint + 5)
			{
				if (BeatBuffer_aver[i] < BeatBuffer_aver[referencepoint]&&BeatBuffer_aver[i] < BeatBuffer_aver[referencepoint + 1]&&BeatBuffer_aver[i] < BeatBuffer_aver[referencepoint + 2]&&BeatBuffer_aver[i] < BeatBuffer_aver[referencepoint + 3]&&BeatBuffer_aver[i] < BeatBuffer_aver[referencepoint + 4]&&BeatBuffer_aver[i] < BeatBuffer_aver[referencepoint + 5]&&BeatBuffer_aver[i] < BeatBuffer_aver[referencepoint + 6]&&BeatBuffer_aver[i] < BeatBuffer_aver[referencepoint + 7]&&
					BeatBuffer_aver[i] < BeatBuffer_aver[referencepoint + 8]&&BeatBuffer_aver[i] < BeatBuffer_aver[referencepoint + 9]&&BeatBuffer_aver[i] < BeatBuffer_aver[referencepoint + 10])
				{
					localminpoint = i;
					break;
				}
				else
					continue;
			}
			else
			{
				localminpoint = i;
				break;
			}
		}
	}
	if (localmaxpoint == 0&&localminpoint == 0)
	{
		int localminpoint_3 = 0;
		int localmaxpoint_3 = 0;
		localmaxpoint = GetMaxPoint(BeatBuffer_f,referencepoint+5,endpoint);
		localminpoint = GetMinPoint(BeatBuffer_f,referencepoint+5,endpoint);
		if (localminpoint == endpoint || localminpoint == referencepoint)
		{
			*peakpoint = localmaxpoint;
			return POSITIVE;
		}
		else if (localmaxpoint == endpoint || localmaxpoint == referencepoint)
		{
			*peakpoint = localminpoint;
			return NEGATIVE;
		}
		else if (localmaxpoint == endpoint && localmaxpoint == referencepoint)
		{
			*peakpoint = -1;
			return UNSURE;
		}

		for (i = localmaxpoint - 3;i <= localmaxpoint + 3;i ++)
		{
			if (BeatBuffer_aver[i] >= BeatBuffer_aver[i + 1]&&BeatBuffer_aver[i] >= BeatBuffer_aver[i - 1]&&BeatBuffer_aver[i] >= BeatBuffer_aver[i + 2]&&
				BeatBuffer_aver[i] >= BeatBuffer_aver[i - 2]&&BeatBuffer_aver[i] >= BeatBuffer_aver[i + 3]&&BeatBuffer_aver[i] >= BeatBuffer_aver[i - 3]&&
				BeatBuffer_aver[i] >= BeatBuffer_aver[i + 4]&&BeatBuffer_aver[i] >= BeatBuffer_aver[i - 4]&&BeatBuffer_aver[i] >= BeatBuffer_aver[i + 5]&&
				BeatBuffer_aver[i] >= BeatBuffer_aver[i - 5])
			{
				localmaxpoint_3 = i;
				break;
			}
		}
		for (i = localminpoint - 3;i <= localminpoint + 3;i ++)
		{
			if (BeatBuffer_aver[i] <= BeatBuffer_aver[i + 1]&&BeatBuffer_aver[i] <= BeatBuffer_aver[i - 1]&&BeatBuffer_aver[i] <= BeatBuffer_aver[i + 2]&&
				BeatBuffer_aver[i] <= BeatBuffer_aver[i - 2]&&BeatBuffer_aver[i] <= BeatBuffer_aver[i + 3]&&BeatBuffer_aver[i] <= BeatBuffer_aver[i - 3]&&
				BeatBuffer_aver[i] <= BeatBuffer_aver[i + 4]&&BeatBuffer_aver[i] <= BeatBuffer_aver[i - 4]&&BeatBuffer_aver[i] <= BeatBuffer_aver[i + 5]&&
				BeatBuffer_aver[i] <= BeatBuffer_aver[i - 5])
			{
				localminpoint_3 = i;
				break;
			}
		}
		if (localminpoint_3 != 0)
		{
			localminpoint = localminpoint_3;
			b_short = true;
		}
		if (localmaxpoint_3 != 0)
		{
			localmaxpoint = localmaxpoint_3;
			b_short = true;
		}

	}
	else
	{
		if (localminpoint)
		{
			for (i = localminpoint + 9;i <= endpoint;i ++)
			{
				if (BeatBuffer_aver[i] <= BeatBuffer_aver[i + 1]&&BeatBuffer_aver[i] <= BeatBuffer_aver[i - 1]&&BeatBuffer_aver[i] <= BeatBuffer_aver[i + 2]&&
					BeatBuffer_aver[i] <= BeatBuffer_aver[i - 2]&&BeatBuffer_aver[i] < BeatBuffer_aver[i + 3]&&BeatBuffer_aver[i] <= BeatBuffer_aver[i - 3]&&
					BeatBuffer_aver[i] < BeatBuffer_aver[i + 4]&&BeatBuffer_aver[i] <= BeatBuffer_aver[i - 4]&&BeatBuffer_aver[i] < BeatBuffer_aver[i + 5]&&
					BeatBuffer_aver[i] <= BeatBuffer_aver[i - 5]&&BeatBuffer_aver[i] < BeatBuffer_aver[i + 6]&&BeatBuffer_aver[i] <= BeatBuffer_aver[i - 6]&&
					BeatBuffer_aver[i] < BeatBuffer_aver[i + 7]&&BeatBuffer_aver[i] <= BeatBuffer_aver[i - 7]&&BeatBuffer_aver[i] < BeatBuffer_aver[i + 8]&&
					BeatBuffer_aver[i] <= BeatBuffer_aver[i - 8]&&BeatBuffer_aver[i] < BeatBuffer_aver[i + 9]&&BeatBuffer_aver[i] < BeatBuffer_aver[i + 10]&&
					BeatBuffer_aver[i] < BeatBuffer_aver[i + 11]&&BeatBuffer_aver[i] < BeatBuffer_aver[i + 12]&&BeatBuffer_aver[i] < BeatBuffer_aver[i + 13])
				{
					localminpoint_2 = i;
					break;
				}
			}
		}
		if (localmaxpoint)
		{
			for (i = localmaxpoint + 9;i <= endpoint;i ++)
			{
				if (BeatBuffer_aver[i] >= BeatBuffer_aver[i + 1]&&BeatBuffer_aver[i] >= BeatBuffer_aver[i - 1]&&BeatBuffer_aver[i] >= BeatBuffer_aver[i + 2]&&
					BeatBuffer_aver[i] >= BeatBuffer_aver[i - 2]&&BeatBuffer_aver[i] >= BeatBuffer_aver[i + 3]&&BeatBuffer_aver[i] >= BeatBuffer_aver[i - 3]&&
					BeatBuffer_aver[i] >= BeatBuffer_aver[i + 4]&&BeatBuffer_aver[i] >= BeatBuffer_aver[i - 4]&&BeatBuffer_aver[i] > BeatBuffer_aver[i + 5]&&
					BeatBuffer_aver[i] >= BeatBuffer_aver[i - 5]&&BeatBuffer_aver[i] > BeatBuffer_aver[i + 6]&&BeatBuffer_aver[i] >= BeatBuffer_aver[i - 6]&&
					BeatBuffer_aver[i] > BeatBuffer_aver[i + 7]&&BeatBuffer_aver[i] >= BeatBuffer_aver[i - 7]&&BeatBuffer_aver[i] > BeatBuffer_aver[i + 8]&&
					BeatBuffer_aver[i] >= BeatBuffer_aver[i - 8]&&BeatBuffer_aver[i] > BeatBuffer_aver[i + 9]&&BeatBuffer_aver[i] > BeatBuffer_aver[i + 10]&&
					BeatBuffer_aver[i] > BeatBuffer_aver[i + 11]&&BeatBuffer_aver[i] > BeatBuffer_aver[i + 12]&&BeatBuffer_aver[i] > BeatBuffer_aver[i + 13])
				{
					localmaxpoint_2 = i;
					break;
				}
			}
		}
		if (localmaxpoint_2 != 0)
		{
			int dis_max_2 = max(BeatBuffer_aver[localmaxpoint_2] - BeatBuffer_aver[localmaxpoint_2 - 10],BeatBuffer_aver[localmaxpoint_2] - BeatBuffer_aver[localmaxpoint_2 + 10]);
			int dis_max = max(BeatBuffer_aver[localmaxpoint] - BeatBuffer_aver[localmaxpoint - 10],BeatBuffer_aver[localmaxpoint] - BeatBuffer_aver[localmaxpoint + 10]);
			if (dis_max_2 >= 2*dis_max)
			{
				localmaxpoint = localmaxpoint_2;
			}
		}
		if (localminpoint_2 != 0)
		{
			int dis_max_2 = max(BeatBuffer_aver[localminpoint_2 - 10] - BeatBuffer_aver[localminpoint_2],BeatBuffer_aver[localminpoint_2 + 10] - BeatBuffer_aver[localminpoint_2]);
			int dis_max = max(BeatBuffer_aver[localminpoint - 10] - BeatBuffer_aver[localminpoint],BeatBuffer_aver[localminpoint + 10] - BeatBuffer_aver[localminpoint]);
			if (dis_max_2 >= 2*dis_max)
			{
				localminpoint = localminpoint_2;
			}
		}
		if (localminpoint == 0)
		{
			*peakpoint = localmaxpoint;
			return POSITIVE;
		}
		else if (localmaxpoint == 0)
		{
			*peakpoint = localminpoint;
			return NEGATIVE;
		}
	}
	if (localminpoint == 0&&localmaxpoint == 0)
	{
		return UNSURE;
	}

	int mindis_maxpoint, maxdis_maxpoint;
	int mindis_minpoint, maxdis_minpoint;
	if (b_short == true)
	{
		int dis = abs(localmaxpoint - localminpoint);
		if (dis > 10)
		{
			dis = 10;
		}
		if (dis < 5)
		{
			dis = 5;
		}
		mindis_maxpoint = min((BeatBuffer_aver[localmaxpoint]-BeatBuffer_aver[localmaxpoint - dis]),(BeatBuffer_aver[localmaxpoint] - BeatBuffer_aver[localmaxpoint + dis]));
		mindis_minpoint = min((-BeatBuffer_aver[localminpoint]+BeatBuffer_aver[localminpoint - dis]),(- BeatBuffer_aver[localminpoint] + BeatBuffer_aver[localminpoint + dis]));
		maxdis_maxpoint = max((BeatBuffer_aver[localmaxpoint]-BeatBuffer_aver[localmaxpoint - dis]),(BeatBuffer_aver[localmaxpoint] - BeatBuffer_aver[localmaxpoint + dis]));
		maxdis_minpoint = max((-BeatBuffer_aver[localminpoint]+BeatBuffer_aver[localminpoint - dis]),(- BeatBuffer_aver[localminpoint] + BeatBuffer_aver[localminpoint + dis]));
	}
	else
	{
		mindis_maxpoint = min((BeatBuffer_aver[localmaxpoint]-BeatBuffer_aver[localmaxpoint - 10]),(BeatBuffer_aver[localmaxpoint] - BeatBuffer_aver[localmaxpoint + 10]));
		mindis_minpoint = min((-BeatBuffer_aver[localminpoint]+BeatBuffer_aver[localminpoint - 10]),(- BeatBuffer_aver[localminpoint] + BeatBuffer_aver[localminpoint + 10]));
		maxdis_maxpoint = max((BeatBuffer_aver[localmaxpoint]-BeatBuffer_aver[localmaxpoint - 10]),(BeatBuffer_aver[localmaxpoint] - BeatBuffer_aver[localmaxpoint + 10]));
		maxdis_minpoint = max((-BeatBuffer_aver[localminpoint]+BeatBuffer_aver[localminpoint - 10]),(- BeatBuffer_aver[localminpoint] + BeatBuffer_aver[localminpoint + 10]));
	}
	if (mindis_maxpoint >= 2.5*mindis_minpoint&&maxdis_maxpoint < 2.5*mindis_maxpoint)
	{
		*peakpoint = localmaxpoint;
		return POSITIVE;
	}
	else if (mindis_minpoint >= 2.5*mindis_maxpoint&&maxdis_minpoint < 2.5*mindis_minpoint)
	{
		*peakpoint = localminpoint;
		return NEGATIVE;
	}
	else
	{
		//////  It's possible that there is no sharp change
		if (maxdis_maxpoint >= 2.5*maxdis_minpoint)
		{
			*peakpoint = localmaxpoint;
			return POSITIVE;
		}
		else if (maxdis_minpoint >= 2.5*maxdis_maxpoint)
		{
			*peakpoint = localminpoint;
			return NEGATIVE;
		}
		else
		{
			if (maxdis_maxpoint >= 2.5*mindis_maxpoint&&maxdis_minpoint >= mindis_minpoint*2.5)
			{
				if (localmaxpoint > localminpoint)
				{
					*peakpoint = localminpoint;
					return NEGATIVE;
				}
				else
				{
					*peakpoint = localmaxpoint;
					return POSITIVE;
				}
			}
			else if (maxdis_maxpoint >= 2.5*mindis_maxpoint)
			{
				*peakpoint = localmaxpoint;
				return POSITIVE;
			}
			else if (maxdis_minpoint >= mindis_minpoint*2.5)
			{
				*peakpoint = localminpoint;
				return NEGATIVE;
			}
			if (localmaxpoint > localminpoint)
			{
				if (Getlocalmin_right(BeatBuffer_f,localmaxpoint,endpoint) == -1 )
				{
					*peakpoint = localminpoint;
					return NEGATIVE;
				}

				if (localminpoint <= endpoint - 30)
				{
					*peakpoint = localmaxpoint;
					return POSITIVE;
				}
				else
				{
					///    in this situation, the driftline is severely sharp
					int startpoint = GetZeropoint_left(BeatBuffer_f,localminpoint - 10,referencepoint);
					int overpoint = GetZeropoint_right(BeatBuffer_f,localmaxpoint + 10,endpoint);
					if (overpoint > endpoint)
					{
						overpoint = endpoint;
					}
					int result = SlopeMethod(BeatBuffer_aver,startpoint,overpoint);
					if (result == 1)
					{
						*peakpoint = localmaxpoint;
						return POSITIVE;
					}
					else
					{
						*peakpoint = localminpoint;
						return NEGATIVE;
					}
				}
			}
			else
			{
				if (Getlocalmax_right(BeatBuffer_f,localminpoint,endpoint) == -1 )
				{
					*peakpoint = localmaxpoint;
					return POSITIVE;
				}
				if (Getlocalmin_right(BeatBuffer_aver,localmaxpoint,endpoint) == localminpoint)
				{
					*peakpoint = localmaxpoint;
					return POSITIVE;
				}
				else
				{
					int startpoint = GetZeropoint_left(BeatBuffer_f,localmaxpoint - 10,referencepoint);
					int overpoint = GetZeropoint_right(BeatBuffer_f,localminpoint + 10,endpoint);
					if (overpoint > endpoint)
					{
						overpoint = endpoint;
					}
					int result = SlopeMethod(BeatBuffer_aver,startpoint,overpoint);
					if (result == 1)
					{
						*peakpoint = localmaxpoint;
						return POSITIVE;
					}
					else
					{
						*peakpoint = localminpoint;
						return NEGATIVE;
					}
				}
			}
		}
	}
	return 2;
}

int SlopeMethod(int *data,int startpoint,int overpoint)


{
	float slope = (float)(data[overpoint] - data[startpoint])/(overpoint - startpoint);
	float c = data[overpoint] - overpoint*slope;
	float b = -1;
	int maxpoint = startpoint;
	int minpoint = startpoint;
	float maxvalue = 0;
	float minvalue = 0;
	int i = startpoint;
	for (;i< overpoint;i++)
	{
		float dist = 1.00;//(slope*i + b*BeatBuffer_aver[i] + c)/sqrt(slope*slope + 1);
		if (dist > maxvalue)
		{
			maxvalue = dist;
			maxpoint = i;
		}
		if (dist < minvalue)
		{
			minvalue = dist;
			minpoint = i;
		}
	}
	if ((maxvalue) > abs(minvalue))
	{
		if (BeatBuffer_aver[maxpoint] < slope*maxpoint + c)
		{
			return NEGATIVE;
		}
		else
			return POSITIVE;
	}
	else
	{
		if (BeatBuffer_aver[minpoint] < slope*minpoint + c)
		{
			return NEGATIVE;
		}
		else
			return POSITIVE;
	}

}



int Find_P_Polarity(int *QRSonset, int *peakpoint)
{

	int endpoint=*QRSonset;
	int i = *QRSonset+4;

	int max_1,min_1,maxpoint,minpoint;   ////  maxpoint2 is the max point on the left of maxpoint, maxpoint3 is the on the right of maxpoint
	int overpoint;
	int max_2 = 0;
	int maxpoint_2 = 0;
	int min_2 = 0;
	int minpoint_2= 0;
	overpoint = *QRSonset;
	min_1 = BeatBuffer_averder[i];
	max_1 = 0;

	maxpoint = 0;

	*peakpoint = 0;

	minpoint = 0;


	///////////////////////////////   4.1 version
	max_1 = BeatBuffer_aver[g_Iso_point];
	min_1 = BeatBuffer_aver[g_Iso_point];
	for (i = endpoint - 26;i < g_Iso_point - 1;i++)  // from left to right, so that we can find the nearest one
	{
		if (BeatBuffer_aver[i] >= max_1&&BeatBuffer_aver[i] >= BeatBuffer_aver[i-1]&&BeatBuffer_aver[i] >= BeatBuffer_aver[i-2]&&BeatBuffer_aver[i] >= BeatBuffer_aver[i-3]&&BeatBuffer_aver[i] > BeatBuffer_aver[i-4]&&BeatBuffer_aver[i] >= BeatBuffer_aver[i+1]&&BeatBuffer_aver[i] >= BeatBuffer_aver[i+2]&&BeatBuffer_aver[i] >= BeatBuffer_aver[i+3]&&BeatBuffer_aver[i] >= BeatBuffer_aver[i+4])
		{
			maxpoint = i;
			max_1 = BeatBuffer_aver[i];
		}
		if (BeatBuffer_aver[i] <= min_1&&BeatBuffer_aver[i] <= BeatBuffer_aver[i-1]&&BeatBuffer_aver[i] <= BeatBuffer_aver[i-2]&&BeatBuffer_aver[i] <= BeatBuffer_aver[i-3]&&BeatBuffer_aver[i] < BeatBuffer_aver[i-4]&&BeatBuffer_aver[i] <= BeatBuffer_aver[i+1]&&BeatBuffer_aver[i] <= BeatBuffer_aver[i+2]&&BeatBuffer_aver[i] <= BeatBuffer_aver[i+3]&&BeatBuffer_aver[i] <= BeatBuffer_aver[i+4])
		{
			minpoint = i;
			min_1 = BeatBuffer_aver[i];
		}
	}
	max_2 = BeatBuffer_aver[g_Iso_point];

	for (i = endpoint - 26;i < g_Iso_point - 1;i++)  // from left to right, so that we can find the nearest one
	{
		if ( i >= maxpoint - 4&& i <= maxpoint + 4)
		{
			continue;
		}
		if (BeatBuffer_aver[i] >= max_2&&BeatBuffer_aver[i] >= BeatBuffer_aver[i-1]&&BeatBuffer_aver[i] >= BeatBuffer_aver[i-2]&&BeatBuffer_aver[i] >= BeatBuffer_aver[i-3]&&BeatBuffer_aver[i] > BeatBuffer_aver[i-4]&&BeatBuffer_aver[i] >= BeatBuffer_aver[i+1]&&BeatBuffer_aver[i] >= BeatBuffer_aver[i+2]&&BeatBuffer_aver[i] >= BeatBuffer_aver[i+3]&&BeatBuffer_aver[i] >= BeatBuffer_aver[i+4])
		{
			maxpoint_2 = i;
			max_2 = BeatBuffer_aver[i];
		}
	}
	if (maxpoint_2 == 0)
	{
		;
	}
	else
	{
		int dist = min(abs(BeatBuffer_aver[maxpoint_2] - BeatBuffer_aver[maxpoint_2 - 4]), abs(BeatBuffer_aver[maxpoint_2] - BeatBuffer_aver[maxpoint_2 + 4]));
		int dist_2 = min(abs(BeatBuffer_aver[maxpoint] - BeatBuffer_aver[maxpoint - 4]),abs(BeatBuffer_aver[maxpoint] - BeatBuffer_aver[maxpoint + 4]));
		if ( dist > dist_2)
		{
			maxpoint = maxpoint_2;
			max_1 = max_2;
		}
	}
	min_2 = BeatBuffer_aver[minpoint_2];
	for (i = endpoint - 26;i < g_Iso_point - 1;i++)
	{
		if ( i >= minpoint - 4&& i <= minpoint + 4)
		{
			continue;
		}
		if (BeatBuffer_aver[i] <= min_2&&BeatBuffer_aver[i] <= BeatBuffer_aver[i-1]&&BeatBuffer_aver[i] <= BeatBuffer_aver[i-2]&&BeatBuffer_aver[i] <= BeatBuffer_aver[i-3]&&BeatBuffer_aver[i] < BeatBuffer_aver[i-4]&&BeatBuffer_aver[i] <= BeatBuffer_aver[i+1]&&BeatBuffer_aver[i] <= BeatBuffer_aver[i+2]&&BeatBuffer_aver[i] <= BeatBuffer_aver[i+3]&&BeatBuffer_aver[i] <= BeatBuffer_aver[i+4])
		{
			minpoint_2 = i;
			min_2 = BeatBuffer_aver[i];
		}
	}
	if (minpoint_2 == 0)
	{
		;
	}
	else
	{
		if (min(abs(BeatBuffer_aver[minpoint_2] - BeatBuffer_aver[minpoint_2 - 4]), abs(BeatBuffer_aver[minpoint_2] - BeatBuffer_aver[minpoint_2 + 4])) > min(abs(BeatBuffer_aver[minpoint] - BeatBuffer_aver[minpoint - 4]),abs(BeatBuffer_aver[minpoint] - BeatBuffer_aver[minpoint + 4])))
		{
			minpoint = minpoint_2;
			min_1 = min_2;
		}
	}

	if ( 4 * abs(BeatBuffer_aver[g_Iso_point] - max_1) < abs(BeatBuffer_aver[g_Iso_point] - min_1))
	{
		/************   to find the minpoint or maxpoint in filtered    ************/
		*peakpoint = GetMinPoint(BeatBuffer_f,minpoint - 2,minpoint + 2);

		/******************          ************/

		return NEGATIVE;    //////////// 0
	}
	if ( 4 * abs(BeatBuffer_aver[g_Iso_point] - min_1) < abs(BeatBuffer_aver[g_Iso_point] - max_1))
	{
		*peakpoint = GetMaxPoint(BeatBuffer_f,maxpoint - 2, maxpoint + 2);
		return POSITIVE;   //// 1
	}

	return UNSURE;
}

bool FlatPCheck(int *data, int startpoint, int length)
{
	int i, max, min ;

	for(i = startpoint, max=min = data[startpoint]; i > startpoint - length; --i)
	{
		if(data[i] > max)
			max = data[i] ;
		else if(data[i] < min)
			min = data[i] ;
	}

	if( abs(max) < ISO_LIMIT_PWAVE&&abs(min) < ISO_LIMIT_PWAVE)
		return(true) ;
	return(false) ;
}

int GetFeatures(int *Pstart, int *Pend, int *QRSonset, int *QRSoffset, int *Tend, int *startbeat,int *endbeat,int *rr )
{
	//to find the P wave polarity
	//////////////////////////////////////////////////////////////////////////
	int flag_P_result = 0;   /////1 indicate that finding P is right.
	int flag_T_result = 0;   /////1 indicate that finding t is right.   whenever both are 1 can return 1;
	int positive_num = 0;
	int negative_num = 0;
	int result = 0;

	int start_search = 13;
	int over_search = 50;
	int referencepoint;
	int i = 0;
	int rough_Pstart = 0;
	int rough_Pend = 0;
	static int aver_QRS = 0;


	if (*QRSonset > 45)  ////  there is P wave
	{
		if (g_QRS_num < 8)
		{
			g_QRS_num++;
		}

		if (g_QRS_num <= 6)
		{
			g_QRSonset_sum = g_QRSonset_sum + *QRSonset;
			aver_QRS = g_QRSonset_sum/g_QRS_num;
		}
		if (g_QRS_num > 6)
		{
			aver_QRS = (aver_QRS*5)/6 + *QRSonset/6;
		}
		if (*QRSonset <= 2*aver_QRS/3)
		{
			*Pend = 0;
			*Pstart = 0;
		}
		else
		{

			referencepoint = CheckTheNearestZero();
			g_Iso_point = referencepoint;

			if (FlatPCheck(BeatBuffer_f,referencepoint,25) == true)
			{
				*Pstart = -1;
				*Pend = -1;
			}
			else
			{
				int peak = 0;
				///     just can find a rough result
				result = Find_P_Polarity(QRSonset, &peak);

				if (result == 2)
				{
					int result;
					int end = *QRSonset;
					int start = g_Iso_point - 45;
					if (start < 13)
					{
						start = 13;
					}
					result = CheckBiphasicWave(QRSonset,&start,&end,3);
					if (result)
					{
						g_pWave_type = 1;  /// biphasic
						*Pend = end;
						*Pstart = start;
						flag_P_result = 1;

					}
					else
					{
						if (end - start >= 15)
						{
							CheckLocalmaxormin(BeatBuffer_f,&start,&end);
						}
						*Pend = end;
						*Pstart = start;
					}
				}
				else if (result == 0)  // negative
				{
					bool b_biphasic_left = false;
					bool b_biphasic_right = false;
					int peak_left,peak_right;
					int zeropoint_minright,zeropoint_minleft,localmax_left,localmax_right,zeropoint_left,zeropoint_right;
					zeropoint_minright = GetZeropoint_right(BeatBuffer_f,peak, g_Iso_point + 1);
					zeropoint_minleft = GetZeropoint_left(BeatBuffer_f,peak,*QRSonset - 35);
					if (zeropoint_minright == -1)
					{
						zeropoint_minright = Getlocalmax_right(BeatBuffer_f,peak,g_Iso_point + 1);
						if (zeropoint_minright == -1)
						{
							*Pend = g_Iso_point;
						}
					}
					if (zeropoint_minleft == -1)
					{
						zeropoint_minleft = Getlocalmin_left(BeatBuffer_f,peak,*QRSonset - 35);
						if (zeropoint_minleft == -1)
						{
							*Pstart = -1;  ///     It's possible that it' too flat
							*Pend = -1;
						}
					}
					else
					{
						localmax_right = Getlocalmax_right(BeatBuffer_f,peak,g_Iso_point + 1);
						localmax_left = Getlocalmax_left(BeatBuffer_f,peak,*QRSonset - 35);
						if (localmax_left == -1||localmax_left == zeropoint_minleft)
						{
							b_biphasic_left = false;
						}
						else
						{
							zeropoint_left = GetZeropoint_left(BeatBuffer_f,localmax_left,*QRSonset - 35);
							if (zeropoint_left == -1)
							{
								b_biphasic_left = false;
							}
							else
							{
								peak_left = Getlocalmax_left(BeatBuffer_aver, zeropoint_minleft, zeropoint_left);
								if (peak_left == -1)
								{
									b_biphasic_left = false;
								}
								else
								{
									if (abs(peak_left - localmax_left) <= 2&&abs(zeropoint_left - zeropoint_minright) <= 20&&(abs(zeropoint_left - peak_left) < zeropoint_minright - zeropoint_minleft)
										&&2*abs(BeatBuffer_aver[peak_left]-BeatBuffer_aver[peak_left-4])>=abs(BeatBuffer_aver[peak_left]-BeatBuffer_aver[peak_left+4]))
									{
										b_biphasic_left = true;
									}
								}
							}
						}
						if (localmax_right == -1||localmax_right == zeropoint_minright)
						{
							b_biphasic_right = false;
						}
						else
						{
							zeropoint_right = GetZeropoint_right(BeatBuffer_f,localmax_right,g_Iso_point + 1);
							if (zeropoint_right == -1)
							{
								;
							}
							else
							{
								peak_right = Getlocalmax_right(BeatBuffer_aver, zeropoint_minright, zeropoint_right);
								if (peak_right == -1)
								{
									;
								}
								else
								{
									if (abs(peak_right - localmax_right) <= 2&&abs(zeropoint_right - zeropoint_minleft) <= 20&&(abs(zeropoint_right - peak_right) < zeropoint_minright - zeropoint_minleft)
										&&2*abs(BeatBuffer_aver[peak_right]-BeatBuffer_aver[peak_right+4])>=abs(BeatBuffer_aver[peak_right]-BeatBuffer_aver[peak_right-4]))
									{
										b_biphasic_right = true;
									}
								}
							}
						}
						if (b_biphasic_left == true&&b_biphasic_right == true)
						{
							if (peak_right > peak_left)
							{
								b_biphasic_left = false;
							}
							else if(peak_right < peak_left)
							{
								b_biphasic_right = false;
							}
							else
							{
								if (BeatBuffer_aver[peak_left] - BeatBuffer_aver[zeropoint_left] > BeatBuffer_aver[peak_right] - BeatBuffer_aver[zeropoint_right])
								{
									b_biphasic_right = false;
								}
								else
								{
									b_biphasic_left = false;
								}
							}
						}
						if (b_biphasic_left == true)
						{
							*Pstart = zeropoint_left;
							*Pend = zeropoint_minright;
						}
						if (b_biphasic_right == true)
						{
							*Pstart = zeropoint_minleft;
							*Pend = zeropoint_right;
						}
						else
						{
							if (localmax_right != -1)
							{
								if ((localmax_right - zeropoint_minright) <= 3&&(localmax_right - zeropoint_minright) > 0)
								{
									*Pend = localmax_right;
								}
								else if (zeropoint_minright - localmax_right <= 3&&(localmax_right - zeropoint_minright) < 0)
								{
									*Pend = zeropoint_minright;
								}
								else
								{
									*Pend = (localmax_right + zeropoint_minright + 1)/2;
								}

							}
							else
							{
								*Pend = zeropoint_minright;
							}
							if (localmax_left != -1)
							{
								if (-(localmax_left - zeropoint_minleft) <= 3&&zeropoint_minleft > localmax_left)
								{
									*Pstart = localmax_left;
								}
								else if (localmax_left - zeropoint_minleft <= 3&&zeropoint_minleft < localmax_left)
								{
									*Pstart = zeropoint_minleft;
								}
								else
								{
									*Pstart = (localmax_left + zeropoint_minleft - 1)/2;
								}
							}
							else
								*Pstart = zeropoint_minleft;
						}
					}
				}
				else if (result == 1)
				{
					bool b_biphasic_left = false;
					bool b_biphasic_right = false;
					int peak_left,peak_right;
					int zeropoint_maxright,zeropoint_maxleft,localmin_left,localmin_right,zeropoint_left,zeropoint_right;
					zeropoint_maxright = GetZeropoint_right(BeatBuffer_f,peak, g_Iso_point + 1);
					zeropoint_maxleft = GetZeropoint_left(BeatBuffer_f,peak,*QRSonset - 35);
					if (zeropoint_maxright == -1)
					{
						zeropoint_maxright = Getlocalmax_right(BeatBuffer_f,peak,g_Iso_point + 1);
						if (zeropoint_maxright == -1)
						{
							*Pend = g_Iso_point;
						}
					}
					if (zeropoint_maxleft == -1)
					{
						zeropoint_maxleft = Getlocalmax_left(BeatBuffer_f,peak,*QRSonset - 35);
						if (zeropoint_maxleft == -1)
						{
							*Pstart = -1;  ///     It's possible that it' too flat
							*Pend = -1;
						}
					}
					localmin_right = Getlocalmin_right(BeatBuffer_f,peak,g_Iso_point + 1);
					localmin_left = Getlocalmin_left(BeatBuffer_f,peak,*QRSonset - 35);
					if (localmin_left == -1||localmin_left == zeropoint_maxleft)
					{
						b_biphasic_left = false;
					}
					else
					{
						zeropoint_left = GetZeropoint_left(BeatBuffer_f,localmin_left,*QRSonset - 35);
						if (zeropoint_left == -1)
						{
							b_biphasic_left = false;
						}
						else
						{
							peak_left = Getlocalmin_left(BeatBuffer_aver, zeropoint_maxleft, zeropoint_left);
							if (peak_left == -1)
							{
								b_biphasic_left = false;
							}
							else
							{
								if (abs(peak_left - localmin_left) <= 2&&abs(zeropoint_left - zeropoint_maxright) <= 20&&(abs(peak_left - zeropoint_left) < zeropoint_maxright - zeropoint_maxleft)
									&&2*abs(BeatBuffer_aver[peak_left] - BeatBuffer_aver[peak_left - 4])>=abs(BeatBuffer_aver[peak_left] - BeatBuffer_aver[peak_left+4]))
								{
									b_biphasic_left = true;
								}
							}
						}
					}
					if (localmin_right == -1||localmin_right == zeropoint_maxright)
					{
						b_biphasic_right = false;
					}
					else
					{
						zeropoint_right = GetZeropoint_right(BeatBuffer_f,localmin_right,g_Iso_point + 1);
						if (zeropoint_right == -1)
						{
							;
						}
						else
						{
							peak_right = Getlocalmax_right(BeatBuffer_aver, zeropoint_maxright, zeropoint_right);
							if (peak_right == -1)
							{
								;
							}
							else
							{
								if (abs(peak_right - localmin_right) <= 2&&abs(zeropoint_right - zeropoint_maxleft) <= 20&&(abs(zeropoint_right - peak_right) < zeropoint_maxright - zeropoint_maxleft)
									&&2*abs(BeatBuffer_aver[peak_right]-BeatBuffer_aver[peak_right+4])>=abs(BeatBuffer_aver[peak_right]-BeatBuffer_aver[peak_right-4]))
								{
									b_biphasic_right = true;
								}
							}
						}
					}
					if (b_biphasic_left == true&&b_biphasic_right == true)
					{
						if (peak_right > peak_left)
						{
							b_biphasic_left = false;
						}
						else if(peak_right < peak_left)
						{
							b_biphasic_right = false;
						}
						else
						{
							if (abs(BeatBuffer_aver[peak_left] - BeatBuffer_aver[zeropoint_left]) > abs(BeatBuffer_aver[peak_right] - BeatBuffer_aver[zeropoint_right]))
							{
								b_biphasic_right = false;
							}
							else
							{
								b_biphasic_left = false;
							}
						}
					}
					if (b_biphasic_left == true)
					{
						*Pstart = zeropoint_left;
						*Pend = zeropoint_maxright;
					}
					else if (b_biphasic_right == true)
					{
						*Pstart = zeropoint_maxleft;
						*Pend = zeropoint_right;
					}
					else
					{
						if (localmin_right != -1)
						{
							if ((localmin_right - zeropoint_maxright) <= 3&&(localmin_right - zeropoint_maxright) > 0)
							{
								*Pend = localmin_right;
							}
							else if (zeropoint_maxright - localmin_right <= 3&&zeropoint_maxright - localmin_right > 0)
							{
								*Pend = zeropoint_maxright;
							}
							else
							{
								*Pend = (localmin_right + zeropoint_maxright + 1)/2;
							}

						}
						else
						{
							*Pend = zeropoint_maxright;
						}
						if (localmin_left != -1)
						{
							if ((zeropoint_maxleft - localmin_left) <= 3&&(zeropoint_maxleft - localmin_left)>0)
							{
								*Pstart = localmin_left;
							}
							else if (localmin_left - zeropoint_maxleft <= 3&&localmin_left - zeropoint_maxleft > 0)
							{
								*Pstart = zeropoint_maxleft;
							}
							else
							{
								*Pstart = (localmin_left + zeropoint_maxleft - 1)/2;
							}
						}
						else
						{
							*Pstart = zeropoint_maxleft;
						}
					}

				}
				if (*Pend - *Pstart >= 15)
				{
					int end = *Pend;
					int start = *Pstart;
					CheckLocalmaxormin(BeatBuffer_f,&start,&end);
					*Pend = end;
					*Pstart = start;
				}

			}
		}

	}
	else
	{
		//////// no p wave
		*Pend = 0;
		*Pstart = 0;
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	int peakpoint;



	int searchend;
	result = Find_T_Polarity( endbeat, QRSoffset, &peakpoint);
	if (peakpoint + 20 > *endbeat)
	{
		searchend = *endbeat;
	}
	else
		searchend = peakpoint + 20;
	if (result == 0)
	{
		*Tend = Getlocalmax_right_T(BeatBuffer_f,peakpoint + 5,searchend);
		if (*Tend == -1)
		{
			if (peakpoint + 30 > *endbeat)
			{
				searchend = *endbeat;
			}
			else
				searchend = peakpoint + 30;
			*Tend = Getlocalmax_right_T(BeatBuffer_f,peakpoint + 5,searchend);
			if (*Tend == -1)
			{
				*Tend = GetZeropoint_right(BeatBuffer_f,peakpoint + 5,searchend);
				if (*Tend == -1)
				{
					*Tend = searchend;
				}
			}
		}
	}
	else if (result == 1)
	{
		*Tend = Getlocalmin_right_T(BeatBuffer_f,peakpoint + 5,searchend);
		if (*Tend == -1)
		{
			if (peakpoint + 30 > *endbeat)
			{
				searchend = *endbeat;
			}
			else
				searchend = peakpoint + 30;
			*Tend = Getlocalmin_right_T(BeatBuffer_f,peakpoint + 5,searchend);
			if (*Tend == -1)
			{
				*Tend = GetZeropoint_right(BeatBuffer_f,peakpoint + 5,searchend);
				if (*Tend == -1)
				{
					*Tend  = searchend;
				}
			}
		}
	}
	else
	{
		*Tend = 0;
	}



	return 2;

}


// added rahul end




//int    Num=0;
//double QRS_Val[15];
//double PR_Val[15];
//double QT_Val[15];


/******************************************************************************
	ResetBDAC() resets static variables required for beat detection and
	classification.
*******************************************************************************/
// added rahul start

JNIEXPORT jint JNICALL Java_com_agatsa_sanket_ECGMonitorActivity_fdatumglobals(JNIEnv *env, jobject obj)
{
	//return fdatumGlobal;
	return hr;
}
JNIEXPORT jint JNICALL Java_com_agatsa_sanket_MainActivity_qPosition(JNIEnv *env, jobject obj)
{
	//return fdatumGlobal;
	return POS_QV;
}
JNIEXPORT jint JNICALL Java_com_agatsa_sanket_MainActivity_tPosition(JNIEnv *env, jobject obj)
{
	//return fdatumGlobal;
	return POS_TV;
}


JNIEXPORT jint JNICALL Java_com_agatsa_sanket_MainActivity_gPDuration(JNIEnv *env, jobject obj)
{
	//return g_P_Duration;
	//return PVal;

	return pos2Value;

}

JNIEXPORT jint JNICALL Java_com_agatsa_sanket_ECGMonitorActivity_gPRIntervals(JNIEnv *env, jobject obj)
{
	//return g_PR_Interval;
	return PRVal;

}
JNIEXPORT jint JNICALL Java_com_agatsa_sanket_MainActivity_sValue(JNIEnv *env,jobject obj){
	return pos3Value;
}
JNIEXPORT jint JNICALL Java_com_agatsa_sanket_ECGMonitorActivity_gQTIntervals(JNIEnv *env, jobject obj)
{
	//return g_QT_Interval;
	return QTVal;

}
JNIEXPORT jint JNICALL Java_com_agatsa_sanket_ECGMonitorActivity_gQTcIntervals(JNIEnv *env, jobject obj)
{
	//return g_QTc_Interval;
	return QTcVal;
}
JNIEXPORT jstring JNICALL Java_com_agatsa_sanket_Utils_gHRSttVs(JNIEnv *env, jobject obj)
{
	//return g_QTc_Interval;
	return "BMDecGzmJv3Xn14WCp6SUV1uNFDbXfoLCFTMBNv0";
}
JNIEXPORT jstring JNICALL Java_com_agatsa_sanket_Utils_gHRSttVk(JNIEnv *env, jobject obj)
{
	//return g_QTc_Interval;
	return "udnuU6aW4DIDbVWO8Um8r3g2tawuLv95WXBRWnCh";
}
JNIEXPORT jint JNICALL Java_com_agatsa_sanket_ECGMonitorActivity_QRSDs(JNIEnv *env, jobject obj)
{
	//return _QRSD;
	return QRSVal;


}

// added rahul end

// added rajiv start

JNIEXPORT jint JNICALL Java_com_agatsa_sanket_MainActivity_gPAvgDuration(JNIEnv *env, jobject obj)
{
	return RRPeakVal;

	//return pos3Value;
}

JNIEXPORT jint JNICALL Java_com_agatsa_sanket_MainActivity_gPRAvgInterval(JNIEnv *env, jobject obj)
{
	return AvgPRVal;
}

JNIEXPORT jint JNICALL Java_com_agatsa_sanket_MainActivity_gQTAvgInterval(JNIEnv *env, jobject obj)
{
	return AvgQTVal;
}

JNIEXPORT jint JNICALL Java_com_agatsa_sanket_MainActivity_gQTcAvgInterval(JNIEnv *env, jobject obj)
{
	return AvgQTcVal;
}

JNIEXPORT jint JNICALL Java_com_agatsa_sanket_MainActivity_AvgQRSD(JNIEnv *env, jobject obj)
{
	return AvgQRSVal;
}

// added rajiv end

//void ResetBDAC(void)
JNIEXPORT jint JNICALL Java_com_agatsa_sanket_MainActivity_getBeatType(JNIEnv *env, jobject obj)
{
//	return GBeatType;
	return pos1Value;
}

/******************************************************************************
	ResetBDAC() resets static variables required for beat detection and
	classification.
*******************************************************************************/

//void ResetBDAC(void)
JNIEXPORT jint JNICALL Java_com_agatsa_sanket_MainActivity_ResetBDAC(JNIEnv *env, jobject obj)
{
	int dummy ;
	QRSDet(0,1) ;	// Reset the qrs detector
	RRCount = 0 ;
	Classify(BeatBuffer,0,0,&dummy,1) ;
	InitBeatFlag = 1 ;
	BeatQueCount = 0 ;	// Flush the beat que.
}

/*****************************************************************************
Syntax:
	int (int ecgSample, int *beatType, *beatMatch) ;
Description:
	BeatDetectAndClassify() implements a beat detector and classifier.
	ECG samples are passed into BeatDetectAndClassify() one sample at a
	time.  BeatDetectAndClassify has been designed for a sample rate of
	200 Hz.  When a beat has been detected and classified the detection
	delay is returned and the beat classification is returned through the
	pointer *beatType.  For use in debugging, the number of the template
   that the beat was matched to is returned in via *beatMatch.
Returns
	BeatDetectAndClassify() returns 0 if no new beat has been detected and
	classified.  If a beat has been classified, BeatDetectAndClassify returns
	the number of samples since the approximate location of the R-wave.
****************************************************************************/
//int BeatDetectAndClassify(int ecgSample, int *beatType, int *beatMatch)
JNIEXPORT jint JNICALL Java_com_agatsa_sanket_ECGMonitorActivity_BeatDetectAndClassifys(JNIEnv *env, jobject obj, jint ecgSample)
{

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

	printf("Ist_Minima %d was found at position :%d\n",Ist_Maxima,pos1);
	printf("IInd_Minima %d was found at position :%d\n",IInd_Maxima,pos2);
	printf("IIIrd_Minima %d was found at position :%d\n",IIIrd_Maxima,pos3);

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


	printf("lowest of two %d and %d\n",lowest,nextLowest);

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

	return RRPeakVal;//QRSVal;//1;
	//return pos4_P2;
}

void DownSampleBeat_2(int *beatOut, int *beatIn, int *beatlength)
{
	int i ;
	int i_num;
	float f_num;


	beatOut[0] = (beatIn[0] + beatIn[1])/2;
	beatOut[BEATLGTH - 1] = (beatIn[*beatlength - 1] + beatIn[*beatlength - 2])/2;
	for(i = 1; i < BEATLGTH - 1; ++i)
	{
		f_num = (float)i*(*beatlength)/150;
		i_num = (int)f_num;
		beatOut[i] = (int)(beatIn[i_num]*(1 - (f_num - i_num))+beatIn[i_num+1]*(f_num - i_num)) ;
		//	tid= omp_get_thread_num();

	}
}


void DownSampleBeat(int *beatOut, int *beatIn)
{
	int i ;

	for(i = 0; i < BEATLGTH; ++i)
		beatOut[i] = (beatIn[i<<1]+beatIn[(i<<1)+1])>>1 ;
}
int least (int a, int b, int c)
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

JNIEXPORT jstring JNICALL
Java_com_agatsa_sanket_App_gHRSttVs(JNIEnv *env, jclass type) {

	// TODO


	return (*env)->NewStringUTF(env, "BMDecGzmJv3Xn14WCp6SUV1uNFDbXfoLCFTMBNv0");
}

JNIEXPORT jstring JNICALL
Java_com_agatsa_sanket_App_gHRSttVk(JNIEnv *env, jclass type) {

	// TODO


	return (*env)->NewStringUTF(env, "udnuU6aW4DIDbVWO8Um8r3g2tawuLv95WXBRWnCh");
}

JNIEXPORT jstring JNICALL
Java_com_agatsa_sanket_FinalReport_gservCmnd(JNIEnv *env, jclass type) {

	// TODO


	return (*env)->NewStringUTF(env, "https://sleepy-sierra-49782.herokuapp.com/api/hello");
	//return (*env)->NewStringUTF(env, "http://192.168.2.8:8000/api/hello");
}

JNIEXPORT jstring JNICALL
Java_com_agatsa_sanket_MainActivity_serEuds(JNIEnv *env, jclass type) {

	// TODO

	//return (*env)->NewStringUTF(env, "0000FFB1-0000-1000-8000-00805F9B34FB");
	return (*env)->NewStringUTF(env, "00002D0D-0000-1000-8000-00805F9B34FB");
}

JNIEXPORT jstring JNICALL
Java_com_agatsa_sanket_MainActivity_charEuds(JNIEnv *env, jclass type) {

	// TODO


	//return (*env)->NewStringUTF(env, "0000FFB2-0000-1000-8000-00805F9B34FB");
	return (*env)->NewStringUTF(env, "00002D37-0000-1000-8000-00805F9B34FB");
}

JNIEXPORT jstring JNICALL
Java_com_agatsa_sanket_MainActivity_desEuds(JNIEnv *env, jclass type) {

	// TODO


	return (*env)->NewStringUTF(env, "00002902-0000-1000-8000-00805f9b34fb");
}

JNIEXPORT jstring JNICALL
Java_com_agatsa_sanket_MainActivity_ser1Euds(JNIEnv *env, jclass type) {

	// TODO


	return (*env)->NewStringUTF(env, "0000180F-0000-1000-8000-00805F9B34FB");
}

JNIEXPORT jstring JNICALL
Java_com_agatsa_sanket_MainActivity_char1Euds(JNIEnv *env, jclass type) {

	// TODO


	return (*env)->NewStringUTF(env, "00002a19-0000-1000-8000-00805f9b34fb");
}

JNIEXPORT jstring JNICALL
Java_com_agatsa_sanket_VerifyPhone_s_1key(JNIEnv *env, jclass type) {

	// TODO


	return (*env)->NewStringUTF(env, "f5f1b11c-e41a-43dd-b472-fc3d349c7c68");
}

JNIEXPORT jstring JNICALL
Java_com_agatsa_sanket_MainActivity_btcharEuds(JNIEnv *env, jclass type) {

	// TODO


	return (*env)->NewStringUTF(env, "00002a19-0000-1000-8000-00805f9b34fb");
}

JNIEXPORT jstring JNICALL
Java_com_agatsa_sanket_PhoneVerification_s_1key(JNIEnv *env, jclass type) {

	// TODO


	return (*env)->NewStringUTF(env,"f5f1b11c-e41a-43dd-b472-fc3d349c7c68");
}

JNIEXPORT jstring JNICALL
Java_com_agatsa_sanket_retrofit_dataset_IOTHub_getPreString(JNIEnv *env, jclass type) {

	// TODO


	return (*env)->NewStringUTF(env, "HostName=agatsaiothub1.azure-devices.net;DeviceId=");
}

JNIEXPORT jstring JNICALL
Java_com_agatsa_sanket_retrofit_dataset_IOTHub_getPostString(JNIEnv *env, jclass type) {

	// TODO


	return (*env)->NewStringUTF(env, "VEGnb9mTBaX1rMeH/ws7cE78ztFo9JufD1VBF3/fYVI=");
}