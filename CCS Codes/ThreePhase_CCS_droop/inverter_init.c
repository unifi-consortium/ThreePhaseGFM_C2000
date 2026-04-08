//###########################################################################
// FILE:    inverter_init.c
// TITLE:   Hardware and Variable Initialization
//###########################################################################

#include "inverter_config.h"

void ConfigureSystemPeripherals(void)
{
    CpuSysRegs.PCLKCR2.bit.EPWM1 = 1;
    CpuSysRegs.PCLKCR2.bit.EPWM2 = 1;
    CpuSysRegs.PCLKCR2.bit.EPWM3 = 1;

    InitEPwm1Gpio();
    InitEPwm2Gpio();
    InitEPwm3Gpio();

    EALLOW;
    ClkCfgRegs.PERCLKDIVSEL.bit.EPWMCLKDIV = 0;

    // Output Pins
    GpioCtrlRegs.GPCGMUX2.bit.GPIO94 = 0; GpioCtrlRegs.GPCPUD.bit.GPIO94 = 1; GpioCtrlRegs.GPCDIR.bit.GPIO94 = 1; // RESET
    GpioCtrlRegs.GPBMUX1.bit.GPIO35 = 0; GpioCtrlRegs.GPBPUD.bit.GPIO35 = 1; GpioCtrlRegs.GPBDIR.bit.GPIO35 = 1; // RELAY
    GpioCtrlRegs.GPBMUX1.bit.GPIO36 = 0; GpioCtrlRegs.GPBPUD.bit.GPIO36 = 1; GpioCtrlRegs.GPBDIR.bit.GPIO36 = 1; // TOGGLE/DEBUG

    // Input Pins
    GpioCtrlRegs.GPCGMUX2.bit.GPIO90 = 0; GpioCtrlRegs.GPCPUD.bit.GPIO90 = 1; GpioCtrlRegs.GPCDIR.bit.GPIO90 = 0; // FLT_STATUS
    GpioCtrlRegs.GPCGMUX2.bit.GPIO92 = 0; GpioCtrlRegs.GPCPUD.bit.GPIO92 = 1; GpioCtrlRegs.GPCDIR.bit.GPIO92 = 0; // READY
    GpioCtrlRegs.GPCGMUX2.bit.GPIO91 = 0; GpioCtrlRegs.GPCPUD.bit.GPIO91 = 1; GpioCtrlRegs.GPCDIR.bit.GPIO91 = 0; // SW1
    GpioCtrlRegs.GPCGMUX2.bit.GPIO93 = 0; GpioCtrlRegs.GPCPUD.bit.GPIO93 = 1; GpioCtrlRegs.GPCDIR.bit.GPIO93 = 0; // SW2 (Power Ramp)
    EDIS;
}

