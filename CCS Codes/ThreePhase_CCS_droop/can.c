//###########################################################################
//droop
// FILE:    epwm_deadband_c28.c
//
// TITLE:   Check PWM Dead-Band
//
//! \addtogroup cpu01_example_list
//! <h1> EPWM dead band control (epwm_deadband)</h1>
//!
//! During the test, monitor ePWM1, ePWM2, and/or ePWM3 outputs
//! on a scope.
//!
//! - ePWM1A is on GPIO0 -- Q1
//! - ePWM1B is on GPIO1 -- Q2
//! - ePWM2A is on GPIO2 -- Q3
//! - ePWM2B is on GPIO3 -- Q4
//! - ePWM3A is on GPIO4 -- Q5
//! - ePWM3B is on GPIO5 -- Q6
//!
//
//###########################################################################
// $TI Release: F2837xD Support Library v210 $
// $Release Date: Tue Nov  1 14:46:15 CDT 2016 $
// $Copyright: Copyright (C) 2013-2016 Texas Instruments Incorporated -
//             http://www.ti.com/ ALL RIGHTS RESERVED $
//###########################################################################


//###########################################################################
// REVISION HISTORY (08/08/2023)
//! write the codes for pwm generation at 100 kHz
//! write codes for sensing
//! write codes for desat and the reset

// REVISION HISTORY (08/11/2023)
// I have updated the inner current outer voltage control I have added new PI controllers. The prev code had some mistake between adc
// sensing and the feedback to the correct variable for cap voltage. That has been fixed.
// I have to test this code now for grid-forming. WHat is happening is that the transients during the grid connection is high
// The controller samping frequency is 100/5 = 20 kHz, make sure this is correctly matched as Tsamp.
// FOr the above look at this line in ePWM.c file :EPwm1Regs.ETSOCPS.bit.SOCAPRD2 = 0x5;
// Factors like : curr_sens_adj_factor have been added to ensure zero steady state errors in current and voltage control
// I generally start with Vdcnom = 300 V, but check this, accordingly bunch of stuff needs to be updated.
// COMMENT-UNCOMMENT DAC : use this keyword to find codes for starting the DAC module- I have turned them all off.
// We are using the new design method for kp, ki, kp = BW_cur*L, ki= 0.4*BW_cur*L*BW_cur. The most you can push the factor is to 0.5. use 0.4;
// SW1 decides the status of the syncflag.

// REVISION HISTORY (08/21/2023)
// I am adding the LPF. Also the transients during the grid connection is too high
// I have added codes for circular current and mod index limiter but the grid side inductance needs to be bulked up
// LPF cutoff freq  = 10 Hz

// REVISION HISTORT (08/22/2023)
// I am adding the sw2 for the active power ref ramp. I saw from simulation that only when you inc the P*, Q comes down
//###########################################################################

//
// Included Files
//
#include "F28x_Project.h"
#include <math.h>
#include "PIR.h"
#include "ADC.h"
#include "ePWM.h"
//#include "pwm.h"
#include "SFO_v8.h"
#include "DLOG_4CH_F_MVAC.h"
//
// Defines
//
// for sampling frequency, look up this in epwm.c file:
// EPwm1Regs.ETPS.bit.SOCPSSEL = 1;
// EPwm1Regs.ETSOCPS.bit.SOCAPRD2 = 0x5; ~ this means that we will sample once every 1/20kHz = 50 us
#define ONEbySQRT3 0.57735026918963    /* 1/sqrt(3) */
#define SQRT3 1.732050807568877
#define Tsamp 50*pow(10, -6) // 50 us for the inverter code execution , runs every 100/5 kHz= inverter switching frequency/5
float Vnom = 60.0*1.41421356237;    // Can be tested until 100 V but start with 60 V
#define omega_nom 60.0*2*pi
#define presync_gain 1.1 // Kp = wc/Vg = 2*pi*30/170- PLL design
#define Vdcnom 300
#define L 122*pow(10, -6)
#define C 2*3.5*pow(10,-6) // 2 3u5 caps in parallel
#define Rf 0.059//1.38
#define BW_cur 2*pi*2000 // 8kHz: bandwidth of the current controller
#define BW_vol 2*pi*400 //  1kHz: bandwidth of the voltage controller
#define tds 380*pow(10, -9) // 380
#define ALPHA 2.0*PI/3.0
#define f_inv 100000.0f
float I_max = 15;     //can be changed, increase to avoid voltage loop saturation
float mod_max = 0.9;  //can be changed, increase to avoid current loop saturation
float P_REF_MAX = 500;  //Pref is initialized as o, can be increased to 500 by toggling the second switch from the left
float Rv = 30;
float int_Vgrid_Q[2];
float Vgrid_Q[2];

// COMMENT-UNCOMMENT DAC - Comment next 8 lines out if not using DAC
//#define REFERENCE_VDAC      1
//#define REFERENCE_VREF      1
//#define DACA         1
//#define DACB         2
//#define REFERENCE            REFERENCE_VDAC
//#define DAC_NUM                DACB
//void configureDAC(Uint16 dac_num);
//volatile struct DAC_REGS* DAC_PTR[4] = {0x0,&DacaRegs,&DacbRegs,&DaccRegs};

//#define BUFF_SIZE                160
#define PWM_CH 3
//#define Wp 2*PI*10.0

float P_REF = 0.0;
float Q_REF = 0.0;
float K1 = 0.998,          // Offset filter coefficient K1: 0.05/(T+0.05);
      K2 = 0.001999;       // Offset filter coefficient K2: T/(T+0.05);
