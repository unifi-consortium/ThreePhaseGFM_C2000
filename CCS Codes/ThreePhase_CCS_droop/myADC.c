/*
 * ADC.c
 *
 *  Created on: Aug 8, 2023
 *      Author: Rahul
 */
// The following are the changes needed to be made to configure it to sample at Tsw_DAB
// Change the TRIGSEL to 5
// Change the ACQPS to 14
// Change the ConfigureEPWM() to be triggered on EPWM1
// ACQPS: single ended 379d required 75 ns, differential requires 320 ns
// correspondong ACQPS : (75e-9*40e6) = 3 ; 320e-9*40e6 = 12.8

#include "ADC.h"


void InitAdcA()
{
  EALLOW;
  CpuSysRegs.PCLKCR13.bit.ADC_A = 1;     // ADC_A clock is turned ON
  AdcaRegs.ADCCTL2.bit.PRESCALE = 8;     //set ADCCLK divider to /5, 200 MHz/5 = 40 MHz
  AdcSetMode(ADC_ADCA, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);

  //power up the ADC
  AdcaRegs.ADCCTL1.bit.ADCPWDNZ = 1;

  //delay for 1ms to allow ADC time to power up
  DELAY_US(1000);
  EDIS;
}

void InitAdcB()
{
  EALLOW;
  CpuSysRegs.PCLKCR13.bit.ADC_B = 1;
  AdcbRegs.ADCCTL2.bit.PRESCALE = 8;
  AdcSetMode(ADC_ADCB, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);

  //power up the ADC
  AdcbRegs.ADCCTL1.bit.ADCPWDNZ = 1;

  //delay for 1ms to allow ADC time to power up
  DELAY_US(1000);
  EDIS;
}

void InitAdcC()
{
  EALLOW;
  CpuSysRegs.PCLKCR13.bit.ADC_C = 1;
  AdccRegs.ADCCTL2.bit.PRESCALE = 8;
  AdcSetMode(ADC_ADCC, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);

  //power up the ADC
  AdccRegs.ADCCTL1.bit.ADCPWDNZ = 1;

  //delay for 1ms to allow ADC time to power up
  DELAY_US(1000);
  EDIS;
}

void InitAdcD()
{
  EALLOW;
  CpuSysRegs.PCLKCR13.bit.ADC_D = 1;
  AdcdRegs.ADCCTL2.bit.PRESCALE = 8;
  AdcSetMode(ADC_ADCD, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);

  //power up the ADC
  AdcdRegs.ADCCTL1.bit.ADCPWDNZ = 1;

  //delay for 1ms to allow ADC time to power up
  DELAY_US(1000);
  EDIS;
}


////////////// IinvA ///////////////////////////////
void config_ADCA_SOC0(void)
{
  EALLOW;
  AdcaRegs.ADCSOC0CTL.bit.CHSEL = 2;   /* Set SOC0 channel select to ADCIN4*/
  AdcaRegs.ADCSOC0CTL.bit.TRIGSEL = 5;   //  EPWM1 socA = 5; Trigger on ePWM3 SOCA = 9 // epwm4 soc A= trigsel 11
  AdcaRegs.ADCSOC0CTL.bit.ACQPS = 10;  /* Set SOC0 S/H Window to 15 ADC Clock Cycles*/
  AdcaRegs.ADCINTSOCSEL1.bit.SOC0 = 0; /* SOCx No ADCINT Interrupt Trigger Select, only ePWM1 SOCA does*/

  AdcaRegs.ADCINTSEL1N2.bit.INT1E = 1; /* Enabled/Disable ADCINT1 interrupt*/
  AdcaRegs.ADCINTSEL1N2.bit.INT1SEL = 0;/* Setup EOC0 to trigger ADCINT1*/
  AdcaRegs.ADCINTSEL1N2.bit.INT1CONT = 0;/* Enable/Disable ADCINT1 Continuous mode*/

//  AdcaRegs.ADCOFFTRIM.bit.OFFTRIM = AdcaRegs.ADCOFFTRIM.bit.OFFTRIM;/* Set Offset Error Correctino Value*/
  AdcaRegs.ADCCTL1.bit.INTPULSEPOS = 1;/* Late interrupt pulse trips AdcResults latch*/
  AdcaRegs.ADCSOCPRICTL.bit.SOCPRIORITY = 0;/* All in round robin mode SOC Priority*/
  EDIS;
}

//////////////// IinvB///////////////////////

