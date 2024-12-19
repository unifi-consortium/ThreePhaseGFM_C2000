/*
 * ePWM.c
 *
 *  Created on: Aug 8, 2023
 *      Author: Rahul
 */


#include "ePWM.h"
#define EPWM_MIN_DB 76 //100000000*380ns =
float PWMph = 0;

//
// InitEPwm1Example - Initialize EPWM1 configuration
//

void InitEPwm1(float freq)
{

    int period;

    period = round(TBCOUNT/freq)-1;


    EPwm1Regs.TBCTL.bit.PRDLD = TB_SHADOW;  // set Shadow load
    EPwm1Regs.TBPRD = period;               // PWM frequency = 1/(2*TBPRD)
    EPwm1Regs.CMPA.bit.CMPA = period / 2;   // set duty 50% initially
    EPwm1Regs.CMPB.bit.CMPB = period / 2;   // set duty 50% initially
    EPwm1Regs.TBPHS.all = 0;
    EPwm1Regs.TBCTR = 0;

    EPwm1Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; // Select up-down
                                                        // count mode
    EPwm1Regs.TBCTL.bit.SYNCOSEL = TB_CTR_ZERO;    // SYNCO signal synced with CTR=0
    EPwm1Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;
    EPwm1Regs.TBCTL.bit.CLKDIV = TB_DIV1;          // TBCLK = SYSCLKOUT
    EPwm1Regs.TBCTL.bit.FREE_SOFT = 11;
    EPwm1Regs.TBCTL.bit.PHSDIR = 0; // Count DOWN on sync (=120 deg)
    EPwm1Regs.TBCTL.bit.PRDLD = TB_SHADOW;
    EPwm1Regs.TBCTL.bit.PHSEN = TB_DISABLE;


    EPwm1Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;  // LOAD CMPA on CTR = 0
    EPwm1Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;
    EPwm1Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
    EPwm1Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;

    EPwm1Regs.AQCTLA.bit.CAU = AQ_CLEAR;             // PWM toggle high/low
    EPwm1Regs.AQCTLA.bit.CAD = AQ_SET;
    EPwm1Regs.AQCTLB.bit.CBU = AQ_SET;             // PWM toggle high/low
    EPwm1Regs.AQCTLB.bit.CBD = AQ_CLEAR;

    EPwm1Regs.DBCTL.bit.OUT_MODE = 0x3;// normal
    EPwm1Regs.DBCTL.bit.POLSEL = 0x2; // A goes , B gets flipped
    EPwm1Regs.DBCTL.bit.IN_MODE = 0x0; // both gets A
    EPwm1Regs.DBRED.bit.DBRED = EPWM_MIN_DB;
    EPwm1Regs.DBFED.bit.DBFED = EPWM_MIN_DB;

    EPwm1Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;//ET_CTRU_CMPA; // INT on PRD event
    EPwm1Regs.ETSEL.bit.INTEN  = 1;              // Enable INT
 //   EPwm1Regs.ETPS.bit.SOCAPRD  = ET_3RD;         // Generate INT on every event
    EPwm1Regs.ETPS.bit.SOCPSSEL = 1;
    EPwm1Regs.ETSOCPS.bit.SOCAPRD2 = 0x5;
}