//#define MAX_ADC 4096
int HALF_TBPRD_inv = 0;
float Idc = 0.0;
float Vdc = 0.0;
float P;
float Q;
float Vmod_d;
float Vmod_q;
float Vmod_alpha;
float Vmod_beta;
float ID_REF = 1.0;
float IQ_REF = 0.0;
float VD_REF = 50.0;
float VQ_REF = 0.0;
float mP;
float mQ;
float V_droop;
float omega_droop[2];
float th_droop[2] ;
float err_Q[2];
float int_err_Q[2];
float ki_Q = 0.0;//0.01
//float temp_mod = 0.8;
int OffsetCalCounter = 0;
float curr_sens_adj_factor = 1.0;
float volt_sens_adj_factor = 1.0; //1.15;
float angle_I_DQ_REF = 0.0;
float mag_I_DQ_REF = 0.0;
float angle_mod_DQ_REF = 0.0;
float mag_mod_DQ_REF = 0.0;



typedef struct{
    float As;
    float Bs;
    float Cs;
} PH3;


PH3 INV_CURRENT;
PH3 CAP_VOLTAGE;
PH3 GRID_CURRENT;
PH3 GRID_VOLTAGE; // dont need voltage sensors unless I m doing synchronization or voltage control on ac side
PH3 Vmod;
PH3 offset_Iinv;
PH3 offset_Vcap;
PH3 offset_Igrid;
PH3 offset_Vgrid;
float offset_Idc;

MinMaxLimit MinMax_id;
MinMaxLimit MinMax_iq;
MinMaxLimit MinMax_vd;
MinMaxLimit MinMax_vq;

typedef struct{
    float Ds;
    float Qs;
} PARK;

typedef struct{
    float Al;
    float Be;
} CLARKE;

PARK INV_CURRENT_p;
PARK CAP_VOLTAGE_p;
PARK GRID_CURRENT_p;
PARK GRID_VOLTAGE_p;

LPF LPF_P;
LPF LPF_Q;

//CLARKE GRID_VOLTAGE_c;

//float DBUFF_4CH1[200],
//      DBUFF_4CH2[200],
//      DlogCh1,
//      DlogCh2;
//
//// Create an instance of DATALOG Module
//DLOG_4CH_F dlog_4ch1;
Compensator_PI pi_id;
Compensator_PI pi_iq;
Compensator_PI pi_vd;
Compensator_PI pi_vq;

SVGEN svgen1 = SVGEN_DEFAULTS;

//__interrupt void cpu_timer0_isr(void);

bool_t syncflag = false; // if false, keep in presync mode, else go to p transfer mode
bool_t FLT_STATUS = false; // if false means no fault is there, means FLT_STATUS pin is high, should normally be false
bool_t READY = false; // if false means READY pin input is zero, it should normally be true
bool_t RESET = false; // if false means, I am not reseting it, this should normally be false, but the GPIO logic should be inverter since it is active low
bool_t RELAY = false; // if false means, I am not connecting to grid

//
// Array of pointers to EPwm register structures:
// *ePWM[0] is defined as dummy value not used in the example
//
volatile struct EPWM_REGS *ePWM[PWM_CH] =
{  &EPwm1Regs, &EPwm2Regs, &EPwm3Regs};