void config_ADCA_SOC1(void)
{
  EALLOW;
  AdcaRegs.ADCSOC1CTL.bit.CHSEL = 3;   /* Set SOC1 channel select to ADCIN2*/
  AdcaRegs.ADCSOC1CTL.bit.TRIGSEL = 5;
  AdcaRegs.ADCSOC1CTL.bit.ACQPS = 10;  /* Set SOC1 S/H Window to 20 ADC Clock Cycles*/

  AdcaRegs.ADCINTSOCSEL1.bit.SOC1 = 0; /* SOCx No ADCINT Interrupt Trigger Select.*/
  AdcaRegs.ADCCTL1.bit.INTPULSEPOS = 1;/* Late interrupt pulse trips AdcResults latch*/
  AdcaRegs.ADCSOCPRICTL.bit.SOCPRIORITY = 0;/* All in round robin mode SOC Priority*/
  EDIS;
}

/////////////////// GRID CURRENT U OR UV pin 17 or in Control board: Out_meas_curr_sec_! /////////////////////
void config_ADCA_SOC2(void)
{
  EALLOW;
  AdcaRegs.ADCSOC2CTL.bit.CHSEL = 4;   /* Set SOC1 channel select to ADCIN2*/
  AdcaRegs.ADCSOC2CTL.bit.TRIGSEL = 5;
  AdcaRegs.ADCSOC2CTL.bit.ACQPS = 10;  /* Set SOC1 S/H Window to 20 ADC Clock Cycles*/

  AdcaRegs.ADCINTSOCSEL1.bit.SOC2 = 0; /* SOCx No ADCINT Interrupt Trigger Select.*/
  AdcaRegs.ADCCTL1.bit.INTPULSEPOS = 1;/* Late interrupt pulse trips AdcResults latch*/
  AdcaRegs.ADCSOCPRICTL.bit.SOCPRIORITY = 0;/* All in round robin mode SOC Priority*/
  EDIS;
}
/////Idc///////////////////////////

void config_ADCB_SOC0(void)
{
  EALLOW;
  AdcbRegs.ADCSOC0CTL.bit.CHSEL = 0;   /* Set SOC0 channel select to ADCIN4*/
  AdcbRegs.ADCSOC0CTL.bit.TRIGSEL = 5;
  AdcbRegs.ADCSOC0CTL.bit.ACQPS = 10;  /* Set SOC0 S/H Window to 15 ADC Clock Cycles*/
  AdcbRegs.ADCINTSOCSEL1.bit.SOC0 = 0; /* SOCx No ADCINT Interrupt Trigger Select.*/
//  AdcbRegs.ADCOFFTRIM.bit.OFFTRIM = AdcaRegs.ADCOFFTRIM.bit.OFFTRIM;/* Set Offset Error Correctino Value*/
  AdcbRegs.ADCCTL1.bit.INTPULSEPOS = 1;/* Late interrupt pulse trips AdcResults latch*/
  AdcbRegs.ADCSOCPRICTL.bit.SOCPRIORITY = 0;/* All in round robin mode SOC Priority*/
  EDIS;
}

//////////////// Vdc//////////////////
void config_ADCB_SOC1(void)
{
  EALLOW;
  AdcbRegs.ADCSOC1CTL.bit.CHSEL = 1;   /* Set SOC0 channel select to ADCIN4*/
  AdcbRegs.ADCSOC1CTL.bit.TRIGSEL = 5;
  AdcbRegs.ADCSOC1CTL.bit.ACQPS = 10;  /* Set SOC0 S/H Window to 15 ADC Clock Cycles*/
  AdcbRegs.ADCINTSOCSEL1.bit.SOC1 = 0; /* SOCx No ADCINT Interrupt Trigger Select.*/
//  AdcbRegs.ADCOFFTRIM.bit.OFFTRIM = AdcaRegs.ADCOFFTRIM.bit.OFFTRIM;/* Set Offset Error Correctino Value*/
  AdcbRegs.ADCCTL1.bit.INTPULSEPOS = 1;/* Late interrupt pulse trips AdcResults latch*/
  AdcbRegs.ADCSOCPRICTL.bit.SOCPRIORITY = 0;/* All in round robin mode SOC Priority*/
  EDIS;
}

