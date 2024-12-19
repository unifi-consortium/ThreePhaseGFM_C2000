/*
 * ADC.h
 *
 *  Created on: Aug 8, 2023
 *      Author: Rahul
 */

#ifndef ePWM_H_
#define ePWM_H_

#include "F28x_Project.h"
#include <math.h>
#include "PIR.h"

#define TBCOUNT 100000000

void InitEPwm1(float freq);
void InitEPwm2(float freq);
void InitEPwm3(float freq);
void InitEPwm4(float freq);
void InitEPwm5(float freq);
void InitEPwm6(float freq);
void InitEPwm7(float freq);
void InitEPwm11(float freq);

void HRCompareP(float fCompareACounts, float fCompareBCounts);
void HRCompareS1(float fCompareACounts, float fCompareBCounts);
void HRCompareS2(float fCompareACounts, float fCompareBCounts);
void HRCompareS3(float fCompareACounts, float fCompareBCounts);
void HRPhaseS1(float fPhaseCounts);
void HRPhaseS2(float fPhaseCounts);
void HRPhaseS3(float fPhaseCounts);

#endif /* ADC_H_ */
