################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
%.obj: ../%.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs900/ccs/tools/compiler/ti-cgt-c2000_18.12.1.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="C:/ti/ccs900/ccs/tools/compiler/ti-cgt-c2000_18.12.1.LTS/include" --include_path="C:/ti/c2000/C2000Ware_1_00_06_00/device_support/f2833x/headers/include" --include_path="C:/ti/c2000/C2000Ware_1_00_06_00/device_support/f2833x/common/include" --include_path="C:/ti/c2000/C2000Ware_1_00_06_00/libraries/math/IQmath/c28/include" --include_path="C:/ti/c2000/C2000Ware_1_00_06_00/libraries/math/FPUfastRTS/c28/include" --define="_DEBUG" --define="LARGE_MODEL" -g --diag_suppress=10063 --diag_warning=225 --issue_remarks --verbose_diagnostics --quiet --abi=coffabi --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

DSP2833x_ADC_cal.obj: C:/ti/c2000/C2000Ware_1_00_06_00/device_support/f2833x/common/source/DSP2833x_ADC_cal.asm $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs900/ccs/tools/compiler/ti-cgt-c2000_18.12.1.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="C:/ti/ccs900/ccs/tools/compiler/ti-cgt-c2000_18.12.1.LTS/include" --include_path="C:/ti/c2000/C2000Ware_1_00_06_00/device_support/f2833x/headers/include" --include_path="C:/ti/c2000/C2000Ware_1_00_06_00/device_support/f2833x/common/include" --include_path="C:/ti/c2000/C2000Ware_1_00_06_00/libraries/math/IQmath/c28/include" --include_path="C:/ti/c2000/C2000Ware_1_00_06_00/libraries/math/FPUfastRTS/c28/include" --define="_DEBUG" --define="LARGE_MODEL" -g --diag_suppress=10063 --diag_warning=225 --issue_remarks --verbose_diagnostics --quiet --abi=coffabi --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

DSP2833x_CodeStartBranch.obj: C:/ti/c2000/C2000Ware_1_00_06_00/device_support/f2833x/common/source/DSP2833x_CodeStartBranch.asm $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs900/ccs/tools/compiler/ti-cgt-c2000_18.12.1.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="C:/ti/ccs900/ccs/tools/compiler/ti-cgt-c2000_18.12.1.LTS/include" --include_path="C:/ti/c2000/C2000Ware_1_00_06_00/device_support/f2833x/headers/include" --include_path="C:/ti/c2000/C2000Ware_1_00_06_00/device_support/f2833x/common/include" --include_path="C:/ti/c2000/C2000Ware_1_00_06_00/libraries/math/IQmath/c28/include" --include_path="C:/ti/c2000/C2000Ware_1_00_06_00/libraries/math/FPUfastRTS/c28/include" --define="_DEBUG" --define="LARGE_MODEL" -g --diag_suppress=10063 --diag_warning=225 --issue_remarks --verbose_diagnostics --quiet --abi=coffabi --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

DSP2833x_DefaultIsr.obj: C:/ti/c2000/C2000Ware_1_00_06_00/device_support/f2833x/common/source/DSP2833x_DefaultIsr.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs900/ccs/tools/compiler/ti-cgt-c2000_18.12.1.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="C:/ti/ccs900/ccs/tools/compiler/ti-cgt-c2000_18.12.1.LTS/include" --include_path="C:/ti/c2000/C2000Ware_1_00_06_00/device_support/f2833x/headers/include" --include_path="C:/ti/c2000/C2000Ware_1_00_06_00/device_support/f2833x/common/include" --include_path="C:/ti/c2000/C2000Ware_1_00_06_00/libraries/math/IQmath/c28/include" --include_path="C:/ti/c2000/C2000Ware_1_00_06_00/libraries/math/FPUfastRTS/c28/include" --define="_DEBUG" --define="LARGE_MODEL" -g --diag_suppress=10063 --diag_warning=225 --issue_remarks --verbose_diagnostics --quiet --abi=coffabi --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

DSP2833x_GlobalVariableDefs.obj: C:/ti/c2000/C2000Ware_1_00_06_00/device_support/f2833x/headers/source/DSP2833x_GlobalVariableDefs.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs900/ccs/tools/compiler/ti-cgt-c2000_18.12.1.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="C:/ti/ccs900/ccs/tools/compiler/ti-cgt-c2000_18.12.1.LTS/include" --include_path="C:/ti/c2000/C2000Ware_1_00_06_00/device_support/f2833x/headers/include" --include_path="C:/ti/c2000/C2000Ware_1_00_06_00/device_support/f2833x/common/include" --include_path="C:/ti/c2000/C2000Ware_1_00_06_00/libraries/math/IQmath/c28/include" --include_path="C:/ti/c2000/C2000Ware_1_00_06_00/libraries/math/FPUfastRTS/c28/include" --define="_DEBUG" --define="LARGE_MODEL" -g --diag_suppress=10063 --diag_warning=225 --issue_remarks --verbose_diagnostics --quiet --abi=coffabi --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