/////////////////// Vcap A/////////////////////
void config_ADCB_SOC2(void)
{
  EALLOW;
  AdcbRegs.ADCSOC2CTL.bit.CHSEL = 2;   /* Set SOC0 channel select to ADCIN4*/
  AdcbRegs.ADCSOC2CTL.bit.TRIGSEL = 5;
  AdcbRegs.ADCSOC2CTL.bit.ACQPS = 10;  /* Set SOC0 S/H Window to 15 ADC Clock Cycles*/
  AdcbRegs.ADCINTSOCSEL1.bit.SOC2 = 0; /* SOCx No ADCINT Interrupt Trigger Select.*/
//  AdcbRegs.ADCOFFTRIM.bit.OFFTRIM = AdcaRegs.ADCOFFTRIM.bit.OFFTRIM;/* Set Offset Error Correctino Value*/
  AdcbRegs.ADCCTL1.bit.INTPULSEPOS = 1;/* Late interrupt pulse trips AdcResults latch*/
  AdcbRegs.ADCSOCPRICTL.bit.SOCPRIORITY = 0;/* All in round robin mode SOC Priority*/
  EDIS;
}

/////////////////// Vcap B/////////////////////
void config_ADCB_SOC3(void)
{
  EALLOW;
  AdcbRegs.ADCSOC3CTL.bit.CHSEL = 3;   /* Set SOC0 channel select to ADCIN4*/
  AdcbRegs.ADCSOC3CTL.bit.TRIGSEL = 5;
  AdcbRegs.ADCSOC3CTL.bit.ACQPS = 10;  /* Set SOC0 S/H Window to 15 ADC Clock Cycles*/
  AdcbRegs.ADCINTSOCSEL1.bit.SOC3 = 0; /* SOCx No ADCINT Interrupt Trigger Select.*/
//  AdcbRegs.ADCOFFTRIM.bit.OFFTRIM = AdcaRegs.ADCOFFTRIM.bit.OFFTRIM;/* Set Offset Error Correctino Value*/
  AdcbRegs.ADCCTL1.bit.INTPULSEPOS = 1;/* Late interrupt pulse trips AdcResults latch*/
  AdcbRegs.ADCSOCPRICTL.bit.SOCPRIORITY = 0;/* All in round robin mode SOC Priority*/
  EDIS;
}

/////////////////// Vcap C/////////////////////
void config_ADCB_SOC4(void)
{
  EALLOW;
  AdcbRegs.ADCSOC4CTL.bit.CHSEL = 4;   /* Set SOC0 channel select to ADCIN4*/
  AdcbRegs.ADCSOC4CTL.bit.TRIGSEL = 5;
  AdcbRegs.ADCSOC4CTL.bit.ACQPS = 10;  /* Set SOC0 S/H Window to 15 ADC Clock Cycles*/
  AdcbRegs.ADCINTSOCSEL1.bit.SOC4 = 0; /* SOCx No ADCINT Interrupt Trigger Select.*/
//  AdcbRegs.ADCOFFTRIM.bit.OFFTRIM = AdcaRegs.ADCOFFTRIM.bit.OFFTRIM;/* Set Offset Error Correctino Value*/
  AdcbRegs.ADCCTL1.bit.INTPULSEPOS = 1;/* Late interrupt pulse trips AdcResults latch*/
  AdcbRegs.ADCSOCPRICTL.bit.SOCPRIORITY = 0;/* All in round robin mode SOC Priority*/
  EDIS;
}
//////////////// Igrid A////////////////

void config_ADCC_SOC0(void)
{
  EALLOW;
  AdccRegs.ADCSOC0CTL.bit.CHSEL = 2;   /* Set SOC0 channel select to ADCIN4*/
  AdccRegs.ADCSOC0CTL.bit.TRIGSEL = 5;
  AdccRegs.ADCSOC0CTL.bit.ACQPS = 10;  /* Set SOC0 S/H Window to 15 ADC Clock Cycles*/
  AdccRegs.ADCINTSOCSEL1.bit.SOC0 = 0; /* SOCx No ADCINT Interrupt Trigger Select.*/
//  AdccRegs.ADCOFFTRIM.bit.OFFTRIM = AdcaRegs.ADCOFFTRIM.bit.OFFTRIM;/* Set Offset Error Correctino Value*/
  AdccRegs.ADCCTL1.bit.INTPULSEPOS = 1;/* Late interrupt pulse trips AdcResults latch*/
  AdccRegs.ADCSOCPRICTL.bit.SOCPRIORITY = 0;/* All in round robin mode SOC Priority*/
  EDIS;
}

