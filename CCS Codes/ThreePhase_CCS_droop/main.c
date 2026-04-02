//###########################################################################
// FILE:    main.c
// TITLE:   Main Entry Point and Background Tasks
//###########################################################################

#include "inverter_config.h"

void main(void)
{
    // Step 1: Device and system initialization
    InitSysCtrl();
    InitGpio();
    InitDACA();
    DacaRegs.DACVALS.all = DAC_LOW_VAL;

    ConfigureSystemPeripherals();

    // Step 2: Clear interrupts and initialize PIE table
    DINT;
    InitPieCtrl();
    IER = 0x0000;
    IFR = 0x0000;
    InitPieVectTable();

    // Map ADCA1 interrupt to the ISR
    EALLOW;
    PieVectTable.ADCA1_INT = &adca1_isr;
    EDIS;

    // Step 3: Initialize and configure specific peripherals
    InitAdcA(); InitAdcB(); InitAdcC(); InitAdcD();
    
    config_ADCA_SOC0(); config_ADCA_SOC1(); config_ADCA_SOC2();
    config_ADCB_SOC0(); config_ADCB_SOC1(); config_ADCB_SOC2();
    config_ADCB_SOC3(); config_ADCB_SOC4();
    config_ADCC_SOC0(); config_ADCC_SOC1(); config_ADCC_SOC2();
    config_ADCD_SOC0(); config_ADCD_SOC1(); config_ADCD_SOC2();

    ConfigureEPWM(); 
    InitEPwm1(f_inv); InitEPwm2(f_inv); InitEPwm3(f_inv);

    HALF_TBPRD_inv = (SYSTEM_FREQUENCY * 1000000 / f_inv) / 4; 

    // Initialize controller gains, limiters, and filters
    InitializeControlParameters();
    
    // Calculate steady state ADC offsets before turning on the PWM
    CalibrateADCOffsets();

    // Step 4: Enable global interrupts
    EALLOW;
    CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 1;
    EDIS;

    PieCtrlRegs.PIECTRL.bit.ENPIE = 1;
    PieCtrlRegs.PIEIER1.bit.INTx1 = 1;
    PieCtrlRegs.PIEIER1.bit.INTx7 = 1;
    PieCtrlRegs.PIEIER10.bit.INTx2 = 1;

    IER |= M_INT1; 
    IER |= M_INT10; 
    EINT;  
    ERTM;  

    // Step 5: Background Idle Loop
    do {
        syncflag = GpioDataRegs.GPCDAT.bit.GPIO91; 
        FLT_STATUS = (GpioDataRegs.GPCDAT.bit.GPIO90 == 0 ? 1 : 0); 
        READY = GpioDataRegs.GPCDAT.bit.GPIO92; 
        GpioDataRegs.GPCDAT.bit.GPIO94 = (RESET == 0 ? 1 : 0);
    } while(1);
}