//
// Main
//
void main(void)
{
// Step 1. Initialize System Control:
// PLL, WatchDog, enable Peripheral Clocks
// This example function is found in the F2837xD_SysCtrl.c file.
//
    InitSysCtrl();

//
// Step 2. Initialize GPIO:
// This example function is found in the F2837xD_Gpio.c file and
// illustrates how to set the GPIO to its default state.
//
    InitGpio();

//
// enable PWM1, PWM2 and PWM3
//
    CpuSysRegs.PCLKCR2.bit.EPWM1=1;
    CpuSysRegs.PCLKCR2.bit.EPWM2=1;
    CpuSysRegs.PCLKCR2.bit.EPWM3=1;
//
// For this case just init GPIO pins for ePWM1, ePWM2, ePWM3
// These functions are in the F2837xD_EPwm.c file
//
    InitEPwm1Gpio();
    InitEPwm2Gpio();
    InitEPwm3Gpio();

//
// Step 3. Clear all interrupts and initialize PIE vector table:
// Disable CPU interrupts
//
    DINT;

//
// Initialize the PIE control registers to their default state.
// The default state is all PIE interrupts disabled and flags
// are cleared.
// This function is found in the F2837xD_PieCtrl.c file.
//
    InitPieCtrl();

//
// Disable CPU interrupts and clear all CPU interrupt flags:
//
    IER = 0x0000;
    IFR = 0x0000;

//
// Initialize the PIE vector table with pointers to the shell Interrupt
// Service Routines (ISR).
// This will populate the entire table, even if the interrupt
// is not used in this example.  This is useful for debug purposes.
// The shell ISR routines are found in F2837xD_DefaultIsr.c.
// This function is found in F2837xD_PieVect.c.
//
    InitPieVectTable();

    //
    // Map ISR functions
    //
//        EALLOW;
//        PieVectTable.ADCA1_INT = &adca1_isr; //function for ADCA interrupt 1
//        EDIS;

        EALLOW;
        PieVectTable.ADCA1_INT = &adca1_isr; //function for ADCA interrupt 1
 //       PieVectTable.ADCA2_INT = &adca2_isr;
//        PieVectTable.TIMER0_INT = &cpu_timer0_isr;
  //      PieVectTable.EPWM11_INT = &epwm11_isr;
        EDIS;
//        CpuTimer0Regs.PRD.all =  500000000;     // A tasks every 1 sec= 100000000
//        CpuTimer0Regs.TCR.bit.TIE = 1;
//        CpuTimer0Regs.TCR.bit.SOFT = 0;  // not needed
//        CpuTimer0Regs.TCR.bit.FREE = 0;  // not needed
//
// Step 4. Initialize the Device Peripherals:
//
//    EALLOW;
//    CpuSysRegs.PCLKCR0.bit.TBCLKSYNC =0;
//    EDIS;

    EALLOW;
    ClkCfgRegs.PERCLKDIVSEL.bit.EPWMCLKDIV = 0;
    EDIS;

    EALLOW;
//    GpioCtrlRegs.GPAMUX2.bit.GPIO25 = 0;   // Keep it as a GPIO
//    GpioCtrlRegs.GPAPUD.bit.GPIO25  = 1;  // 0 = Enables Pull-up, 1= Disables pull-up // DO pull up with input or only output
//    GpioCtrlRegs.GPADIR.bit.GPIO25  = 0;   // 0 = input, 1= output
//    GpioCtrlRegs.GPAMUX2.bit.GPIO18 = 0;   // Keep it as a GPIO
//    GpioCtrlRegs.GPAPUD.bit.GPIO18  = 1;  // 0 = Enables Pull-up, 1= Disables pull-up // DO pull up with input or only output
//    GpioCtrlRegs.GPADIR.bit.GPIO18  = 0;   // 0 = input, 1= output
//    GpioCtrlRegs.GPCGMUX2.bit.GPIO81 = 0;
//    GpioCtrlRegs.GPCPUD.bit.GPIO81 = 0;
//    GpioCtrlRegs.GPCDIR.bit.GPIO81 = 1;
//   GpioCtrlRegs.GPCGMUX2.bit.GPIO86 = 0; // SW6
//   GpioCtrlRegs.GPCPUD.bit.GPIO86 = 1;
//   GpioCtrlRegs.GPCDIR.bit.GPIO86 = 0;

    // Reset pin : output
    //   The Si828x gate driver will shut down when a fault is detected. It then provides FLTb indication to the MCU and remains in the
    //   shutdown state until the MCU applies a reset signal to RSTb. - ACTIVE LOW
       GpioCtrlRegs.GPCGMUX2.bit.GPIO94 = 0; // reset pin
       GpioCtrlRegs.GPCPUD.bit.GPIO94 = 1;
       GpioCtrlRegs.GPCDIR.bit.GPIO94 = 1;

       // /FLT_STATUS pin : input
          GpioCtrlRegs.GPCGMUX2.bit.GPIO90 = 0; // Keep it gpio
          GpioCtrlRegs.GPCPUD.bit.GPIO90 = 1;
          GpioCtrlRegs.GPCDIR.bit.GPIO90 = 0;

      // READY pin : input
         GpioCtrlRegs.GPCGMUX2.bit.GPIO92 = 0; // Keep it gpio
         GpioCtrlRegs.GPCPUD.bit.GPIO92 = 1;
         GpioCtrlRegs.GPCDIR.bit.GPIO92 = 0;

     // This is SW1 and we want to flip it to convert from presync to normal operation
     GpioCtrlRegs.GPCGMUX2.bit.GPIO91 = 0;// SW1
     GpioCtrlRegs.GPCPUD.bit.GPIO91 = 1;
     GpioCtrlRegs.GPCDIR.bit.GPIO91 = 0;

     // Use this for power ramp
     GpioCtrlRegs.GPCGMUX2.bit.GPIO93 = 0;// SW2
     GpioCtrlRegs.GPCPUD.bit.GPIO93 = 1;
     GpioCtrlRegs.GPCDIR.bit.GPIO93 = 0;

    // RELAY to connect the ac side to the output of LCL filter
     GpioCtrlRegs.GPBMUX1.bit.GPIO35 = 0;   // Keep it as a GPIO
     GpioCtrlRegs.GPBPUD.bit.GPIO35 = 1;  // 0 = Enables Pull-up, 1= Disables pull-up // DO pull up with input or only output
     GpioCtrlRegs.GPBDIR.bit.GPIO35  = 1;   // 0 = input, 1= output

     // toggle this to see whether the code fits or not.
     GpioCtrlRegs.GPBMUX1.bit.GPIO36 = 0;   // Keep it as a GPIO
     GpioCtrlRegs.GPBPUD.bit.GPIO36 = 1;  // 0 = Enables Pull-up, 1= Disables pull-up // DO pull up with input or only output
     GpioCtrlRegs.GPBDIR.bit.GPIO36  = 1;   // 0 = input, 1= output

    EDIS;


    // ADC initialization

    InitAdcA();
    InitAdcB();
    InitAdcC();
    InitAdcD();
    // ADC configuration

    config_ADCA_SOC0();
    config_ADCA_SOC1();
    config_ADCA_SOC2();
    config_ADCB_SOC0();
    config_ADCB_SOC1();
    config_ADCB_SOC2();
    config_ADCB_SOC3();
    config_ADCB_SOC4();
    config_ADCC_SOC0();
    config_ADCC_SOC1();
    config_ADCC_SOC2();
    config_ADCD_SOC0();
    config_ADCD_SOC1();
    config_ADCD_SOC2();

// COMMENT-UNCOMMENT DAC - next 2 lines
//    configureDAC(2); //DAC_NUM
//    configureDAC(1); //DAC_NUM

    ConfigureEPWM(); // to sync epwm and adc

    // ePWM initialization

    InitEPwm1(f_inv);
    InitEPwm2(f_inv);
    InitEPwm3(f_inv);

    HALF_TBPRD_inv = (SYSTEM_FREQUENCY*1000000/f_inv)/4; // divby2 for triangular carrier, divby2 for half tbprd

    // All this commented out to save space
    offset_Iinv.As  = 0;         // offset in inverter current - phase A @ 0A
    offset_Iinv.Bs  = 0;
    offset_Iinv.Cs  = 0;
    offset_Vcap.As  = 0;
    offset_Vcap.Bs  = 0;
    offset_Vcap.Cs  = 0;
    offset_Igrid.As  = 0;
    offset_Igrid.Bs  = 0;
    offset_Igrid.Cs  = 0;
    offset_Vgrid.As  = 0;
    offset_Vgrid.Bs  = 0;
    offset_Vgrid.Cs  = 0;
    offset_Idc = 0.0;

    Vmod.As = 0.0;
    Vmod.Bs = 0.0;
    Vmod.Cs = 0.0;

    LPF_P = InitiateLPF( Tsamp, 62.8319);  //  2*PI*10 = 62.8319
    LPF_Q = InitiateLPF( Tsamp, 62.8319); // 10 hz

        V_droop =0.0;
        omega_droop[0] = 0;
        omega_droop[1] = 0;
        th_droop[0] = 0;
        th_droop[1] = 0;

         mP = 2*PI*5/5000;
         mQ = 0.05*Vnom/5000; //originally 0.05

        MinMax_id = initiateMinMax( -0.45*Vdcnom, 0.45*Vdcnom);
        MinMax_iq = initiateMinMax( -0.45*Vdcnom, 0.45*Vdcnom);
        MinMax_vd = initiateMinMax( -I_max, I_max);
        MinMax_vq = initiateMinMax( -I_max, I_max);

       // Compensator_PI InitiateCompensator_PI( float NewTs, float NewKp, float NewKi, float NewKa, MinMaxLimit NewLimits);

//        pi_id = InitiateCompensator_PI(Tsamp, BW_cur*L, BW_cur*Rf, 1, MinMax_id);
//        pi_iq = InitiateCompensator_PI(Tsamp, BW_cur*L, BW_cur*Rf, 1, MinMax_iq);
        pi_id = InitiateCompensator_PI(Tsamp, BW_cur*L, 0.1*BW_cur*L*BW_cur, 1000/(0.1*BW_cur*L*BW_cur), MinMax_id);  //0.4*BW_cur*L*BW_cur
        pi_iq = InitiateCompensator_PI(Tsamp, BW_cur*L, 0.1*BW_cur*L*BW_cur, 1000/(0.1*BW_cur*L*BW_cur), MinMax_iq);  //0.4*BW_cur*L*BW_cur
        pi_vd = InitiateCompensator_PI(Tsamp, BW_vol*C, 0.1*BW_vol*C*BW_vol, 1000/(0.1*BW_vol*C*BW_vol), MinMax_vd);
        pi_vq = InitiateCompensator_PI(Tsamp, BW_vol*C, 0.1*BW_vol*C*BW_vol, 1000/(0.1*BW_vol*C*BW_vol), MinMax_vq);

    // ****************************************************
    // Initialize DATALOG module
    // ****************************************************
//        DLOG_4CH_F_init(&dlog_4ch1);
//        dlog_4ch1.input_ptr1 = &DlogCh1;    //data value
//        dlog_4ch1.input_ptr2 = &DlogCh2;
//        dlog_4ch1.output_ptr1 = &DBUFF_4CH1[0];
//        dlog_4ch1.output_ptr2 = &DBUFF_4CH2[0];
//        dlog_4ch1.size = 200;
//        dlog_4ch1.pre_scalar = 5;
//        dlog_4ch1.trig_value = 0.01;
//        dlog_4ch1.status = 2;


    for (OffsetCalCounter=0; OffsetCalCounter<20000; )
    {
        if(AdcaRegs.ADCINTFLG.bit.ADCINT1 == 1) //   AdcaRegs.ADCINTFLG.bit.ADCINT2 == 1)EPwm11Regs.ETFLG.bit.INT == 1
        {
            if(OffsetCalCounter>1000)
            {
                offset_Iinv.As   = K1*offset_Iinv.As + K2*(IINV_A)*ADC_PU_SCALE_FACTOR;           //Phase A offset
                offset_Iinv.Bs   = K1*offset_Iinv.Bs + K2*(IINV_B)*ADC_PU_SCALE_FACTOR;
                offset_Iinv.Cs   = K1*offset_Iinv.Cs + K2*(IINV_C)*ADC_PU_SCALE_FACTOR;
                offset_Vcap.As   = K1*offset_Vcap.As + K2*(VCAP_A)*ADC_PU_SCALE_FACTOR;           //Phase A offset
                offset_Vcap.Bs   = K1*offset_Vcap.Bs + K2*(VCAP_B)*ADC_PU_SCALE_FACTOR;
                offset_Vcap.Cs   = K1*offset_Vcap.Cs + K2*(VCAP_C)*ADC_PU_SCALE_FACTOR;
                offset_Igrid.As   = K1*offset_Igrid.As + K2*(IGRID_A)*ADC_PU_SCALE_FACTOR;           //Phase A offset
                offset_Igrid.Bs   = K1*offset_Igrid.Bs + K2*(IGRID_B)*ADC_PU_SCALE_FACTOR;
                offset_Igrid.Cs   = K1*offset_Igrid.Cs + K2*(IGRID_C)*ADC_PU_SCALE_FACTOR;
                offset_Vgrid.As   = K1*offset_Vgrid.As + K2*(VGRID_A)*ADC_PU_SCALE_FACTOR;           //Phase A offset
                offset_Vgrid.Bs   = K1*offset_Vgrid.Bs + K2*(VGRID_B)*ADC_PU_SCALE_FACTOR;
                offset_Vgrid.Cs   = K1*offset_Vgrid.Cs + K2*(VGRID_C)*ADC_PU_SCALE_FACTOR;
                offset_Idc        = K1*offset_Idc + K2*(IDC)*ADC_PU_SCALE_FACTOR;

            }
            AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;//AdcaRegs.ADCINTFLGCLR.bit.ADCINT2 = 1;//EPwm11Regs.ETCLR.bit.INT=1;
            OffsetCalCounter++;
        }
    }


    //
    // Enable global Interrupts and higher priority real-time debug events:
    //
    EALLOW;
    CpuSysRegs.PCLKCR0.bit.TBCLKSYNC =1;
    EDIS;
    //
    // enable PIE interrupt
    //


    PieCtrlRegs.PIECTRL.bit.ENPIE = 1;
    PieCtrlRegs.PIEIER1.bit.INTx1 = 1;
    PieCtrlRegs.PIEIER1.bit.INTx7 = 1;
  //  PieCtrlRegs.PIEIER3.bit.INTx11 = 1;
    PieCtrlRegs.PIEIER10.bit.INTx2 = 1;



    IER |= M_INT1; //Enable group 1 interrupts // adc A1 and timer0
 //   IER |= M_INT3; // enable group 3 interrupt epwm 11
    IER |= M_INT10; // enable group 10 interrrupts adcaint2
    EINT;  // Enable Global interrupt INTM
    ERTM;  // Enable Global realtime interrupt DBGM

//
// Step 6. IDLE loop. Just sit and loop forever (optional):
//
do
{
    /* SFO Step Calibration */
    syncflag = GpioDataRegs.GPCDAT.bit.GPIO91; // if zero, then we keep it in the presync state, keep relay off
    FLT_STATUS = (GpioDataRegs.GPCDAT.bit.GPIO90==0?1:0); // if false means no fault is there, means FLT_STATUS pin is high, should normally be false
    READY = GpioDataRegs.GPCDAT.bit.GPIO92; // if false means READY pin input is zero, it should normally be true
    GpioDataRegs.GPCDAT.bit.GPIO94 = (RESET==0?1:0);
//    while ( SFO() == 0 ) {}

}while(1);

}

