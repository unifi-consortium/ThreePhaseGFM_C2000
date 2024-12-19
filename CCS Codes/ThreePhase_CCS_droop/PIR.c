#ifndef __PIR_C_
#define __PIR_C_


#include <stdio.h>
#include <math.h>

#ifndef pi
#define pi 3.141592653589f
#endif

typedef  struct MinMaxLimit{
    float MinValue;
    float MaxValue;
} MinMaxLimit;


typedef  struct MovingAverage{
    float filterin[4];
    float filterout;
} MovingAverage;

typedef  struct LPF{
    float Ts;
    float wp;
    float x[2];
    float y[2];

    float a;
    float b;

} LPF;

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
}Compensator_PI;

Compensator_PI InitiateCompensator_PI( float NewTs, float NewKp, float NewKi, float NewKa, MinMaxLimit NewLimits)
{
    Compensator_PI NewComp;

    NewComp.Ts1 = NewTs;
    NewComp.Kp1 = NewKp;
    NewComp.Ki1 = NewKi;
    NewComp.Ka1 = NewKa;
    NewComp.e1[0] = 0;
    NewComp.e1[1] = 0;
    NewComp.yp1 = 0;
    NewComp.yi1[0] = 0;
    NewComp.yi1[1] = 0;
    NewComp.y1 = 0;
    NewComp.xa1 = 0;
    NewComp.limits1.MaxValue = NewLimits.MaxValue;
    NewComp.limits1.MinValue = NewLimits.MinValue;
    NewComp.ai1 = NewKi*NewTs*0.5;
    return NewComp;
 }

LPF InitiateLPF( float NewTs, float NewWp){

    LPF NewLPF;

    NewLPF.Ts = NewTs;
    NewLPF.wp = NewWp;
    NewLPF.x[0] = 0;
    NewLPF.x[1] = 0;
    NewLPF.y[0] = 0;
    NewLPF.y[1] = 0;

    NewLPF.a = -(1 - 2/(NewTs*NewWp))/(1 + 2/(NewTs*NewWp));
    NewLPF.b = 1/(1 + 2/(NewTs*NewWp));

    return NewLPF;
 }

float CompensatorCalculation_PI(float measurement, float reference, Compensator_PI *Comp, float feedforward)
{
    int ki = 1;

    Comp->e1[ki] = reference - measurement - Comp->xa1;

    Comp->yp1 = Comp->Kp1*(reference - measurement);

   // Comp->yp1 = Comp->Kp1*Comp->e1[ki];

    Comp->yi1[ki] = Comp->ai1*(Comp->e1[ki] + Comp->e1[ki-1]) + Comp->yi1[ki-1];

    Comp->y1 = (Comp->yp1 + Comp->yi1[ki] + feedforward);

    if(Comp->y1 > Comp->limits1.MaxValue){

         Comp->xa1 = Comp->Ka1*(Comp->y1 - Comp->limits1.MaxValue);
         Comp->y1 = Comp->limits1.MaxValue;

     }

     else if(Comp->y1 <  Comp->limits1.MinValue){

         Comp->xa1 = Comp->Ka1*(Comp->y1 - Comp->limits1.MinValue);
         Comp->y1 = Comp->limits1.MinValue;

     }
     else{

         Comp->xa1 = 0;

     }


    Comp->e1[ki-1] = Comp->e1[ki];
    Comp->yi1[ki-1] = Comp->yi1[ki];

    return Comp->y1;

}

float LPFCalculation(float measurement, LPF *Filter){

    int k = 1;


    Filter->x[k] = measurement;


    Filter->y[k] = Filter->a*Filter->y[k-1] + Filter->b*(Filter->x[k] + Filter->x[k-1]);


    Filter->y[k-1] = Filter->y[k];
    Filter->x[k-1] = Filter->x[k];

    return (Filter->y[k]);

}





void InitiateMoving(MovingAverage *filter){

    int k = 3;

    filter->filterin[k] = 0;
    filter->filterin[k-1] = 0;
    filter->filterin[k-2] = 0;
    filter->filterin[k-3] = 0;

}



float moving( MovingAverage *filter, float newvalue){

    int k = 3;

    filter->filterin[k] = newvalue;

    filter->filterout = (filter->filterin[k] + filter->filterin[k-1] + filter->filterin[k-2] + filter->filterin[k-3])/4;

    filter->filterin[k-3] = filter->filterin[k-2];
    filter->filterin[k-2] = filter->filterin[k-1];
    filter->filterin[k-1] = filter->filterin[k];

    return filter->filterout;
}



MinMaxLimit initiateMinMax( float min, float max){

    MinMaxLimit temp;

    temp.MinValue = min;
    temp.MaxValue = max;

    return temp;
}

void buff_store(float *buffer, float new_value, int k){

    int i = 0;

    for(i = 0; i<k-1; i++){

        buffer[i] = buffer[i+1];

    }

    buffer[i] = new_value;

}


#endif

