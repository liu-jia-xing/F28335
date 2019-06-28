################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
src/RAMandFLASH.obj: ../src/RAMandFLASH.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv5/tools/compiler/c2000_6.1.0/bin/cl2000" -v28 -ml -mt --float_support=fpu32 -g --include_path="C:/ti/ccsv5/tools/compiler/c2000_6.1.0/include" --include_path="E:/workspace_v5_2_ZQ28335_14COM_jjl/ZQ28335_EXRAMandNOR_FLASH/src" --include_path="E:/workspace_v5_2_ZQ28335_14COM_jjl/ZQ28335_EXRAMandNOR_FLASH/DSP2833x_common/include" --include_path="E:/workspace_v5_2_ZQ28335_14COM_jjl/ZQ28335_EXRAMandNOR_FLASH/DSP2833x_common/lib" --include_path="E:/workspace_v5_2_ZQ28335_14COM_jjl/ZQ28335_EXRAMandNOR_FLASH/DSP2833x_common/source" --include_path="E:/workspace_v5_2_ZQ28335_14COM_jjl/ZQ28335_EXRAMandNOR_FLASH/DSP2833x_headers/include" --include_path="E:/workspace_v5_2_ZQ28335_14COM_jjl/ZQ28335_EXRAMandNOR_FLASH/DSP2833x_headers/source" --diag_warning=225 --display_error_number --diag_wrap=off --preproc_with_compile --preproc_dependency="src/RAMandFLASH.pp" --obj_directory="src" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


