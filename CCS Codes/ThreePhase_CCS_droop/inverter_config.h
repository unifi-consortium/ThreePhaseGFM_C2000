//###########################################################################
// FILE:    inverter_config.h
// TITLE:   Master Configuration and Shared Variables
//###########################################################################

#ifndef INVERTER_CONFIG_H
#define INVERTER_CONFIG_H

#include "F28x_Project.h"
#include <math.h>
#include "PIR.h"
#include "ADC.h"
#include "ePWM.h"
#include "SFO_v8.h"
#include "DLOG_4CH_F_MVAC.h"
#include "dac_setup.h"

//===========================================================================
// CONTROL ARCHITECTURE SELECTORS
// Uncomment exactly ONE option per category to select the desired logic
//===========================================================================

// --- Controller Gains Tuning ---
//#define GAINS_WORKING
//#define GAINS_REF
//#define GAINS_DHRUV
#define CUSTOM

// --- P and Q Calculation Method ---
//#define PQ_CALC_NO_LPF
#define PQ_CALC_INNER_LOOPS
//#define PQ_CALC_NO_INNER

// --- Outer Voltage Loop Architecture ---
//#define OUTER_LOOP_NONE
#define OUTER_LOOP_CC_FF
// #define OUTER_LOOP_CC

// --- Inner Current Loop (or Single Loop) Architecture ---
//#define INNER_LOOP_CC_FF
#define INNER_LOOP_CC_FF_V0
//#define INNER_LOOP_CC
//#define INNER_LOOP_NONE
//#define INNER_LOOP_SINGLE_LOOP

//===========================================================================
// DEFINES & CONSTANTS
//===========================================================================
#define ONEbySQRT3      0.57735026918963
#define SQRT3           1.732050807568877
#define f_inv           100000.0f
#define Tsamp           (30.0e-6 * 0.999)
#define L               (122.0e-6)
#define C               (7e-6)
#define tds             (380.0e-9)

#define Vnom            (60.0 * 1.41421356237)
#define omega_nom       (60.0 * 2.0 * PI)
#define presync_gain    2.2
#define Vdcnom          300.0


#define Rf              0.059
#define ALPHA           (2.0 * PI / 3.0)

#define BW_cur          (2 * PI * 2000)
#define BW_vol          (2 * PI * 400)
#define BW_LPF          (2 * PI * 10)

#define PWM_CH          3
#define DAC_LOW_VAL     200
#define DAC_HIGH_VAL    3896

// ADC Scaling Multipliers
#define VDC_SCALAR      (415.0 * 3.0 * 0.9769)
#define IDC_SCALAR      (9.899208063354930 * 3.0)
#define I_INV_SCALAR    (23.9980801536 * 3.0)
#define V_CAP_SCALAR    (133.3333333 * 3.0)
#define PARK_GAIN       0.66666667

//===========================================================================
// DATA STRUCTURES
//===========================================================================
typedef struct { float As; float Bs; float Cs; } PH3;
typedef struct { float Ds; float Qs; } PARK;
typedef struct { float Al; float Be; } CLARKE;

//===========================================================================
// GLOBAL VARIABLE DECLARATIONS (Defined in inverter_isr.c)
//===========================================================================
extern float Vnom_meas;
extern float I_max, I_maxSAT, mod_max, P_REF_MAX, Rv;
extern float P_REF, Q_REF, K1, K2;
extern int HALF_TBPRD_inv;
extern int OffsetCalCounter;

extern float Idc, Vdc, P, Q;
extern float Vmod_d, Vmod_q, Vmod_alpha, Vmod_beta;
extern float ID_REF, IQ_REF, VD_REF, VQ_REF;

extern float mP, mQ, V_droop;
extern float omega_droop[2], th_droop[2];
extern float err_Q[2], int_err_Q[2], ki_Q;
extern float err_P[2], int_err_P[2], ki_P;

extern float curr_sens_adj_factor, volt_sens_adj_factor;
extern float angle_I_DQ_REF, mag_I_DQ_REF;
extern float angle_mod_DQ_REF, mag_mod_DQ_REF;

extern float maxGridCurrent, minGridCurrent;
extern float maxInvCurrent, minInvCurrent;

extern volatile uint16_t user_flag;
extern uint16_t prev_flag;

extern bool_t syncflag, FLT_STATUS, READY, RESET, RELAY;

extern PH3 INV_CURRENT, CAP_VOLTAGE, GRID_CURRENT, GRID_VOLTAGE, Vmod;
extern PH3 offset_Iinv, offset_Vcap, offset_Igrid, offset_Vgrid;
extern float offset_Idc;

extern PARK INV_CURRENT_p, CAP_VOLTAGE_p, GRID_CURRENT_p, GRID_VOLTAGE_p;

extern MinMaxLimit MinMax_id, MinMax_iq, MinMax_vd, MinMax_vq;
extern LPF LPF_P, LPF_Q, LPF_Vd, LPF_IgD, LPF_IgQ;
extern Compensator_PI pi_id, pi_iq, pi_vd, pi_vq;
extern SVGEN svgen1;

extern volatile struct EPWM_REGS *ePWM[PWM_CH];

//===========================================================================
// FUNCTION PROTOTYPES
//===========================================================================
void ConfigureSystemPeripherals(void);
void InitializeControlParameters(void);
void CalibrateADCOffsets(void);
interrupt void adca1_isr(void);

#endif // INVERTER_CONFIG_H
