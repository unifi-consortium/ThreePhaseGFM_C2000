################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
%.obj: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs2020/ccs/tools/compiler/ti-cgt-c2000_22.6.2.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 -Ooff --fp_mode=relaxed --include_path="C:/ti/C2000Ware_5_04_00_00/device_support/f2837xd/headers/include" --include_path="C:/ti/C2000Ware_5_04_00_00/device_support/f2837xd/common/include" --include_path="C:/ti/ccs2020/ccs/tools/compiler/ti-cgt-c2000_22.6.2.LTS/include" --advice:performance=all --define=CPU1 -g --diag_suppress=1311 --diag_warning=225 --display_error_number --gen_data_subsections=on --abi=coffabi --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

F2837xD_Adc.obj: C:/ti/C2000Ware_5_04_00_00/device_support/f2837xd/common/source/F2837xD_Adc.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs2020/ccs/tools/compiler/ti-cgt-c2000_22.6.2.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 -Ooff --fp_mode=relaxed --include_path="C:/ti/C2000Ware_5_04_00_00/device_support/f2837xd/headers/include" --include_path="C:/ti/C2000Ware_5_04_00_00/device_support/f2837xd/common/include" --include_path="C:/ti/ccs2020/ccs/tools/compiler/ti-cgt-c2000_22.6.2.LTS/include" --advice:performance=all --define=CPU1 -g --diag_suppress=1311 --diag_warning=225 --display_error_number --gen_data_subsections=on --abi=coffabi --preproc_with_compile --preproc_dependency="F2837xD_Adc.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

F2837xD_CodeStartBranch.obj: C:/ti/C2000Ware_5_04_00_00/device_support/f2837xd/common/source/F2837xD_CodeStartBranch.asm $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs2020/ccs/tools/compiler/ti-cgt-c2000_22.6.2.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 -Ooff --fp_mode=relaxed --include_path="C:/ti/C2000Ware_5_04_00_00/device_support/f2837xd/headers/include" --include_path="C:/ti/C2000Ware_5_04_00_00/device_support/f2837xd/common/include" --include_path="C:/ti/ccs2020/ccs/tools/compiler/ti-cgt-c2000_22.6.2.LTS/include" --advice:performance=all --define=CPU1 -g --diag_suppress=1311 --diag_warning=225 --display_error_number --gen_data_subsections=on --abi=coffabi --preproc_with_compile --preproc_dependency="F2837xD_CodeStartBranch.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

F2837xD_DefaultISR.obj: C:/ti/C2000Ware_5_04_00_00/device_support/f2837xd/common/source/F2837xD_DefaultISR.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs2020/ccs/tools/compiler/ti-cgt-c2000_22.6.2.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 -Ooff --fp_mode=relaxed --include_path="C:/ti/C2000Ware_5_04_00_00/device_support/f2837xd/headers/include" --include_path="C:/ti/C2000Ware_5_04_00_00/device_support/f2837xd/common/include" --include_path="C:/ti/ccs2020/ccs/tools/compiler/ti-cgt-c2000_22.6.2.LTS/include" --advice:performance=all --define=CPU1 -g --diag_suppress=1311 --diag_warning=225 --display_error_number --gen_data_subsections=on --abi=coffabi --preproc_with_compile --preproc_dependency="F2837xD_DefaultISR.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

F2837xD_GlobalVariableDefs.obj: C:/ti/C2000Ware_5_04_00_00/device_support/f2837xd/headers/source/F2837xD_GlobalVariableDefs.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs2020/ccs/tools/compiler/ti-cgt-c2000_22.6.2.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 -Ooff --fp_mode=relaxed --include_path="C:/ti/C2000Ware_5_04_00_00/device_support/f2837xd/headers/include" --include_path="C:/ti/C2000Ware_5_04_00_00/device_support/f2837xd/common/include" --include_path="C:/ti/ccs2020/ccs/tools/compiler/ti-cgt-c2000_22.6.2.LTS/include" --advice:performance=all --define=CPU1 -g --diag_suppress=1311 --diag_warning=225 --display_error_number --gen_data_subsections=on --abi=coffabi --preproc_with_compile --preproc_dependency="F2837xD_GlobalVariableDefs.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

