/*
 * PIR.h
 *
 *  Created on: Aug 8, 2023
 *      Author: Rahul
 */

#ifndef PIR_H_
#define PIR_H_


#include <stdio.h>
#include <math.h>

#ifndef pi
#define pi 3.141592653589f
#endif

#define mySAT(A,Pos,Neg) (fmaxf(((fminf((A),(Pos)))),(Neg)))

typedef  struct MinMaxLimit{
    float MinValue;
    float MaxValue;
} MinMaxLimit;


typedef  struct Compensator_PI{
    float Ts1;
    float Kp1;
    float Ki1;
    float Ka1;
    float e1[2];
    float yp1;
    float yi1[2];
    float y1;
    float xa1;
    MinMaxLimit limits1;
    float ai1;
    float ar1;
    float br11;
    float br21;
}Compensator_PI;

//typedef struct {  double  Ref;              // Input: reference set-point
//                  double  Fbk;              // Input: feedback
//                  double  Out;              // Output: controller output
//                  double  Kp;               // Parameter: proportional loop gain
//                  double  Ki;               // Parameter: integral gain
//                  double  Umax;         // Parameter: upper saturation limit
//                  double  Umin;         // Parameter: lower saturation limit
//                  double yp_out;
//                  double Out_sat;
//                  double yi_out;
//                  double yi_out_prev;
//                  double err;
//                  double err_prev;
//                  double Tsamp_PI;
//                  double Awndp;
//                } PI_CONTROLLER;
//
//#define PI_CONTROLLER_DEFAULTS {    0.0,    \
//                                      0.0,          \
//                                0.0,            \
//                                      1.0,          \
//                           1.0, \
//                           1.0, \
//                           -1.0,    \
//                           0.0,     \
//                           0.0, \
//                           0.0,     \
//                           0.0, \
//                           0.0, \
//                           0.0 ,    \
//                           0.0,\
//                           0.0\
//                          }

        typedef struct  { double  Ualpha;           // Input: reference alpha-axis phase voltage
                          double  Ubeta;            // Input: reference beta-axis phase voltage
                          double  Ta;               // Output: reference phase-a switching function
                          double  Tb;               // Output: reference phase-b switching function
                          double  Tc;               // Output: reference phase-c switching function
                          double  tmp1;         // Variable: temp variable
                          double  tmp2;         // Variable: temp variable
                          double  tmp3;         // Variable: temp variable
                          int VecSector;        // Space vector sector
                        } SVGEN;

        #define SVGEN_DEFAULTS { 0,0,0,0,0 }

//#define PI_MACRO(v)                                             \
//                                                                \
//    v.err = v.Ref - v.Fbk;                      \
//    v.yp_out = v.Kp*v.err; \
//    v.yi_out = (v.Out == v.Out_sat)? (v.yi_out_prev +0.5*v.Ki*v.Tsamp_PI*(v.err+v.err_prev)):v.yi_out_prev; \
//    v.Out = v.yp_out + v.yi_out;\
//    v.Out_sat = mySAT(v.Out, v.Umax, v.Umin); \
//    v.err_prev = v.err;\
//    v.yi_out_prev = v.yi_out;\


#define SVGENDQ_MACRO(v)                                                        \
    v.tmp1= v.Ubeta;                                                            \
    v.tmp2= v.Ubeta/2.0 + 0.866*v.Ualpha;                   \
    v.tmp3= v.tmp2 - v.tmp1;                                                    \
                                                                                \
    v.VecSector=3;                                                              \
    v.VecSector=(v.tmp2> 0)?( v.VecSector-1):v.VecSector;                       \
    v.VecSector=(v.tmp3> 0)?( v.VecSector-1):v.VecSector;                       \
    v.VecSector=(v.tmp1< 0)?(7-v.VecSector) :v.VecSector;                       \
                                                                                \
    if     (v.VecSector==1 || v.VecSector==4)                                   \
      {     v.Ta= v.tmp2;                                                       \
            v.Tb= v.tmp1-v.tmp3;                                                \
            v.Tc=-v.tmp2;                                                       \
      }                                                                         \
                                                                                \
    else if(v.VecSector==2 || v.VecSector==5)                                   \
      {     v.Ta= v.tmp3+v.tmp2;                                                \
            v.Tb= v.tmp1;                                                       \
            v.Tc=-v.tmp1;                                                       \
      }                                                                         \
                                                                                \
    else                                                                        \
      {     v.Ta= v.tmp3;                                                       \
            v.Tb=-v.tmp3;                                                       \
            v.Tc=-(v.tmp1+v.tmp2);                                              \
      }                                                                         \
                                                                                \


typedef  struct LPF{
    float Ts;
    float wp;
    float x[2];
    float y[2];

    float a;
    float b;

} LPF;

typedef  struct LPF_RM{
    float x[2];
    float y[2];
} LPF_RM;




typedef  struct MovingAverage{
    float filterin[4];
    float filterout;
} MovingAverage;

LPF InitiateLPF( float NewTs, float NewWp);

float LPFCalculation(float measurement, LPF *Filter);

Compensator_PI InitiateCompensator_PI( float NewTs, float NewKp, float NewKi, float NewKa, MinMaxLimit NewLimits);

MinMaxLimit initiateMinMax( float min, float max);

void InitiateMoving(MovingAverage *filter);

float moving( MovingAverage *filter, float newvalue);

void buff_store(float *buffer, float new_value, int k);

float CompensatorCalculation_PI(float measurement, float reference, Compensator_PI *Comp, float feedforward);


#endif /* PIR_H_ */