///////////// Igrid B////////////
void config_ADCC_SOC1(void)
{
  EALLOW;
  AdccRegs.ADCSOC1CTL.bit.CHSEL = 3;   /* Set SOC0 channel select to ADCIN4*/
  AdccRegs.ADCSOC1CTL.bit.TRIGSEL = 5;
  AdccRegs.ADCSOC1CTL.bit.ACQPS = 10;  /* Set SOC0 S/H Window to 15 ADC Clock Cycles*/
  AdccRegs.ADCINTSOCSEL1.bit.SOC1 = 0; /* SOCx No ADCINT Interrupt Trigger Select.*/
//  AdccRegs.ADCOFFTRIM.bit.OFFTRIM = AdcaRegs.ADCOFFTRIM.bit.OFFTRIM;/* Set Offset Error Correctino Value*/
  AdccRegs.ADCCTL1.bit.INTPULSEPOS = 1;/* Late interrupt pulse trips AdcResults latch*/
  AdccRegs.ADCSOCPRICTL.bit.SOCPRIORITY = 0;/* All in round robin mode SOC Priority*/
  EDIS;
}

///////////// Igrid C ////////////
void config_ADCC_SOC2(void)
{
  EALLOW;
  AdccRegs.ADCSOC2CTL.bit.CHSEL = 4;   /* Set SOC0 channel select to ADCIN4*/
  AdccRegs.ADCSOC2CTL.bit.TRIGSEL = 5;
  AdccRegs.ADCSOC2CTL.bit.ACQPS = 10;  /* Set SOC0 S/H Window to 15 ADC Clock Cycles*/
  AdccRegs.ADCINTSOCSEL1.bit.SOC2 = 0; /* SOCx No ADCINT Interrupt Trigger Select.*/
//  AdccRegs.ADCOFFTRIM.bit.OFFTRIM = AdcaRegs.ADCOFFTRIM.bit.OFFTRIM;/* Set Offset Error Correctino Value*/
  AdccRegs.ADCCTL1.bit.INTPULSEPOS = 1;/* Late interrupt pulse trips AdcResults latch*/
  AdccRegs.ADCSOCPRICTL.bit.SOCPRIORITY = 0;/* All in round robin mode SOC Priority*/
  EDIS;
}


///////////// Vgrid A ////////////
void config_ADCD_SOC0(void)
{
  EALLOW;
  AdcdRegs.ADCSOC0CTL.bit.CHSEL = 2;
  AdcdRegs.ADCSOC0CTL.bit.TRIGSEL = 5;
  AdcdRegs.ADCSOC0CTL.bit.ACQPS = 10;  /* Set SOC0 S/H Window to 15 ADC Clock Cycles*/
  AdcdRegs.ADCINTSOCSEL1.bit.SOC0 = 0;
  AdcdRegs.ADCCTL1.bit.INTPULSEPOS = 1;
  AdcdRegs.ADCSOCPRICTL.bit.SOCPRIORITY = 0;
  EDIS;
}

///////////// Vgrid B ////////////
void config_ADCD_SOC1(void)
{
  EALLOW;
  AdcdRegs.ADCSOC1CTL.bit.CHSEL = 3;
  AdcdRegs.ADCSOC1CTL.bit.TRIGSEL = 5;
  AdcdRegs.ADCSOC1CTL.bit.ACQPS = 10;  /* Set SOC0 S/H Window to 15 ADC Clock Cycles*/
  AdcdRegs.ADCINTSOCSEL1.bit.SOC1 = 0;
  AdcdRegs.ADCCTL1.bit.INTPULSEPOS = 1;
  AdcdRegs.ADCSOCPRICTL.bit.SOCPRIORITY = 0;
  EDIS;
}

///////////// Vgrid C ////////////
void config_ADCD_SOC2(void)
{
  EALLOW;
  AdcdRegs.ADCSOC2CTL.bit.CHSEL = 4;
  AdcdRegs.ADCSOC2CTL.bit.TRIGSEL = 5;
  AdcdRegs.ADCSOC2CTL.bit.ACQPS = 10;  /* Set SOC0 S/H Window to 15 ADC Clock Cycles*/
  AdcdRegs.ADCINTSOCSEL1.bit.SOC2 = 0;
  AdcdRegs.ADCCTL1.bit.INTPULSEPOS = 1;
  AdcdRegs.ADCSOCPRICTL.bit.SOCPRIORITY = 0;
  EDIS;
}
void ConfigureEPWM(void)
{
    EALLOW;
    EPwm1Regs.ETSEL.bit.SOCAEN = 0;    // Disable SOC on A group
    EPwm1Regs.ETSEL.bit.SOCASEL = 2;    // Select SOC when TBCTR = PRD
    EPwm1Regs.ETPS.bit.SOCAPRD = 1;       // Generate pulse on 1st event
    EPwm1Regs.ETSEL.bit.SOCAEN = 1;  //enable SOCA
    EDIS;
}