void InitializeControlParameters(void)
{
    LPF_P  = InitiateLPF(Tsamp, BW_LPF);   // 10 Hz
    LPF_Q  = InitiateLPF(Tsamp, BW_LPF);   // 10 Hz
    LPF_Vd = InitiateLPF(Tsamp, 6.28319);   // 1 Hz
    LPF_IgD = InitiateLPF(Tsamp, 10*6.28319);   // 1 Hz
    LPF_IgQ = InitiateLPF(Tsamp, 10*6.28319);   // 1 Hz

    mP = 0.05 * omega_nom / 5000;
    mQ = 0.05 * Vnom / 5000; 

    MinMax_id = initiateMinMax(-0.45 * Vdcnom, 0.45 * Vdcnom);
    MinMax_iq = initiateMinMax(-0.45 * Vdcnom, 0.45 * Vdcnom);
    MinMax_vd = initiateMinMax(-I_max, I_max);
    MinMax_vq = initiateMinMax(-I_max, I_max);

    

#ifdef GAINS_WORKING
    // Working Gains
    pi_id = InitiateCompensator_PI(Tsamp, BW_cur*L, 0.4*BW_cur*L*BW_cur, 1000/(0.4*BW_cur*L*BW_cur), MinMax_id);
    pi_iq = InitiateCompensator_PI(Tsamp, BW_cur*L, 0.4*BW_cur*L*BW_cur, 1000/(0.4*BW_cur*L*BW_cur), MinMax_iq);
    pi_vd = InitiateCompensator_PI(Tsamp, BW_vol*C, 0.4*BW_vol*C*BW_vol, 1000/(0.4*BW_vol*C*BW_vol), MinMax_vd);
    pi_vq = InitiateCompensator_PI(Tsamp, BW_vol*C, 0.4*BW_vol*C*BW_vol, 1000/(0.4*BW_vol*C*BW_vol), MinMax_vq);
#elif defined(GAINS_REF)
    // Ref Design Gains
    pi_id = InitiateCompensator_PI(Tsamp, BW_cur*L, BW_cur*Rf, 1000/(BW_cur*Rf), MinMax_id);
    pi_iq = InitiateCompensator_PI(Tsamp, BW_cur*L, BW_cur*Rf, 1000/(BW_cur*Rf), MinMax_iq);
    pi_vd = InitiateCompensator_PI(Tsamp, BW_vol*C, BW_vol*C*BW_vol*BW_vol/BW_cur, 1000/(BW_vol*C*BW_vol*BW_vol/BW_cur), MinMax_vd);
    pi_vq = InitiateCompensator_PI(Tsamp, BW_vol*C, BW_vol*C*BW_vol*BW_vol/BW_cur, 1000/(BW_vol*C*BW_vol*BW_vol/BW_cur), MinMax_vq);
#elif defined(GAINS_DHRUV)
    // Dhruv Design Gains
    pi_id = InitiateCompensator_PI(Tsamp, 2*BW_cur*L - Rf, BW_cur*BW_cur*L, 1000/(BW_cur*BW_cur*L), MinMax_id);
    pi_iq = InitiateCompensator_PI(Tsamp, 2*BW_cur*L - Rf, BW_cur*BW_cur*L, 1000/(BW_cur*BW_cur*L), MinMax_iq);
    pi_vd = InitiateCompensator_PI(Tsamp, 1.414*BW_vol*C, BW_vol*C*BW_vol, 1000/(BW_vol*C*BW_vol), MinMax_vd);
    pi_vq = InitiateCompensator_PI(Tsamp, 1.414*BW_vol*C, BW_vol*C*BW_vol, 1000/(BW_vol*C*BW_vol), MinMax_vq);
#elif defined(CUSTOM)
    // Dhruv Design Gains
    pi_id = InitiateCompensator_PI(Tsamp, 2*BW_cur*L - Rf, 1*BW_cur*BW_cur*L, 10000/(BW_cur*BW_cur*L), MinMax_id);
    pi_iq = InitiateCompensator_PI(Tsamp, 2*BW_cur*L - Rf, 1*BW_cur*BW_cur*L, 10000/(BW_cur*BW_cur*L), MinMax_iq);
    pi_vd = InitiateCompensator_PI(Tsamp, 2*BW_vol*C, 1*BW_vol*C*BW_vol, 10000/(BW_vol*C*BW_vol), MinMax_vd);
    pi_vq = InitiateCompensator_PI(Tsamp, 2*BW_vol*C, 1*BW_vol*C*BW_vol, 10000/(BW_vol*C*BW_vol), MinMax_vq);
#endif
}

void CalibrateADCOffsets(void)
{
    for (OffsetCalCounter = 0; OffsetCalCounter < 20000; ) {
        if(AdcaRegs.ADCINTFLG.bit.ADCINT1 == 1) {
            if(OffsetCalCounter > 1000) {
                offset_Iinv.As  = K1*offset_Iinv.As  + K2*(IINV_A)*ADC_PU_SCALE_FACTOR;
                offset_Iinv.Bs  = K1*offset_Iinv.Bs  + K2*(IINV_B)*ADC_PU_SCALE_FACTOR;
                offset_Iinv.Cs  = K1*offset_Iinv.Cs  + K2*(IINV_C)*ADC_PU_SCALE_FACTOR;
                offset_Vcap.As  = K1*offset_Vcap.As  + K2*(VCAP_A)*ADC_PU_SCALE_FACTOR;
                offset_Vcap.Bs  = K1*offset_Vcap.Bs  + K2*(VCAP_B)*ADC_PU_SCALE_FACTOR;
                offset_Vcap.Cs  = K1*offset_Vcap.Cs  + K2*(VCAP_C)*ADC_PU_SCALE_FACTOR;
                offset_Igrid.As = K1*offset_Igrid.As + K2*(IGRID_A)*ADC_PU_SCALE_FACTOR;
                offset_Igrid.Bs = K1*offset_Igrid.Bs + K2*(IGRID_B)*ADC_PU_SCALE_FACTOR;
                offset_Igrid.Cs = K1*offset_Igrid.Cs + K2*(IGRID_C)*ADC_PU_SCALE_FACTOR;
                offset_Vgrid.As = K1*offset_Vgrid.As + K2*(VGRID_A)*ADC_PU_SCALE_FACTOR;
                offset_Vgrid.Bs = K1*offset_Vgrid.Bs + K2*(VGRID_B)*ADC_PU_SCALE_FACTOR;
                offset_Vgrid.Cs = K1*offset_Vgrid.Cs + K2*(VGRID_C)*ADC_PU_SCALE_FACTOR;
                offset_Idc      = K1*offset_Idc      + K2*(IDC)*ADC_PU_SCALE_FACTOR;
            }
            AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;
            OffsetCalCounter++;
        }
    }
}
