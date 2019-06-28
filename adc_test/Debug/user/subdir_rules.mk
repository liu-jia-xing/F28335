################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
user/%.obj: ../user/%.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs900/ccs/tools/compiler/ti-cgt-c2000_18.12.1.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="C:/ti/ccs900/ccs/tools/compiler/ti-cgt-c2000_18.12.1.LTS/include" --include_path="C:/F28335/adc_test/user" --include_path="C:/ti/c2000/C2000Ware_1_00_06_00/device_support/f2833x/headers/include" --include_path="C:/ti/c2000/C2000Ware_1_00_06_00/device_support/f2833x/common/include" --include_path="C:/ti/c2000/C2000Ware_1_00_06_00/libraries/math/IQmath/c28/include" --include_path="C:/ti/c2000/C2000Ware_1_00_06_00/libraries/math/FPUfastRTS/c28/include" --define="_DEBUG" --define="LARGE_MODEL" -g --diag_suppress=10063 --diag_warning=225 --issue_remarks --verbose_diagnostics --quiet --abi=coffabi --preproc_with_compile --preproc_dependency="user/$(basename $(<F)).d_raw" --obj_directory="user" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