//
// InitEPwm2Example - Initialize EPWM2 configuration
//
void InitEPwm2(float freq)
{

    int period;

    period = round(TBCOUNT/freq)-1;

    EPwm2Regs.TBCTL.bit.PRDLD = TB_SHADOW;  // set Shadow load
    EPwm2Regs.TBPRD = period;               // PWM frequency = 1/(2*TBPRD)
    EPwm2Regs.CMPA.bit.CMPA = period / 2;   // set duty 50% initially
    EPwm2Regs.CMPB.bit.CMPB = period / 2;   // set duty 50% initially
    EPwm2Regs.TBPHS.all = 0;
    EPwm2Regs.TBCTR = 0;

    EPwm2Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; // Select up-down
                                                        // count mode
    EPwm2Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;    // SYNCO signal synced with CTR=0 ?? sends the signal from epwm1 to epwm3
    EPwm2Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;
    EPwm2Regs.TBCTL.bit.CLKDIV = TB_DIV1;          // TBCLK = SYSCLKOUT
    EPwm2Regs.TBCTL.bit.FREE_SOFT = 11;
    EPwm2Regs.TBCTL.bit.PHSDIR = 0; // Count DOWN on sync (=120 deg)
    EPwm2Regs.TBCTL.bit.PRDLD = TB_SHADOW;
    EPwm2Regs.TBCTL.bit.PHSEN = TB_DISABLE;


    EPwm2Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;  // LOAD CMPA on CTR = 0
    EPwm2Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;
    EPwm2Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
    EPwm2Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;

    EPwm2Regs.AQCTLA.bit.CAU = AQ_CLEAR;             // PWM toggle high/low
    EPwm2Regs.AQCTLA.bit.CAD = AQ_SET;
    EPwm2Regs.AQCTLB.bit.CBU = AQ_SET;             // PWM toggle high/low
    EPwm2Regs.AQCTLB.bit.CBD = AQ_CLEAR;

    EPwm2Regs.DBCTL.bit.OUT_MODE = 0x3;// normal
    EPwm2Regs.DBCTL.bit.POLSEL = 0x2; // A goes , B gets flipped
    EPwm2Regs.DBCTL.bit.IN_MODE = 0x0; // both gets A
    EPwm2Regs.DBRED.bit.DBRED = EPWM_MIN_DB;
    EPwm2Regs.DBFED.bit.DBFED = EPWM_MIN_DB;
}


//
// InitEPwm3Example - Initialize EPWM2 configuration
//
void InitEPwm3(float freq)
{

    int period;

        period = round(TBCOUNT/freq)-1;

        EPwm3Regs.TBCTL.bit.PRDLD = TB_SHADOW;  // set Shadow load
        EPwm3Regs.TBPRD = period;               // PWM frequency = 1/(2*TBPRD)
        EPwm3Regs.CMPA.bit.CMPA = period / 2;   // set duty 50% initially
        EPwm3Regs.CMPB.bit.CMPB = period / 2;   // set duty 50% initially
        EPwm3Regs.TBPHS.all = 0;
        EPwm3Regs.TBCTR = 0;

        EPwm3Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; // Select up-down
                                                            // count mode
        EPwm3Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;    // SYNCO signal synced with CTR=0
        EPwm3Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;
        EPwm3Regs.TBCTL.bit.CLKDIV = TB_DIV1;          // TBCLK = SYSCLKOUT
        EPwm3Regs.TBCTL.bit.FREE_SOFT = 11;
        EPwm3Regs.TBCTL.bit.PHSDIR = 0; // Count DOWN on sync (=120 deg)
        EPwm3Regs.TBCTL.bit.PRDLD = TB_SHADOW;
        EPwm3Regs.TBCTL.bit.PHSEN = TB_DISABLE;


        EPwm3Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;  // LOAD CMPA on CTR = 0
        EPwm3Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;
        EPwm3Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
        EPwm3Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;

        EPwm3Regs.AQCTLA.bit.CAU = AQ_CLEAR;             // PWM toggle high/low
        EPwm3Regs.AQCTLA.bit.CAD = AQ_SET;
        EPwm3Regs.AQCTLB.bit.CBU = AQ_SET;             // PWM toggle high/low
        EPwm3Regs.AQCTLB.bit.CBD = AQ_CLEAR;

        EPwm3Regs.DBCTL.bit.OUT_MODE = 0x3;// normal
        EPwm3Regs.DBCTL.bit.POLSEL = 0x2; // A goes , B gets flipped
        EPwm3Regs.DBCTL.bit.IN_MODE = 0x0; // both gets A
        EPwm3Regs.DBRED.bit.DBRED = EPWM_MIN_DB;
        EPwm3Regs.DBFED.bit.DBFED = EPWM_MIN_DB;
}


