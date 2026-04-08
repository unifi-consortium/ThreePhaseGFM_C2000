//###########################################################################
// FILE:    inverter_isr.c
// TITLE:   High-Speed Interrupt Service Routine (Control Loop)
//###########################################################################

#include "inverter_config.h"

//===========================================================================
// GLOBAL VARIABLE DEFINITIONS
//===========================================================================
float Vnom_meas;
float Igrid_filt_D, Igrid_filt_Q;

float I_max = 15.0, I_maxSAT = 15.0, mod_max = 0.9, P_REF_MAX = 100.0, Rv = 30.0;
float P_REF = 0.0, Q_REF = 0.0, K1 = 0.998, K2 = 0.001999;
int HALF_TBPRD_inv = 0, OffsetCalCounter = 0;

float Idc = 0.0, Vdc = 0.0, P = 0.0, Q = 0.0;
float Vmod_d = 0.0, Vmod_q = 0.0, Vmod_alpha = 0.0, Vmod_beta = 0.0;
float ID_REF = 1.0, IQ_REF = 0.0, VD_REF = 50.0, VQ_REF = 0.0;

float mP, mQ, V_droop;
float omega_droop[2] = {0}, th_droop[2] = {0};
float err_Q[2] = {0}, int_err_Q[2] = {0}, ki_Q = 0.001;
float err_P[2] = {0}, int_err_P[2] = {0}, ki_P = 0.001;

float curr_sens_adj_factor = 1.0, volt_sens_adj_factor = 1.0;
float angle_I_DQ_REF = 0.0, mag_I_DQ_REF = 0.0;
float angle_mod_DQ_REF = 0.0, mag_mod_DQ_REF = 0.0;

float maxGridCurrent = 0.0, minGridCurrent = 0.0;
float maxInvCurrent = 0.0, minInvCurrent = 0.0;

volatile uint16_t user_flag = 0;
uint16_t prev_flag = 0;

bool_t syncflag = false, FLT_STATUS = false, READY = false, RESET = false, RELAY = false;

PH3 INV_CURRENT, CAP_VOLTAGE, GRID_CURRENT, GRID_VOLTAGE, Vmod;
PH3 offset_Iinv = {0}, offset_Vcap = {0}, offset_Igrid = {0}, offset_Vgrid = {0};
float offset_Idc = 0.0;

PARK INV_CURRENT_p, CAP_VOLTAGE_p, GRID_CURRENT_p, GRID_VOLTAGE_p;

MinMaxLimit MinMax_id, MinMax_iq, MinMax_vd, MinMax_vq;
LPF LPF_P, LPF_Q, LPF_Vd, LPF_IgD, LPF_IgQ;
Compensator_PI pi_id, pi_iq, pi_vd, pi_vq;
SVGEN svgen1 = SVGEN_DEFAULTS;

volatile struct EPWM_REGS *ePWM[PWM_CH] = { &EPwm1Regs, &EPwm2Regs, &EPwm3Regs };

//===========================================================================
// STATIC INLINE HELPER FUNCTIONS (Optimized for speed)
//===========================================================================
static inline void UpdateSystemFlagsAndDAC(void) {
    GpioDataRegs.GPBDAT.bit.GPIO36 = 1; 
    GpioDataRegs.GPBDAT.bit.GPIO35 = (RELAY == 1 ? 1 : 0);

    DacaRegs.DACVALS.all = P * (DAC_HIGH_VAL-DAC_LOW_VAL)/1000 + 1024;
    // if((user_flag == 1) && (prev_flag == 0)) {
    //     P_REF_MAX = 125;
    //     DacaRegs.DACVALS.all = DAC_HIGH_VAL;
    // }
    // else if((user_flag == 0) && (prev_flag == 1)) {
    //     DacaRegs.DACVALS.all = DAC_LOW_VAL; 
    // }
    // prev_flag = user_flag;
}