//
// cpu_timer0_isr - CPU Timer1 ISR
//
//__interrupt void cpu_timer0_isr(void)
//{
//   PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
//   if(CpuTimer0Regs.TCR.bit.TIF == 1)
//      {
//          CpuTimer0Regs.TCR.bit.TIF = 1;  // clear flag
//
//      }
//}

interrupt void adca1_isr(void)
{
    GpioDataRegs.GPBDAT.bit.GPIO36 = 1; // - to check whether code fits or not
    GpioDataRegs.GPBDAT.bit.GPIO35 = (RELAY==1?1:0);
    AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; //clear INT1 flag
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;



     volatile int16 temp;

     Vdc = (temp=VDC)*ADC_PU_SCALE_FACTOR*415*3.0; // 415.3405315614618 = gain = 56/(4*100k+56)  *  8  * 4k3/2k  ; res divider* AMC1100 gain* opamp res gain
     Idc = ((temp=IDC)*ADC_PU_SCALE_FACTOR-offset_Idc)*9.899208063354930*3.0; // gain = 41.67mV/A*(8k/3k3)

     INV_CURRENT.As = ((temp=IINV_A)*ADC_PU_SCALE_FACTOR-offset_Iinv.As)*23.9980801536*3.0*curr_sens_adj_factor;// gain = 41.67mV/A, sign is positive , only sensor gain
     INV_CURRENT.Bs = ((temp=IINV_B)*ADC_PU_SCALE_FACTOR-offset_Iinv.Bs)*23.9980801536*3.0*curr_sens_adj_factor;// gain = 41.67mV/A, sign is positive
     INV_CURRENT.Cs = ((temp=IINV_C)*ADC_PU_SCALE_FACTOR-offset_Iinv.Cs)*23.9980801536*3.0*curr_sens_adj_factor;// gain = 41.67mV/A, sign is positive
     INV_CURRENT_p.Ds =  (INV_CURRENT.As*cos(th_droop[1]) + INV_CURRENT.Bs*cos(th_droop[1]-ALPHA) + INV_CURRENT.Cs*cos(th_droop[1]+ALPHA))*0.66666667;
     INV_CURRENT_p.Qs = (-INV_CURRENT.As*sin(th_droop[1]) - INV_CURRENT.Bs*sin(th_droop[1]-ALPHA) - INV_CURRENT.Cs*sin(th_droop[1]+ALPHA))*0.66666667;

     CAP_VOLTAGE.As = -((temp=VCAP_A)*ADC_PU_SCALE_FACTOR-offset_Vcap.As)*133.3333333*3.0*volt_sens_adj_factor;//gain = R2/R1 = 30e3/4e6
     CAP_VOLTAGE.Bs = -((temp=VCAP_B)*ADC_PU_SCALE_FACTOR-offset_Vcap.Bs)*133.3333333*3.0*volt_sens_adj_factor;//gain = R2/R1 = 30e3/4e6
     CAP_VOLTAGE.Cs = -((temp=VCAP_C)*ADC_PU_SCALE_FACTOR-offset_Vcap.Cs)*133.3333333*3.0*volt_sens_adj_factor;//gain = R2/R1 = 30e3/4e6
     CAP_VOLTAGE_p.Ds = (CAP_VOLTAGE.As*cos(th_droop[1]) + CAP_VOLTAGE.Bs*cos(th_droop[1]-ALPHA) + CAP_VOLTAGE.Cs*cos(th_droop[1]+ALPHA))*0.66666667;
     CAP_VOLTAGE_p.Qs = (-CAP_VOLTAGE.As*sin(th_droop[1]) - CAP_VOLTAGE.Bs*sin(th_droop[1]-ALPHA) - CAP_VOLTAGE.Cs*sin(th_droop[1]+ALPHA))*0.66666667;

     GRID_CURRENT.As = ((temp=IGRID_A)*ADC_PU_SCALE_FACTOR-offset_Igrid.As)*23.9980801536*3.0*curr_sens_adj_factor;// gain = 41.67mV/A, sign is positive
     GRID_CURRENT.Bs = ((temp=IGRID_B)*ADC_PU_SCALE_FACTOR-offset_Igrid.Bs)*23.9980801536*3.0*curr_sens_adj_factor;// gain = 41.67mV/A, sign is positive
     GRID_CURRENT.Cs = ((temp=IGRID_C)*ADC_PU_SCALE_FACTOR-offset_Igrid.Cs)*23.9980801536*3.0*curr_sens_adj_factor;// gain = 41.67mV/A, sign is positive
     GRID_CURRENT_p.Ds =  (GRID_CURRENT.As*cos(th_droop[1]) + GRID_CURRENT.Bs*cos(th_droop[1]-ALPHA) + GRID_CURRENT.Cs*cos(th_droop[1]+ALPHA))*0.66666667;
     GRID_CURRENT_p.Qs = (-GRID_CURRENT.As*sin(th_droop[1]) - GRID_CURRENT.Bs*sin(th_droop[1]-ALPHA) - GRID_CURRENT.Cs*sin(th_droop[1]+ALPHA))*0.66666667;

     GRID_VOLTAGE.As = -((temp=VGRID_A)*ADC_PU_SCALE_FACTOR-offset_Vgrid.As)*133.3333333*3.0*volt_sens_adj_factor;//gain = R2/R1 = 30e3/4e6, sign is positive
     GRID_VOLTAGE.Bs = -((temp=VGRID_B)*ADC_PU_SCALE_FACTOR-offset_Vgrid.Bs)*133.3333333*3.0*volt_sens_adj_factor;//gain = R2/R1 = 30e3/4e6
     GRID_VOLTAGE.Cs = -((temp=VGRID_C)*ADC_PU_SCALE_FACTOR-offset_Vgrid.Cs)*133.3333333*3.0*volt_sens_adj_factor;//gain = R2/R1 = 30e3/4e6
     GRID_VOLTAGE_p.Qs = (-GRID_VOLTAGE.As*sin(th_droop[1]) - GRID_VOLTAGE.Bs*sin(th_droop[1]-ALPHA) - GRID_VOLTAGE.Cs*sin(th_droop[1]+ALPHA))*0.66666667;

//     GRID_VOLTAGE_c.Al =  (0.3333333333)*(2*GRID_VOLTAGE.As-GRID_VOLTAGE.Bs-GRID_VOLTAGE.Cs);
//     GRID_VOLTAGE_c.Be = 0.57735026918963*(GRID_VOLTAGE.Bs-GRID_VOLTAGE.Cs);



         //****************************************************************************************
         //   Inverter Closed Loop Control
         //****************************************************************************************

     // ************* ICOV code begins here

//     th_droop[1] = th_droop[0] + omega_nom*Tsamp;
//      if(th_droop[1]>2*PI)
//      {
//          th_droop[1] = th_droop[1]-2.0*PI;
//
//      }
//      else if(th_droop[1]<0)
//      {
//          th_droop[1] = th_droop[1]+2.0*PI;
//      }
      // ************* ICOV code ends here

//        P = 1.5*(CAP_VOLTAGE_p.Ds*GRID_CURRENT_p.Ds  +  CAP_VOLTAGE_p.Qs*GRID_CURRENT_p.Qs);
//        Q = 1.5*(-CAP_VOLTAGE_p.Ds*GRID_CURRENT_p.Qs  +  CAP_VOLTAGE_p.Qs*GRID_CURRENT_p.Ds);

        P = LPFCalculation( 1.5*(CAP_VOLTAGE_p.Ds*GRID_CURRENT_p.Ds  +  CAP_VOLTAGE_p.Qs*GRID_CURRENT_p.Qs) , &LPF_P);
        Q = LPFCalculation( 1.5*(-CAP_VOLTAGE_p.Ds*GRID_CURRENT_p.Qs  +  CAP_VOLTAGE_p.Qs*GRID_CURRENT_p.Ds) , &LPF_Q);

         if(RELAY==0)//if(RELAY==0)
         {
             V_droop   =  Vnom;
             omega_droop[1] = omega_nom + presync_gain*GRID_VOLTAGE_p.Qs;
             // omega_droop[1] = omega_nom - presync_gain*sin(th_droop[1]- atan2(GRID_VOLTAGE_c.Be, GRID_VOLTAGE_c.Al));
         }
         else//if(RELAY==1)
         {
             err_Q[1] = Q_REF-Q;
             int_err_Q[1] = int_err_Q[0] + 0.5*ki_Q*(err_Q[1] + err_Q[0])*Tsamp;
             V_droop     = Vnom + mQ*err_Q[1];// +int_err_Q[1]; //Vnom - mQ*(Q-Q_REF);
             //V_droop = mySAT(V_droop, 110, 40); // saturate the Vdroop between two limits
             omega_droop[1]  = omega_nom - mP*(P - P_REF); // P_REF = 0
             err_Q[0] = err_Q[1];
             int_err_Q[0] = int_err_Q[1];
             if(GpioDataRegs.GPCDAT.bit.GPIO93 == 1)
             {
                 P_REF = P_REF + 0.005;
                 if(P_REF>P_REF_MAX){P_REF = P_REF_MAX;}
             }
             else
             {
                 P_REF = 50.0;
             }
         }

          th_droop[1] = th_droop[0] + 0.5*(omega_droop[1] + omega_droop[0])*Tsamp;

         if(th_droop[1]>2*PI)
         {
             th_droop[1] = th_droop[1]-2.0*PI;

         }
         else if(th_droop[1]<0)
         {
             th_droop[1] = th_droop[1]+2.0*PI;
         }

         VD_REF = V_droop;
         VQ_REF = 0.0;


/* Note for next steps:
Droop stage generates V_droop and th_droop. After this stage PWM signals (Vmod_d and Vmod_q) can be generated two ways:
         1. Single loop: These V_droop and th_droop will be used to generate Vmod_d and Vmod_q
         2. Multi-loop: VD_REF and VQ_REF (i.e. V_droop) will go to outer voltage control loop
             a) outer voltage control loop: will generate ID_REF and IQ_REF from VD_REF and VQ_REF
             b) inner current control loop: will generate Vmod_d  and Vmod_q from ID_REF and IQ_REF
*/

//-----------------------Commented for Single-loop-----------------------------------------------//
                  //IF Current control mode, commented out

         //**********Multi-loop:Outer Voltage Control Loop************//
         //**********will generate ID_REF and IQ_REF******//

//no cross-coupling and feedfwd terms
        //ID_REF     = CompensatorCalculation_PI(CAP_VOLTAGE_p.Ds, VD_REF, &pi_vd, 0.0);
       // IQ_REF     = CompensatorCalculation_PI(CAP_VOLTAGE_p.Qs, VQ_REF, &pi_vq, 0.0);

//both cross-coupling and feedfwd terms
       //  ID_REF     = CompensatorCalculation_PI(CAP_VOLTAGE_p.Ds, VD_REF, &pi_vd, -omega_nom*C*CAP_VOLTAGE_p.Qs+ GRID_CURRENT_p.Ds);
       // IQ_REF      = CompensatorCalculation_PI(CAP_VOLTAGE_p.Qs, VQ_REF, &pi_vq, omega_nom*C*CAP_VOLTAGE_p.Ds + GRID_CURRENT_p.Qs);


//only cross-coupling terms
       //PI controller
         ID_REF     = CompensatorCalculation_PI(CAP_VOLTAGE_p.Ds, VD_REF, &pi_vd, -omega_nom*C*CAP_VOLTAGE_p.Qs);
         IQ_REF     = CompensatorCalculation_PI(CAP_VOLTAGE_p.Qs, VQ_REF, &pi_vq, omega_nom*C*CAP_VOLTAGE_p.Ds );



//-----------------------
         mag_I_DQ_REF = sqrt(ID_REF*ID_REF + IQ_REF*IQ_REF);
         angle_I_DQ_REF = atan2(IQ_REF,ID_REF);

         //voltage-loop anti-windup
         if(mag_I_DQ_REF>I_max)
         {
             ID_REF =  I_max*cos(angle_I_DQ_REF);
             IQ_REF =  I_max*sin(angle_I_DQ_REF);

 //--Commented for Single-loop-----------------------------------------------//




             // *******************CLOSED  LOOP - SINE TRIANGLE ONLY BEGINS*********************


 //Calculation of Vmod_dq (can be done using multi-loop (A,B,C,D) or single loop (E,F)-directly from droop)

         //-------------Multi-loop: Inner current control loop-----------------------------------------------//
         //-----------------will generate Vmod_d  and Vmod_q---------------------//


//Vdcnom and Vdc both used. Theoretically it should be Vdc

         //A. both cross-coupling and feedfwd terms
// Vmod_d     = (CompensatorCalculation_PI(INV_CURRENT_p.Ds, ID_REF, &pi_id, -omega_nom*L*INV_CURRENT_p.Qs + CAP_VOLTAGE_p.Ds) )/(0.5*Vdc);
// Vmod_q     = (CompensatorCalculation_PI(INV_CURRENT_p.Qs, IQ_REF, &pi_iq, omega_nom*L*INV_CURRENT_p.Ds + CAP_VOLTAGE_p.Qs) )/(0.5*Vdc);


         //B. only cross-coupling terms
// Vmod_d     = (CompensatorCalculation_PI(INV_CURRENT_p.Ds, ID_REF, &pi_id, -omega_nom*L*INV_CURRENT_p.Qs) )/(0.5*Vdc);
// Vmod_q     = (CompensatorCalculation_PI(INV_CURRENT_p.Qs, IQ_REF, &pi_iq, omega_nom*L*INV_CURRENT_p.Ds) )/(0.5*Vdc);


         //C. only cross-coupling terms
 //Vmod_d     = (CompensatorCalculation_PI(INV_CURRENT_p.Ds, ID_REF, &pi_id, -omega_nom*L*INV_CURRENT_p.Qs) )/(0.5*Vdcnom);
 //Vmod_q     = (CompensatorCalculation_PI(INV_CURRENT_p.Qs, IQ_REF, &pi_iq, omega_nom*L*INV_CURRENT_p.Ds) )/(0.5*Vdcnom);


         //D. no cross-coupling and feedfwd terms
 Vmod_d     = (CompensatorCalculation_PI(INV_CURRENT_p.Ds, ID_REF, &pi_id, 0.0 ) )/(0.5*Vdc);
 Vmod_q     = (CompensatorCalculation_PI(INV_CURRENT_p.Qs, IQ_REF, &pi_iq, 0.0 ) )/(0.5*Vdc);




//-------------------------------Single-loop -----------------------------------------------//
 //*************************will directly generate Vmod_d  and Vmod_q from V_droop************//

 //next two are single loop. both are same. Be careful to check VD_REF is commented. V_droop is safe to use

  //E. single loop, use this or next, both same
        // Vmod_d     = (V_droop)/(0.5*Vdcnom);
        // Vmod_q     = 0.0 ;
 //F.
      // Vmod_d     = (V_droop)/(0.5*Vdc);
      // Vmod_q     = 0.0 ;
//------------------------------------------------------------------------------//





//this is for current loop anti-windup
             mag_mod_DQ_REF = sqrt(Vmod_d*Vmod_d + Vmod_q*Vmod_q);
             angle_mod_DQ_REF = atan2(Vmod_q,Vmod_d);

             if(mag_mod_DQ_REF>mod_max)
             {
                 Vmod_d =  mod_max*cos(angle_mod_DQ_REF);
                 Vmod_q =  mod_max*sin(angle_mod_DQ_REF);
             }


             //Vmod_dq to alpha, beta conversion
             Vmod_alpha = Vmod_d*cos(th_droop[1])-Vmod_q*sin(th_droop[1]);
             Vmod_beta  = Vmod_q*cos(th_droop[1])+Vmod_d*sin(th_droop[1]);

             // Vmod_alpha,beta to abc conversion
             Vmod.As = Vmod_alpha;
             Vmod.Bs = 0.5*(-Vmod_alpha + SQRT3*Vmod_beta);
             Vmod.Cs = 0.5*(-Vmod_alpha - SQRT3*Vmod_beta);
             // *******************CLOSED  LOOP - SINE TRIANGLE ONLY ENDS*********************




             // *******************CLOSED  LOOP - SVGEN ONLY BEGINS*********************
//             Vmod_d     = (pi_id.Out_sat - omega_nom*L*INV_CURRENT_p.Qs + CAP_VOLTAGE_p.Ds)*sqrt(3.0)/Vdc;
//             Vmod_q     = (pi_iq.Out_sat + omega_nom*L*INV_CURRENT_p.Ds + CAP_VOLTAGE_p.Qs)*sqrt(3.0)/Vdc;
//             Vmod_alpha = Vmod_d*cos(th_droop[1])-Vmod_q*sin(th_droop[1]);
//             Vmod_beta  = Vmod_q*cos(th_droop[1])+Vmod_d*sin(th_droop[1]);
//             svgen1.Ualpha = Vmod_alpha;
//             svgen1.Ubeta  = Vmod_beta;
//             SVGENDQ_MACRO(svgen1)
             // *******************CLOSED  LOOP - SVGEN ONLY ENDS*********************

           // *******************OPEN LOOP - SVGEN ONLY BEGINS*********************
//             Vmod_d     = temp_mod;
//             Vmod_q     = 0.0;
//             Vmod_alpha = Vmod_d*cos(th_droop[1])-Vmod_q*sin(th_droop[1]);
//             Vmod_beta  = Vmod_q*cos(th_droop[1])+Vmod_d*sin(th_droop[1]);
//             svgen1.Ualpha = Vmod_alpha;
//             svgen1.Ubeta  = Vmod_beta;
//             SVGENDQ_MACRO(svgen1)
             // *******************OPEN LOOP - SVGEN ONLY ENDS*********************

             if(READY==1 && FLT_STATUS==0)
             {
                 EPwm1Regs.CMPA.bit.CMPA = (HALF_TBPRD_inv*Vmod.As)+HALF_TBPRD_inv;
                 EPwm2Regs.CMPA.bit.CMPA = (HALF_TBPRD_inv*Vmod.Bs)+HALF_TBPRD_inv;
                 EPwm3Regs.CMPA.bit.CMPA = (HALF_TBPRD_inv*Vmod.Cs)+HALF_TBPRD_inv;
                 // *******************OPEN/CLOSED  LOOP - SVGEN ONLY BEGINS*********************
//                 Vmod.As = svgen1.Ta;
//                 Vmod.Bs = svgen1.Tb;
//                 Vmod.Cs = svgen1.Tc;
                 // *******************OPEN/CLOSED  LOOP - SVGEN ONLY ENDS*********************


                 // *******************OPEN LOOP - SINE TRIANGLE ONLY BEGINS*********************
//                 Vmod.As = temp_mod*cos(th_droop[1]);
//                 Vmod.Bs = temp_mod*cos(th_droop[1]-ALPHA);
//                 Vmod.Cs = temp_mod*cos(th_droop[1]+ALPHA);
                 // *******************OPEN LOOP - SINE TRIANGLE ONLY ENDS*********************

                 // *******************CLOSED LOOP - SINE TRIANGLE ONLY BEGINS*********************

                 // *******************CLOSED LOOP - SINE TRIANGLE ONLY ENDS*********************
             }
             else
             {
                 EPwm1Regs.CMPA.bit.CMPA = 0;
                 EPwm2Regs.CMPA.bit.CMPA = 0;
                 EPwm3Regs.CMPA.bit.CMPA = 0;
             }


           th_droop[0]=th_droop[1];
           omega_droop[0]=omega_droop[1];
         //  GpioDataRegs.GPBDAT.bit.GPIO36 = 0;// -  to check whether code fits or not
return;
}


// COMMENT-UNCOMMENT DAC
//void configureDAC(Uint16 dac_num)
//{
//    EALLOW;
//    DAC_PTR[dac_num]->DACCTL.bit.DACREFSEL = 1; //REFERENCE
//    DAC_PTR[dac_num]->DACOUTEN.bit.DACOUTEN = 1;
//    DAC_PTR[dac_num]->DACVALS.all = 0;
//    DELAY_US(10); // Delay for buffered DAC to power up
//    EDIS;
//}