void InitEPwm4(float freq)
{

    int TBPRD;

      TBPRD = round(TBCOUNT/freq)-1;

      EPwm4Regs.TBPRD = TBPRD; // Period = 900 TBCLK counts
      EPwm4Regs.CMPA.bit.CMPA = TBPRD*0.5;
      EPwm4Regs.CMPB.bit.CMPB = TBPRD*0.5;
   //   EPwm4Regs.CMPC = TBPRD*0.5;
      EPwm4Regs.TBCTR = 0x0000;                     // Clear counter

      EPwm4Regs.TBPHS.bit.TBPHS = TBPRD*0;           // Phase is 0 // Phase = 300/900 * 360 = 120 deg
      EPwm4Regs.TBCTR = 0x0000;                     // Clear counter

      EPwm4Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;       // Clock ratio to SYSCLKOUT
      EPwm4Regs.TBCTL.bit.CLKDIV = TB_DIV1;

      EPwm4Regs.TBCTL.bit.CTRMODE = 2; // Symmetrical mode
      EPwm4Regs.TBCTL.bit.PHSDIR = 0; // Count DOWN on sync (=120 deg)
      EPwm4Regs.TBCTL.bit.PRDLD = TB_SHADOW;
      EPwm4Regs.TBCTL.bit.PHSEN = 0;
   //   EPwm4Regs.TBCTL.bit.SYNCOSEL = 1;//TB_SYNC_IN;


      EPwm4Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
      EPwm4Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
      EPwm4Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO; // load on CTR=Zero
      EPwm4Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO; // load on CTR=Zero

      SyncSocRegs.SYNCSELECT.bit.EPWM4SYNCIN = 1; // sync y epwm4
      EPwm4Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;

      //EPwm4Regs.AQCTLA.bit.ZRO = 2;
      EPwm4Regs.AQCTLA.bit.CAU = 1;            // Set PWM3A on Zero
      EPwm4Regs.AQCTLA.bit.CAD = 2;

      //EPwm4Regs.AQCTLB.bit.ZRO = 1;
      EPwm4Regs.AQCTLB.bit.CBU = 1;          // Set PWM3A on Zero
      EPwm4Regs.AQCTLB.bit.CBD = 2;

}



void InitEPwm5(float freq)
{

    int TBPRD;

    TBPRD = round(TBCOUNT/freq)-1;
    //TBPRD = 19999;
    EPwm5Regs.TBPRD = TBPRD; // Period = 900 TBCLK counts
    EPwm5Regs.CMPA.bit.CMPA = TBPRD*0.5;
    EPwm5Regs.CMPB.bit.CMPB = TBPRD*0.5;
  //  EPwm5Regs.CMPC = TBPRD*0.5;
    EPwm5Regs.TBCTR = 0x0000;                     // Clear counter

    EPwm5Regs.TBPHS.bit.TBPHS = TBPRD*0;           // Phase is 0 // Phase = 300/900 * 360 = 120 deg
    EPwm5Regs.TBCTR = 0x0000;                     // Clear counter

    EPwm5Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;       // Clock ratio to SYSCLKOUT
    EPwm5Regs.TBCTL.bit.CLKDIV = TB_DIV1;

    EPwm5Regs.TBCTL.bit.CTRMODE = 2; // Symmetrical mode
    EPwm5Regs.TBCTL.bit.PHSEN = 0; // Slave module
    EPwm5Regs.TBCTL.bit.PHSDIR = 0; // Count DOWN on sync (=120 deg)
    EPwm5Regs.TBCTL.bit.PRDLD = TB_SHADOW;
    EPwm5Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;


    EPwm5Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
    EPwm5Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
    EPwm5Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO; // load on CTR=Zero
    EPwm5Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO; // load on CTR=Zero

    EPwm5Regs.AQCTLA.bit.CAU = AQ_CLEAR;            // Set PWM3A on Zero
    EPwm5Regs.AQCTLA.bit.CAD = AQ_SET;

    EPwm5Regs.AQCTLB.bit.CBU = AQ_CLEAR;          // Set PWM3A on Zero
    EPwm5Regs.AQCTLB.bit.CBD = AQ_SET;

}



