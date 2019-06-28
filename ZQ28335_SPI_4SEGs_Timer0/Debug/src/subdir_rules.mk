################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
src/main_8SEG_Timer0_new.obj: ../src/main_8SEG_Timer0_new.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv5/tools/compiler/c2000_6.1.0/bin/cl2000" -v28 -ml -mt --float_support=fpu32 -g --include_path="E:/workspace_v5_2_ZQ28335_14COM_jjl/main_8SEG_Timer0_new/DSP2833x_common/include" --include_path="E:/workspace_v5_2_ZQ28335_14COM_jjl/main_8SEG_Timer0_new/DSP2833x_common/source" --include_path="E:/workspace_v5_2_ZQ28335_14COM_jjl/main_8SEG_Timer0_new/DSP2833x_headers/include" --include_path="E:/workspace_v5_2_ZQ28335_14COM_jjl/main_8SEG_Timer0_new/DSP2833x_headers/source" --include_path="E:/workspace_v5_2_ZQ28335_14COM_jjl/main_8SEG_Timer0_new/src" --diag_warning=225 --display_error_number --diag_wrap=off --preproc_with_compile --preproc_dependency="src/main_8SEG_Timer0_new.pp" --obj_directory="src" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


