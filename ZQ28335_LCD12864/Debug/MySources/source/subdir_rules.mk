################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
MySources/source/ZQ28335_LCD12864_ST7920LR.obj: ../MySources/source/ZQ28335_LCD12864_ST7920LR.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv5/tools/compiler/c2000_6.1.0/bin/cl2000" -v28 -ml -mt --float_support=fpu32 -g --include_path="C:/ti/ccsv5/tools/compiler/c2000_6.1.0/include" --include_path="E:/workspace_v5_2_ZQ28335_CODE/ZQ28335_LCD12864/DSP2833x_common/include" --include_path="E:/workspace_v5_2_ZQ28335_CODE/ZQ28335_LCD12864/DSP2833x_common/source" --include_path="E:/workspace_v5_2_ZQ28335_CODE/ZQ28335_LCD12864/DSP2833x_headers/include" --include_path="E:/workspace_v5_2_ZQ28335_CODE/ZQ28335_LCD12864/MySources/include" --include_path="E:/workspace_v5_2_ZQ28335_CODE/ZQ28335_LCD12864/MySources/source" --diag_warning=225 --display_error_number --diag_wrap=off --preproc_with_compile --preproc_dependency="MySources/source/ZQ28335_LCD12864_ST7920LR.pp" --obj_directory="MySources/source" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