void InitEPwm6(float freq)
{

    int TBPRD;

       TBPRD = round(TBCOUNT/freq)-1;
     //   TBPRD = 19999;
       EPwm6Regs.TBPRD = TBPRD; // Period = 900 TBCLK counts
       EPwm6Regs.CMPA.bit.CMPA = TBPRD*0.5;
       EPwm6Regs.CMPB.bit.CMPB = TBPRD*0.5;
     //  EPwm6Regs.CMPC = TBPRD*0.5;
       EPwm6Regs.TBCTR = 0x0000;                     // Clear counter

       EPwm6Regs.TBPHS.bit.TBPHS = TBPRD*0;           // Phase is 0 // Phase = 300/900 * 360 = 120 deg
       EPwm6Regs.TBCTR = 0x0000;                     // Clear counter

       EPwm6Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;       // Clock ratio to SYSCLKOUT
       EPwm6Regs.TBCTL.bit.CLKDIV = TB_DIV1;

       EPwm6Regs.TBCTL.bit.CTRMODE = 2; // Symmetrical mode
       EPwm6Regs.TBCTL.bit.PHSEN = 0; // Slave module
       EPwm6Regs.TBCTL.bit.PHSDIR = 0; // Count DOWN on sync (=120 deg)
       EPwm6Regs.TBCTL.bit.PRDLD = TB_SHADOW;
     //  EPwm6Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;


       EPwm6Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
       EPwm6Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
       EPwm6Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO; // load on CTR=Zero
       EPwm6Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO; // load on CTR=Zero

       EPwm6Regs.AQCTLA.bit.CAU = AQ_CLEAR;            // Set PWM3A on Zero
       EPwm6Regs.AQCTLA.bit.CAD = AQ_SET;

       EPwm6Regs.AQCTLB.bit.CBU = AQ_CLEAR;          // Set PWM3A on Zero
       EPwm6Regs.AQCTLB.bit.CBD = AQ_SET;
}