static inline void ReadAndScaleMeasurements(void) {
    volatile int16 temp;

    Vdc = ((temp = VDC) * ADC_PU_SCALE_FACTOR * VDC_SCALAR) - 1.0698; 
    Idc = ((temp = IDC) * ADC_PU_SCALE_FACTOR - offset_Idc) * IDC_SCALAR; 

    INV_CURRENT.As = ((temp = IINV_A) * ADC_PU_SCALE_FACTOR - offset_Iinv.As) * I_INV_SCALAR * curr_sens_adj_factor;
    INV_CURRENT.Bs = ((temp = IINV_B) * ADC_PU_SCALE_FACTOR - offset_Iinv.Bs) * I_INV_SCALAR * curr_sens_adj_factor;
    INV_CURRENT.Cs = ((temp = IINV_C) * ADC_PU_SCALE_FACTOR - offset_Iinv.Cs) * I_INV_SCALAR * curr_sens_adj_factor;
    INV_CURRENT_p.Ds = (INV_CURRENT.As*cos(th_droop[1]) + INV_CURRENT.Bs*cos(th_droop[1]-ALPHA) + INV_CURRENT.Cs*cos(th_droop[1]+ALPHA)) * PARK_GAIN;
    INV_CURRENT_p.Qs = (-INV_CURRENT.As*sin(th_droop[1]) - INV_CURRENT.Bs*sin(th_droop[1]-ALPHA) - INV_CURRENT.Cs*sin(th_droop[1]+ALPHA)) * PARK_GAIN;

    CAP_VOLTAGE.As = -((temp = VCAP_A) * ADC_PU_SCALE_FACTOR - offset_Vcap.As) * V_CAP_SCALAR * volt_sens_adj_factor;
    CAP_VOLTAGE.Bs = -((temp = VCAP_B) * ADC_PU_SCALE_FACTOR - offset_Vcap.Bs) * V_CAP_SCALAR * volt_sens_adj_factor;
    CAP_VOLTAGE.Cs = -((temp = VCAP_C) * ADC_PU_SCALE_FACTOR - offset_Vcap.Cs) * V_CAP_SCALAR * volt_sens_adj_factor;
    CAP_VOLTAGE_p.Ds = (CAP_VOLTAGE.As*cos(th_droop[1]) + CAP_VOLTAGE.Bs*cos(th_droop[1]-ALPHA) + CAP_VOLTAGE.Cs*cos(th_droop[1]+ALPHA)) * PARK_GAIN;
    CAP_VOLTAGE_p.Qs = (-CAP_VOLTAGE.As*sin(th_droop[1]) - CAP_VOLTAGE.Bs*sin(th_droop[1]-ALPHA) - CAP_VOLTAGE.Cs*sin(th_droop[1]+ALPHA)) * PARK_GAIN;

    GRID_CURRENT.As = ((temp = IGRID_A) * ADC_PU_SCALE_FACTOR - offset_Igrid.As) * I_INV_SCALAR * curr_sens_adj_factor;
    GRID_CURRENT.Bs = ((temp = IGRID_B) * ADC_PU_SCALE_FACTOR - offset_Igrid.Bs) * I_INV_SCALAR * curr_sens_adj_factor;
    GRID_CURRENT.Cs = ((temp = IGRID_C) * ADC_PU_SCALE_FACTOR - offset_Igrid.Cs) * I_INV_SCALAR * curr_sens_adj_factor;
    GRID_CURRENT_p.Ds = (GRID_CURRENT.As*cos(th_droop[1]) + GRID_CURRENT.Bs*cos(th_droop[1]-ALPHA) + GRID_CURRENT.Cs*cos(th_droop[1]+ALPHA)) * PARK_GAIN;
    GRID_CURRENT_p.Qs = (-GRID_CURRENT.As*sin(th_droop[1]) - GRID_CURRENT.Bs*sin(th_droop[1]-ALPHA) - GRID_CURRENT.Cs*sin(th_droop[1]+ALPHA)) * PARK_GAIN;

    GRID_VOLTAGE.As = -((temp = VGRID_A) * ADC_PU_SCALE_FACTOR - offset_Vgrid.As) * V_CAP_SCALAR * volt_sens_adj_factor;
    GRID_VOLTAGE.Bs = -((temp = VGRID_B) * ADC_PU_SCALE_FACTOR - offset_Vgrid.Bs) * V_CAP_SCALAR * volt_sens_adj_factor;
    GRID_VOLTAGE.Cs = -((temp = VGRID_C) * ADC_PU_SCALE_FACTOR - offset_Vgrid.Cs) * V_CAP_SCALAR * volt_sens_adj_factor;
    GRID_VOLTAGE_p.Ds = (GRID_VOLTAGE.As*cos(th_droop[1]) + GRID_VOLTAGE.Bs*cos(th_droop[1]-ALPHA) + GRID_VOLTAGE.Cs*cos(th_droop[1]+ALPHA)) * PARK_GAIN;
    GRID_VOLTAGE_p.Qs = (-GRID_VOLTAGE.As*sin(th_droop[1]) - GRID_VOLTAGE.Bs*sin(th_droop[1]-ALPHA) - GRID_VOLTAGE.Cs*sin(th_droop[1]+ALPHA)) * PARK_GAIN;

    if (GRID_CURRENT.As < minGridCurrent) minGridCurrent = GRID_CURRENT.As;
    if (GRID_CURRENT.As > maxGridCurrent) maxGridCurrent = GRID_CURRENT.As;
    if (INV_CURRENT.As < minInvCurrent)   minInvCurrent = INV_CURRENT.As;
    if (INV_CURRENT.As > maxInvCurrent)   maxInvCurrent = INV_CURRENT.As;
}