DSP2833x_PieCtrl.obj: C:/ti/c2000/C2000Ware_1_00_06_00/device_support/f2833x/common/source/DSP2833x_PieCtrl.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs900/ccs/tools/compiler/ti-cgt-c2000_18.12.1.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="C:/ti/ccs900/ccs/tools/compiler/ti-cgt-c2000_18.12.1.LTS/include" --include_path="C:/ti/c2000/C2000Ware_1_00_06_00/device_support/f2833x/headers/include" --include_path="C:/ti/c2000/C2000Ware_1_00_06_00/device_support/f2833x/common/include" --include_path="C:/ti/c2000/C2000Ware_1_00_06_00/libraries/math/IQmath/c28/include" --include_path="C:/ti/c2000/C2000Ware_1_00_06_00/libraries/math/FPUfastRTS/c28/include" --define="_DEBUG" --define="LARGE_MODEL" -g --diag_suppress=10063 --diag_warning=225 --issue_remarks --verbose_diagnostics --quiet --abi=coffabi --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

DSP2833x_PieVect.obj: C:/ti/c2000/C2000Ware_1_00_06_00/device_support/f2833x/common/source/DSP2833x_PieVect.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs900/ccs/tools/compiler/ti-cgt-c2000_18.12.1.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="C:/ti/ccs900/ccs/tools/compiler/ti-cgt-c2000_18.12.1.LTS/include" --include_path="C:/ti/c2000/C2000Ware_1_00_06_00/device_support/f2833x/headers/include" --include_path="C:/ti/c2000/C2000Ware_1_00_06_00/device_support/f2833x/common/include" --include_path="C:/ti/c2000/C2000Ware_1_00_06_00/libraries/math/IQmath/c28/include" --include_path="C:/ti/c2000/C2000Ware_1_00_06_00/libraries/math/FPUfastRTS/c28/include" --define="_DEBUG" --define="LARGE_MODEL" -g --diag_suppress=10063 --diag_warning=225 --issue_remarks --verbose_diagnostics --quiet --abi=coffabi --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

DSP2833x_SysCtrl.obj: C:/ti/c2000/C2000Ware_1_00_06_00/device_support/f2833x/common/source/DSP2833x_SysCtrl.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs900/ccs/tools/compiler/ti-cgt-c2000_18.12.1.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="C:/ti/ccs900/ccs/tools/compiler/ti-cgt-c2000_18.12.1.LTS/include" --include_path="C:/ti/c2000/C2000Ware_1_00_06_00/device_support/f2833x/headers/include" --include_path="C:/ti/c2000/C2000Ware_1_00_06_00/device_support/f2833x/common/include" --include_path="C:/ti/c2000/C2000Ware_1_00_06_00/libraries/math/IQmath/c28/include" --include_path="C:/ti/c2000/C2000Ware_1_00_06_00/libraries/math/FPUfastRTS/c28/include" --define="_DEBUG" --define="LARGE_MODEL" -g --diag_suppress=10063 --diag_warning=225 --issue_remarks --verbose_diagnostics --quiet --abi=coffabi --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

DSP2833x_usDelay.obj: C:/ti/c2000/C2000Ware_1_00_06_00/device_support/f2833x/common/source/DSP2833x_usDelay.asm $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs900/ccs/tools/compiler/ti-cgt-c2000_18.12.1.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="C:/ti/ccs900/ccs/tools/compiler/ti-cgt-c2000_18.12.1.LTS/include" --include_path="C:/ti/c2000/C2000Ware_1_00_06_00/device_support/f2833x/headers/include" --include_path="C:/ti/c2000/C2000Ware_1_00_06_00/device_support/f2833x/common/include" --include_path="C:/ti/c2000/C2000Ware_1_00_06_00/libraries/math/IQmath/c28/include" --include_path="C:/ti/c2000/C2000Ware_1_00_06_00/libraries/math/FPUfastRTS/c28/include" --define="_DEBUG" --define="LARGE_MODEL" -g --diag_suppress=10063 --diag_warning=225 --issue_remarks --verbose_diagnostics --quiet --abi=coffabi --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