void InitEPwm7(float freq)
{

    int period;

       period = round(TBCOUNT/freq)-1;

       //
       // ePWM channel register configuration with HRPWM
       // ePWMxA toggle low/high with MEP control on Rising edge
       //
       EALLOW;
       CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 0;   // Disable TBCLK within the EPWM
       EDIS;


       EPwm7Regs.TBCTL.bit.PRDLD = TB_SHADOW;  // set Shadow load
       EPwm7Regs.TBPRD = period;               // PWM frequency = 1/(2*TBPRD)
       EPwm7Regs.CMPA.bit.CMPA = period / 2;   // set duty 50% initially
       EPwm7Regs.CMPA.all |= 1;   // initialize HRPWM extension
       EPwm7Regs.CMPB.bit.CMPB = period / 2;   // set duty 50% initially
       EPwm7Regs.CMPB.all |= 1;
       EPwm7Regs.TBPHS.all = 0;
       EPwm7Regs.TBCTR = 0;

       EPwm7Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; // Select up-down
                                                           // count mode
       EPwm7Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;
       EPwm7Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;
       EPwm7Regs.TBCTL.bit.CLKDIV = TB_DIV1;          // TBCLK = SYSCLKOUT
       EPwm7Regs.TBCTL.bit.FREE_SOFT = 11;

       EPwm7Regs.TBCTL.bit.PHSEN = 1;

       EPwm7Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;  // LOAD CMPA on CTR = 0
       EPwm7Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;
       EPwm7Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
       EPwm7Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;

       EPwm7Regs.AQCTLA.bit.CAU = AQ_CLEAR;             // PWM toggle high/low
       EPwm7Regs.AQCTLA.bit.CAD = AQ_SET;
       EPwm7Regs.AQCTLB.bit.CBU = AQ_SET;             // PWM toggle high/low
       EPwm7Regs.AQCTLB.bit.CBD = AQ_CLEAR;

       EALLOW;

       EPwm7Regs.HRCNFG.all = 0x0;
       EPwm7Regs.HRCNFG.bit.EDGMODE = HR_BEP;          // MEP control on
                                                            // both edges.
       EPwm7Regs.HRCNFG.bit.CTLMODE = HR_CMP;          // CMPAHR and TBPRDHR
                                                            // HR control.
       EPwm7Regs.HRCNFG.bit.HRLOAD = HR_CTR_ZERO_PRD;  // load on CTR = 0
                                                            // and CTR = TBPRD
       EPwm7Regs.HRCNFG.bit.EDGMODEB = HR_BEP;         // MEP control on
                                                            // both edges
       EPwm7Regs.HRCNFG.bit.CTLMODEB = HR_CMP;         // CMPBHR and TBPRDHR
                                                            // HR control
       EPwm7Regs.HRCNFG.bit.HRLOADB = HR_CTR_ZERO_PRD; // load on CTR = 0
                                                            // and CTR = TBPRD
       EPwm7Regs.HRCNFG.bit.AUTOCONV = 1;        // Enable autoconversion for
                                                      // HR period

       EPwm7Regs.HRPCTL.bit.TBPHSHRLOADE = 1;    // Enable TBPHSHR sync
                                                      // (required for updwn
                                                      //  count HR control)
       EPwm7Regs.HRPCTL.bit.HRPE = 1;            // Turn on high-resolution
                                                      // period control.

       CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 1;      // Enable TBCLK within
                                                      // the EPWM
       EPwm7Regs.TBCTL.bit.SWFSYNC = 1;          // Synchronize high
                                                      // resolution phase to
                                                      // start HR period
       EDIS;


}

void InitEPwm11(float freq)
{

    int TBPRD;

      TBPRD = round(TBCOUNT/freq)-1;

      EALLOW;

      EPwm11Regs.TBPRD = TBPRD; // Period = 900 TBCLK counts
      EPwm11Regs.TBCTR = 0x0000;                     // Clear counter

      EPwm11Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;       // Clock ratio to SYSCLKOUT
      EPwm11Regs.TBCTL.bit.CLKDIV = TB_DIV1;

      EPwm11Regs.TBCTL.bit.CTRMODE = 2; // Symmetrical mode
      EPwm11Regs.TBCTL.bit.PRDLD = TB_SHADOW;



      EPwm11Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
      EPwm11Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
      EPwm11Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO; // load on CTR=Zero
      EPwm11Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO; // load on CTR=Zero

      //EPwm4Regs.AQCTLA.bit.ZRO = 2;
      EPwm11Regs.AQCTLA.bit.CAU = 1;            // Set PWM3A on Zero
      EPwm11Regs.AQCTLA.bit.CAD = 2;


      SyncSocRegs.SYNCSELECT.bit.EPWM10SYNCIN = 1; // sync y epwm4
      EPwm10Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;
      EPwm11Regs.TBCTL.bit.PHSEN = TB_ENABLE;
      EPwm11Regs.TBCTL.bit.PHSDIR = TB_UP;
      EPwm11Regs.TBPHS.bit.TBPHS = 2;

      EPwm11Regs.ETSEL.bit.INTSEL = 3;//ET_CTRU_CMPA; // INT on PRD event
      EPwm11Regs.ETSEL.bit.INTEN  = 1;              // Enable INT
      EPwm11Regs.ETPS.bit.INTPRD  = ET_1ST;         // Generate INT on every event


      EDIS;
}