static inline void CalculateActiveReactivePower(void) {
#ifdef PQ_CALC_NO_LPF
    P = 1.5 * (CAP_VOLTAGE_p.Ds*GRID_CURRENT_p.Ds + CAP_VOLTAGE_p.Qs*GRID_CURRENT_p.Qs);
    Q = 1.5 * (-CAP_VOLTAGE_p.Ds*GRID_CURRENT_p.Qs + CAP_VOLTAGE_p.Qs*GRID_CURRENT_p.Ds);
#elif defined(PQ_CALC_INNER_LOOPS)
    P = LPFCalculation(1.5 * (CAP_VOLTAGE_p.Ds*GRID_CURRENT_p.Ds + CAP_VOLTAGE_p.Qs*GRID_CURRENT_p.Qs), &LPF_P);
    Q = LPFCalculation(1.5 * (-CAP_VOLTAGE_p.Ds*GRID_CURRENT_p.Qs + CAP_VOLTAGE_p.Qs*GRID_CURRENT_p.Ds), &LPF_Q);
#elif defined(PQ_CALC_NO_INNER)
    P = LPFCalculation(1.5 * (V_droop*INV_CURRENT_p.Ds), &LPF_P);
    Q = LPFCalculation(1.5 * (-V_droop*INV_CURRENT_p.Qs), &LPF_Q);
#endif
}

