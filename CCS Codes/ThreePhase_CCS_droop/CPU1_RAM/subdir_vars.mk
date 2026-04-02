################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Add inputs and outputs from these tool invocations to the build variables 
LIB_SRCS += \
../SFO_v8_fpu_lib_build_c28.lib 

ASM_SRCS += \
C:/ti/C2000Ware_5_04_00_00/device_support/f2837xd/common/source/F2837xD_CodeStartBranch.asm \
C:/ti/C2000Ware_5_04_00_00/device_support/f2837xd/common/source/F2837xD_usDelay.asm 

C_SRCS += \
../DLOG_4CH_F_MVAC.c \
C:/ti/C2000Ware_5_04_00_00/device_support/f2837xd/common/source/F2837xD_Adc.c \
C:/ti/C2000Ware_5_04_00_00/device_support/f2837xd/common/source/F2837xD_DefaultISR.c \
../F2837xD_EPwm.c \
C:/ti/C2000Ware_5_04_00_00/device_support/f2837xd/headers/source/F2837xD_GlobalVariableDefs.c \
C:/ti/C2000Ware_5_04_00_00/device_support/f2837xd/common/source/F2837xD_Gpio.c \
C:/ti/C2000Ware_5_04_00_00/device_support/f2837xd/common/source/F2837xD_PieCtrl.c \
C:/ti/C2000Ware_5_04_00_00/device_support/f2837xd/common/source/F2837xD_PieVect.c \
C:/ti/C2000Ware_5_04_00_00/device_support/f2837xd/common/source/F2837xD_SysCtrl.c \
../PIR.c \
../dac_setup.c \
../ePWM.c \
../inverter_init.c \
../inverter_isr.c \
../main.c \
../myADC.c 

C_DEPS += \
./DLOG_4CH_F_MVAC.d \
./F2837xD_Adc.d \
./F2837xD_DefaultISR.d \
./F2837xD_EPwm.d \
./F2837xD_GlobalVariableDefs.d \
./F2837xD_Gpio.d \
./F2837xD_PieCtrl.d \
./F2837xD_PieVect.d \
./F2837xD_SysCtrl.d \
./PIR.d \
./dac_setup.d \
./ePWM.d \
./inverter_init.d \
./inverter_isr.d \
./main.d \
./myADC.d 

OBJS += \
./DLOG_4CH_F_MVAC.obj \
./F2837xD_Adc.obj \
./F2837xD_CodeStartBranch.obj \
./F2837xD_DefaultISR.obj \
./F2837xD_EPwm.obj \
./F2837xD_GlobalVariableDefs.obj \
./F2837xD_Gpio.obj \
./F2837xD_PieCtrl.obj \
./F2837xD_PieVect.obj \
./F2837xD_SysCtrl.obj \
./F2837xD_usDelay.obj \
./PIR.obj \
./dac_setup.obj \
./ePWM.obj \
./inverter_init.obj \
./inverter_isr.obj \
./main.obj \
./myADC.obj 

ASM_DEPS += \
./F2837xD_CodeStartBranch.d \
./F2837xD_usDelay.d 

OBJS__QUOTED += \
"DLOG_4CH_F_MVAC.obj" \
"F2837xD_Adc.obj" \
"F2837xD_CodeStartBranch.obj" \
"F2837xD_DefaultISR.obj" \
"F2837xD_EPwm.obj" \
"F2837xD_GlobalVariableDefs.obj" \
"F2837xD_Gpio.obj" \
"F2837xD_PieCtrl.obj" \
"F2837xD_PieVect.obj" \
"F2837xD_SysCtrl.obj" \
"F2837xD_usDelay.obj" \
"PIR.obj" \
"dac_setup.obj" \
"ePWM.obj" \
"inverter_init.obj" \
"inverter_isr.obj" \
"main.obj" \
"myADC.obj" 

C_DEPS__QUOTED += \
"DLOG_4CH_F_MVAC.d" \
"F2837xD_Adc.d" \
"F2837xD_DefaultISR.d" \
"F2837xD_EPwm.d" \
"F2837xD_GlobalVariableDefs.d" \
"F2837xD_Gpio.d" \
"F2837xD_PieCtrl.d" \
"F2837xD_PieVect.d" \
"F2837xD_SysCtrl.d" \
"PIR.d" \
"dac_setup.d" \
"ePWM.d" \
"inverter_init.d" \
"inverter_isr.d" \
"main.d" \
"myADC.d" 

ASM_DEPS__QUOTED += \
"F2837xD_CodeStartBranch.d" \
"F2837xD_usDelay.d" 

C_SRCS__QUOTED += \
"../DLOG_4CH_F_MVAC.c" \
"C:/ti/C2000Ware_5_04_00_00/device_support/f2837xd/common/source/F2837xD_Adc.c" \
"C:/ti/C2000Ware_5_04_00_00/device_support/f2837xd/common/source/F2837xD_DefaultISR.c" \
"../F2837xD_EPwm.c" \
"C:/ti/C2000Ware_5_04_00_00/device_support/f2837xd/headers/source/F2837xD_GlobalVariableDefs.c" \
"C:/ti/C2000Ware_5_04_00_00/device_support/f2837xd/common/source/F2837xD_Gpio.c" \
"C:/ti/C2000Ware_5_04_00_00/device_support/f2837xd/common/source/F2837xD_PieCtrl.c" \
"C:/ti/C2000Ware_5_04_00_00/device_support/f2837xd/common/source/F2837xD_PieVect.c" \
"C:/ti/C2000Ware_5_04_00_00/device_support/f2837xd/common/source/F2837xD_SysCtrl.c" \
"../PIR.c" \
"../dac_setup.c" \
"../ePWM.c" \
"../inverter_init.c" \
"../inverter_isr.c" \
"../main.c" \
"../myADC.c" 

ASM_SRCS__QUOTED += \
"C:/ti/C2000Ware_5_04_00_00/device_support/f2837xd/common/source/F2837xD_CodeStartBranch.asm" \
"C:/ti/C2000Ware_5_04_00_00/device_support/f2837xd/common/source/F2837xD_usDelay.asm" 