F2837xD_Gpio.obj: C:/ti/C2000Ware_5_04_00_00/device_support/f2837xd/common/source/F2837xD_Gpio.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs2020/ccs/tools/compiler/ti-cgt-c2000_22.6.2.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 -Ooff --fp_mode=relaxed --include_path="C:/ti/C2000Ware_5_04_00_00/device_support/f2837xd/headers/include" --include_path="C:/ti/C2000Ware_5_04_00_00/device_support/f2837xd/common/include" --include_path="C:/ti/ccs2020/ccs/tools/compiler/ti-cgt-c2000_22.6.2.LTS/include" --advice:performance=all --define=CPU1 -g --diag_suppress=1311 --diag_warning=225 --display_error_number --gen_data_subsections=on --abi=coffabi --preproc_with_compile --preproc_dependency="F2837xD_Gpio.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

F2837xD_PieCtrl.obj: C:/ti/C2000Ware_5_04_00_00/device_support/f2837xd/common/source/F2837xD_PieCtrl.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs2020/ccs/tools/compiler/ti-cgt-c2000_22.6.2.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 -Ooff --fp_mode=relaxed --include_path="C:/ti/C2000Ware_5_04_00_00/device_support/f2837xd/headers/include" --include_path="C:/ti/C2000Ware_5_04_00_00/device_support/f2837xd/common/include" --include_path="C:/ti/ccs2020/ccs/tools/compiler/ti-cgt-c2000_22.6.2.LTS/include" --advice:performance=all --define=CPU1 -g --diag_suppress=1311 --diag_warning=225 --display_error_number --gen_data_subsections=on --abi=coffabi --preproc_with_compile --preproc_dependency="F2837xD_PieCtrl.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

F2837xD_PieVect.obj: C:/ti/C2000Ware_5_04_00_00/device_support/f2837xd/common/source/F2837xD_PieVect.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs2020/ccs/tools/compiler/ti-cgt-c2000_22.6.2.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 -Ooff --fp_mode=relaxed --include_path="C:/ti/C2000Ware_5_04_00_00/device_support/f2837xd/headers/include" --include_path="C:/ti/C2000Ware_5_04_00_00/device_support/f2837xd/common/include" --include_path="C:/ti/ccs2020/ccs/tools/compiler/ti-cgt-c2000_22.6.2.LTS/include" --advice:performance=all --define=CPU1 -g --diag_suppress=1311 --diag_warning=225 --display_error_number --gen_data_subsections=on --abi=coffabi --preproc_with_compile --preproc_dependency="F2837xD_PieVect.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

F2837xD_SysCtrl.obj: C:/ti/C2000Ware_5_04_00_00/device_support/f2837xd/common/source/F2837xD_SysCtrl.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs2020/ccs/tools/compiler/ti-cgt-c2000_22.6.2.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 -Ooff --fp_mode=relaxed --include_path="C:/ti/C2000Ware_5_04_00_00/device_support/f2837xd/headers/include" --include_path="C:/ti/C2000Ware_5_04_00_00/device_support/f2837xd/common/include" --include_path="C:/ti/ccs2020/ccs/tools/compiler/ti-cgt-c2000_22.6.2.LTS/include" --advice:performance=all --define=CPU1 -g --diag_suppress=1311 --diag_warning=225 --display_error_number --gen_data_subsections=on --abi=coffabi --preproc_with_compile --preproc_dependency="F2837xD_SysCtrl.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

F2837xD_usDelay.obj: C:/ti/C2000Ware_5_04_00_00/device_support/f2837xd/common/source/F2837xD_usDelay.asm $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs2020/ccs/tools/compiler/ti-cgt-c2000_22.6.2.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 -Ooff --fp_mode=relaxed --include_path="C:/ti/C2000Ware_5_04_00_00/device_support/f2837xd/headers/include" --include_path="C:/ti/C2000Ware_5_04_00_00/device_support/f2837xd/common/include" --include_path="C:/ti/ccs2020/ccs/tools/compiler/ti-cgt-c2000_22.6.2.LTS/include" --advice:performance=all --define=CPU1 -g --diag_suppress=1311 --diag_warning=225 --display_error_number --gen_data_subsections=on --abi=coffabi --preproc_with_compile --preproc_dependency="F2837xD_usDelay.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