static inline void ExecuteDroopControl(void) {
    if(RELAY == 0) {
        Vnom_meas = LPFCalculation(GRID_VOLTAGE_p.Ds, &LPF_Vd);
        V_droop   = Vnom;
        omega_droop[1] = omega_nom + presync_gain * GRID_VOLTAGE_p.Qs;
    } else {
        err_Q[1] = Q_REF - Q;
        int_err_Q[1] = int_err_Q[0] + 0.5 * ki_Q * (err_Q[1] + err_Q[0]) * Tsamp;
        V_droop = Vnom + mQ * err_Q[1]; 
        err_Q[0] = err_Q[1];
        int_err_Q[0] = int_err_Q[1];

        err_P[1] = P_REF - P;
        int_err_P[1] = int_err_P[0] + 0.5 * ki_P * (err_P[1] + err_P[0]) * Tsamp;
        omega_droop[1] = omega_nom + mP * err_P[1]; 
        err_P[0] = err_P[1];
        int_err_P[0] = int_err_P[1];

        if(GpioDataRegs.GPCDAT.bit.GPIO93 == 1) {
            P_REF += 0.05;
            if(P_REF > P_REF_MAX) P_REF = P_REF_MAX;
        } else {
            P_REF = 0.0;
        }
    }

    th_droop[1] = th_droop[0] + 0.5 * (omega_droop[1] + omega_droop[0]) * Tsamp;
    if(th_droop[1] > 2.0*PI) th_droop[1] -= 2.0*PI;
    else if(th_droop[1] < 0) th_droop[1] += 2.0*PI;

    VD_REF = V_droop;
    VQ_REF = 0.0;
}



static inline void ExecuteOuterVoltageLoop(void) {
    Igrid_filt_D = LPFCalculation(GRID_CURRENT_p.Ds, &LPF_IgD);
    Igrid_filt_Q = LPFCalculation(GRID_CURRENT_p.Qs, &LPF_IgQ);
#ifdef OUTER_LOOP_NO_CC_FF
    ID_REF = CompensatorCalculation_PI(CAP_VOLTAGE_p.Ds, VD_REF, &pi_vd, 0.0);
    IQ_REF = CompensatorCalculation_PI(CAP_VOLTAGE_p.Qs, VQ_REF, &pi_vq, 0.0);
#elif defined(OUTER_LOOP_BOTH_CC_FF)
    ID_REF = CompensatorCalculation_PI(CAP_VOLTAGE_p.Ds, VD_REF, &pi_vd, -omega_droop[1]*C*CAP_VOLTAGE_p.Qs + P/(1.5*V_droop));
    IQ_REF = CompensatorCalculation_PI(CAP_VOLTAGE_p.Qs, VQ_REF, &pi_vq, omega_droop[1]*C*CAP_VOLTAGE_p.Ds + Q/(1.5*V_droop));
#elif defined(OUTER_LOOP_ONLY_CC)
    ID_REF = CompensatorCalculation_PI(CAP_VOLTAGE_p.Ds, VD_REF, &pi_vd, -omega_droop[1]*C*CAP_VOLTAGE_p.Qs);
    IQ_REF = CompensatorCalculation_PI(CAP_VOLTAGE_p.Qs, VQ_REF, &pi_vq, omega_droop[1]*C*CAP_VOLTAGE_p.Ds);
#endif

    mag_I_DQ_REF = sqrt(ID_REF*ID_REF + IQ_REF*IQ_REF);
    angle_I_DQ_REF = atan2(IQ_REF, ID_REF);

    if(mag_I_DQ_REF > I_max) {
        ID_REF = I_max * cos(angle_I_DQ_REF);
        IQ_REF = I_max * sin(angle_I_DQ_REF);
    }
}

