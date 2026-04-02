/*
 * dac_setup.c
 *
 *  Created on: Mar 30, 2026
 *      Author: jptol
 */

#include "dac_setup.h"
void InitDACA(void)
{
    EALLOW; // Allow writing to protected registers

    // Step 1: Enable the DAC-A clock
    CpuSysRegs.PCLKCR16.bit.DAC_A = 1;

    // Step 2: Set the Reference Voltage
    // controlCARDs typically use a precision 3.0V reference IC on VREFHI
    DacaRegs.DACCTL.bit.DACREFSEL = 1;
    DacaRegs.DACCTL.bit.LOADMODE = 0;  // Load value on next SYSCLK

    // Step 3: Power Up the DAC
    DacaRegs.DACOUTEN.bit.DACOUTEN = 1;
    DacaRegs.DACVALS.all = 0;

    // Step 4: Delay for DAC circuitry to power up and stabilize (~10us)
    DELAY_US(10);

    EDIS; // Disable writing to protected registers
}

void InitDACB(void)
{
    EALLOW; // Allow writing to protected registers

    // Step 1: Enable the DAC-A clock
    CpuSysRegs.PCLKCR16.bit.DAC_B = 1;

    // Step 2: Set the Reference Voltage
    // controlCARDs typically use a precision 3.0V reference IC on VREFHI
    DacbRegs.DACCTL.bit.DACREFSEL = 1;
    DacbRegs.DACCTL.bit.LOADMODE = 0;  // Load value on next SYSCLK

    // Step 3: Power Up the DAC
    DacbRegs.DACOUTEN.bit.DACOUTEN = 1;
    DacbRegs.DACVALS.all = 0;

    // Step 4: Delay for DAC circuitry to power up and stabilize (~10us)
    DELAY_US(10);

    EDIS; // Disable writing to protected registers
}

