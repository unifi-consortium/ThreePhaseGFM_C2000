################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/ti/ccs2020/ccs/tools/compiler/ti-cgt-c2000_22.6.2.LTS

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./DLOG_4CH_F_MVAC.obj" \
"./F2837xD_Adc.obj" \
"./F2837xD_CodeStartBranch.obj" \
"./F2837xD_DefaultISR.obj" \
"./F2837xD_EPwm.obj" \
"./F2837xD_GlobalVariableDefs.obj" \
"./F2837xD_Gpio.obj" \
"./F2837xD_PieCtrl.obj" \
"./F2837xD_PieVect.obj" \
"./F2837xD_SysCtrl.obj" \
"./F2837xD_usDelay.obj" \
"./PIR.obj" \
"./dac_setup.obj" \
"./ePWM.obj" \
"./inverter_init.obj" \
"./inverter_isr.obj" \
"./main.obj" \
"./myADC.obj" \
"../SFO_v8_fpu_lib_build_c28.lib" \
$(GEN_CMDS__FLAG) \
-l"C:/ti/C2000Ware_5_04_00_00/device_support/f2837xd/common/cmd/2837xD_RAM_lnk_cpu1.cmd" \
-lrts2800_fpu32.lib \
-l"C:/ti/C2000Ware_5_04_00_00/device_support/f2837xd/headers/cmd/F2837xD_Headers_BIOS_cpu1.cmd" \
-llibc.a \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
ThreePhaseDOEGFM_NewInverter_droop_reformat.out 

EXE_OUTPUTS__QUOTED += \
"ThreePhaseDOEGFM_NewInverter_droop_reformat.out" 


# All Target
all: ThreePhaseDOEGFM_NewInverter_droop_reformat.out

# Tool invocations
ThreePhaseDOEGFM_NewInverter_droop_reformat.out: $(OBJS) $(LIB_SRCS) $(GEN_CMDS)
	@echo 'Building target: "$@"'
	@echo 'Invoking: C2000 Linker'
	"C:/ti/ccs2020/ccs/tools/compiler/ti-cgt-c2000_22.6.2.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 -Ooff --fp_mode=relaxed --advice:performance=all --define=CPU1 -g --diag_suppress=1311 --diag_warning=225 --display_error_number --gen_data_subsections=on --abi=coffabi -z -m"ThreePhaseDOEGFM_NewInverter_droop_reformat.map" --stack_size=0x100 --warn_sections -i"C:/ti/ccs2020/ccs/tools/compiler/ti-cgt-c2000_22.6.2.LTS/lib" -i"C:/ti/C2000Ware_5_04_00_00/device_support/f2837xd/cmd" -i"C:/ti/ccs2020/ccs/tools/compiler/ti-cgt-c2000_22.6.2.LTS/include" --reread_libs --display_error_number --xml_link_info="ThreePhaseDOEGFM_NewInverter_droop_reformat_linkInfo.xml" --rom_model -o "ThreePhaseDOEGFM_NewInverter_droop_reformat.out" $(ORDERED_OBJS)
	@echo 'Finished building target: "$@"'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXE_OUTPUTS__QUOTED)
	-$(RM) "DLOG_4CH_F_MVAC.obj" "F2837xD_Adc.obj" "F2837xD_CodeStartBranch.obj" "F2837xD_DefaultISR.obj" "F2837xD_EPwm.obj" "F2837xD_GlobalVariableDefs.obj" "F2837xD_Gpio.obj" "F2837xD_PieCtrl.obj" "F2837xD_PieVect.obj" "F2837xD_SysCtrl.obj" "F2837xD_usDelay.obj" "PIR.obj" "dac_setup.obj" "ePWM.obj" "inverter_init.obj" "inverter_isr.obj" "main.obj" "myADC.obj" 
	-$(RM) "DLOG_4CH_F_MVAC.d" "F2837xD_Adc.d" "F2837xD_DefaultISR.d" "F2837xD_EPwm.d" "F2837xD_GlobalVariableDefs.d" "F2837xD_Gpio.d" "F2837xD_PieCtrl.d" "F2837xD_PieVect.d" "F2837xD_SysCtrl.d" "PIR.d" "dac_setup.d" "ePWM.d" "inverter_init.d" "inverter_isr.d" "main.d" "myADC.d" 
	-$(RM) "F2837xD_CodeStartBranch.d" "F2837xD_usDelay.d" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

