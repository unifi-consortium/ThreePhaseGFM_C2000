/*
 * ADC.h
 *
 *  Created on: Aug 8, 2023
 *      Author: Rahul
 */

#ifndef ADC_H_
#define ADC_H_

#include "F28x_Project.h"
#include <math.h>
#include "PIR.h"

#ifndef TRUE
#define FALSE 0
#define TRUE 1
#endif

#define PI 3.14159265358979

#define SYSTEM_FREQUENCY 200 // 200 MHz
#define ISR_FREQUENCY 100 // This is also the inverter switching frequency
#define INV_PWM_TICKS ((SYSTEM_FREQUENCY)/ISR_FREQUENCY)*1000 //EPWMCLKDIV, HSPCLKDIV,CLKDIV = 0 all, TBCLK = SYSCLK
#define INV_PWM_TBPRD INV_PWM_TICKS/2
#define INV_PWM_HALF_TBPRD INV_PWM_TICKS/4   //5000

#define ADC_PU_SCALE_FACTOR 0.000244140625 // (1/2^12)

#define IDC AdcbResultRegs.ADCRESULT0
#define VDC AdcbResultRegs.ADCRESULT1

#define IINV_A AdcaResultRegs.ADCRESULT0
#define IINV_B AdcaResultRegs.ADCRESULT1
#define IINV_C AdcaResultRegs.ADCRESULT2

#define VCAP_A AdcbResultRegs.ADCRESULT2
#define VCAP_B AdcbResultRegs.ADCRESULT3
#define VCAP_C AdcbResultRegs.ADCRESULT4

#define IGRID_A AdccResultRegs.ADCRESULT0
#define IGRID_B AdccResultRegs.ADCRESULT1
#define IGRID_C AdccResultRegs.ADCRESULT2

#define VGRID_A AdcdResultRegs.ADCRESULT0
#define VGRID_B AdcdResultRegs.ADCRESULT1
#define VGRID_C AdcdResultRegs.ADCRESULT2

//void SetupADCEpwm(Uint16 channel);
interrupt void adca1_isr(void);
//interrupt void epwm11_isr(void);
//interrupt void adca2_isr(void);

void InitAdcA();
void InitAdcB();
void InitAdcC();
void InitAdcD();

void config_ADCA_SOC0(void);
void config_ADCA_SOC1(void);
void config_ADCA_SOC2(void);
void config_ADCB_SOC0(void);
void config_ADCB_SOC1(void);
void config_ADCB_SOC2(void);
void config_ADCB_SOC3(void);
void config_ADCB_SOC4(void);
void config_ADCC_SOC0(void);
void config_ADCC_SOC1(void);
void config_ADCC_SOC2(void);
void config_ADCD_SOC0(void);
void config_ADCD_SOC1(void);
void config_ADCD_SOC2(void);
void ConfigureEPWM(void);




#endif /* ADC_H_ */