static inline void ExecuteInnerCurrentLoop(void) {
#ifdef INNER_LOOP_CC_FF
    Vmod_d = (CompensatorCalculation_PI(INV_CURRENT_p.Ds, ID_REF, &pi_id, -omega_droop[1]*L*INV_CURRENT_p.Qs + CAP_VOLTAGE_p.Ds)) / (0.5*Vdcnom);
    Vmod_q = (CompensatorCalculation_PI(INV_CURRENT_p.Qs, IQ_REF, &pi_iq, omega_droop[1]*L*INV_CURRENT_p.Ds + CAP_VOLTAGE_p.Qs)) / (0.5*Vdcnom);
#elif defined(INNER_LOOP_CC_FF_V0)
    Vmod_d = (CompensatorCalculation_PI(INV_CURRENT_p.Ds, ID_REF, &pi_id, -omega_droop[1]*L*INV_CURRENT_p.Qs + V_droop)) / (0.5*Vdcnom);
    Vmod_q = (CompensatorCalculation_PI(INV_CURRENT_p.Qs, IQ_REF, &pi_iq, omega_droop[1]*L*INV_CURRENT_p.Ds + 0)) / (0.5*Vdcnom);
#elif defined(INNER_LOOP_CC)
    Vmod_d = (CompensatorCalculation_PI(INV_CURRENT_p.Ds, ID_REF, &pi_id, -omega_droop[1]*L*INV_CURRENT_p.Qs)) / (0.5*Vdcnom);
    Vmod_q = (CompensatorCalculation_PI(INV_CURRENT_p.Qs, IQ_REF, &pi_iq, omega_droop[1]*L*INV_CURRENT_p.Ds)) / (0.5*Vdcnom);
#elif defined(INNER_LOOP_NONE)
    Vmod_d = (CompensatorCalculation_PI(INV_CURRENT_p.Ds, ID_REF, &pi_id, 0.0)) / (0.5*Vdcnom);
    Vmod_q = (CompensatorCalculation_PI(INV_CURRENT_p.Qs, IQ_REF, &pi_iq, 0.0)) / (0.5*Vdcnom);
#elif defined(INNER_LOOP_SINGLE_LOOP)
    Vmod_d = (V_droop) / (0.5*Vdcnom);
    Vmod_q = 0.0;
#endif
}

static inline void ApplyModulationLimitsAndTransform(void) {
    mag_mod_DQ_REF = sqrt(Vmod_d*Vmod_d + Vmod_q*Vmod_q); 
    angle_mod_DQ_REF = atan2(Vmod_q, Vmod_d);

    if(mag_mod_DQ_REF > mod_max) {
        Vmod_d = mod_max * cos(angle_mod_DQ_REF);
        Vmod_q = mod_max * sin(angle_mod_DQ_REF);
    }

    Vmod_alpha = Vmod_d*cos(th_droop[1]) - Vmod_q*sin(th_droop[1]);
    Vmod_beta  = Vmod_q*cos(th_droop[1]) + Vmod_d*sin(th_droop[1]);

    Vmod.As = Vmod_alpha;
    Vmod.Bs = 0.5 * (-Vmod_alpha + SQRT3*Vmod_beta);
    Vmod.Cs = 0.5 * (-Vmod_alpha - SQRT3*Vmod_beta);
}

static inline void UpdatePWMDutyCycles(void) {
    if(READY == 1 && FLT_STATUS == 0) {
        EPwm1Regs.CMPA.bit.CMPA = (uint16_t)((HALF_TBPRD_inv * Vmod.As) + HALF_TBPRD_inv);
        EPwm2Regs.CMPA.bit.CMPA = (uint16_t)((HALF_TBPRD_inv * Vmod.Bs) + HALF_TBPRD_inv);
        EPwm3Regs.CMPA.bit.CMPA = (uint16_t)((HALF_TBPRD_inv * Vmod.Cs) + HALF_TBPRD_inv);
    } else {
        EPwm1Regs.CMPA.bit.CMPA = 0;
        EPwm2Regs.CMPA.bit.CMPA = 0;
        EPwm3Regs.CMPA.bit.CMPA = 0;
    }
}

//===========================================================================
// ISR EXECUTION (Mapped to RAM for zero-wait-state performance)
//===========================================================================
#pragma CODE_SECTION(adca1_isr, ".TI.ramfunc");
interrupt void adca1_isr(void)
{
    UpdateSystemFlagsAndDAC();
    ReadAndScaleMeasurements();
    CalculateActiveReactivePower();
    ExecuteDroopControl();
    ExecuteOuterVoltageLoop(); 
    ExecuteInnerCurrentLoop();
    ApplyModulationLimitsAndTransform();
    UpdatePWMDutyCycles();

    th_droop[0] = th_droop[1];
    omega_droop[0] = omega_droop[1];
    
    GpioDataRegs.GPBDAT.bit.GPIO36 = 0; 
    AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; 
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1; 
}
