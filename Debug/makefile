################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := c:/ti/ccsv6/tools/compiler/c2000_6.2.5

ORDERED_OBJS += \
$(GEN_CMDS__FLAG) \
"./rampa.obj" \
"./main.obj" \
"./eQEP_lib.obj" \
"./PWM_config.obj" \
"./PID.obj" \
"./Modulator.obj" \
"./DSP2833x_usDelay.obj" \
"./DSP2833x_Xintf.obj" \
"./DSP2833x_SysCtrl.obj" \
"./DSP2833x_Spi.obj" \
"./DSP2833x_Sci.obj" \
"./DSP2833x_PieVect.obj" \
"./DSP2833x_PieCtrl.obj" \
"./DSP2833x_MemCopy.obj" \
"./DSP2833x_Mcbsp.obj" \
"./DSP2833x_I2C.obj" \
"./DSP2833x_Gpio.obj" \
"./DSP2833x_GlobalVariableDefs.obj" \
"./DSP2833x_EQep.obj" \
"./DSP2833x_EPwm.obj" \
"./DSP2833x_ECap.obj" \
"./DSP2833x_ECan.obj" \
"./DSP2833x_DisInt.obj" \
"./DSP2833x_DefaultIsr.obj" \
"./DSP2833x_DMA.obj" \
"./DSP2833x_DBGIER.obj" \
"./DSP2833x_CpuTimers.obj" \
"./DSP2833x_CodeStartBranch.obj" \
"./DSP2833x_CSMPasswords.obj" \
"./DSP2833x_Adc.obj" \
"./DSP2833x_ADC_cal.obj" \
"./ClarkPark.obj" \
"./ADC_config.obj" \
"../28335_RAM_lnk.cmd" \
"../DSP2833x_Headers_nonBIOS.cmd" \
-l"libc.a" \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
Testy.out \

EXE_OUTPUTS__QUOTED += \
"Testy.out" \

BIN_OUTPUTS += \
Testy.hex \

BIN_OUTPUTS__QUOTED += \
"Testy.hex" \


# All Target
all: Testy.out

# Tool invocations
Testy.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: $@'
	@echo 'Invoking: C2000 Linker'
	"c:/ti/ccsv6/tools/compiler/c2000_6.2.5/bin/cl2000" -v28 -ml -mt --float_support=fpu32 -g --diag_warning=225 --display_error_number --diag_wrap=off -z -m"Testy.map" --stack_size=0x300 --warn_sections -i"c:/ti/ccsv6/tools/compiler/c2000_6.2.5/lib" -i"c:/ti/ccsv6/tools/compiler/c2000_6.2.5/include" --reread_libs --display_error_number --diag_wrap=off --xml_link_info="Testy_linkInfo.xml" --rom_model -o "Testy.out" $(ORDERED_OBJS)
	@echo 'Finished building target: $@'
	@echo ' '

Testy.hex: $(EXE_OUTPUTS)
	@echo 'Invoking: C2000 Hex Utility'
	"c:/ti/ccsv6/tools/compiler/c2000_6.2.5/bin/hex2000"  -o "Testy.hex" $(EXE_OUTPUTS__QUOTED)
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXE_OUTPUTS__QUOTED)$(BIN_OUTPUTS__QUOTED)
	-$(RM) "ADC_config.pp" "ClarkPark.pp" "DSP2833x_Adc.pp" "DSP2833x_CpuTimers.pp" "DSP2833x_DMA.pp" "DSP2833x_DefaultIsr.pp" "DSP2833x_ECan.pp" "DSP2833x_ECap.pp" "DSP2833x_EPwm.pp" "DSP2833x_EQep.pp" "DSP2833x_GlobalVariableDefs.pp" "DSP2833x_Gpio.pp" "DSP2833x_I2C.pp" "DSP2833x_Mcbsp.pp" "DSP2833x_MemCopy.pp" "DSP2833x_PieCtrl.pp" "DSP2833x_PieVect.pp" "DSP2833x_Sci.pp" "DSP2833x_Spi.pp" "DSP2833x_SysCtrl.pp" "DSP2833x_Xintf.pp" "Modulator.pp" "PID.pp" "PWM_config.pp" "eQEP_lib.pp" "main.pp" "rampa.pp" 
	-$(RM) "ADC_config.obj" "ClarkPark.obj" "DSP2833x_ADC_cal.obj" "DSP2833x_Adc.obj" "DSP2833x_CSMPasswords.obj" "DSP2833x_CodeStartBranch.obj" "DSP2833x_CpuTimers.obj" "DSP2833x_DBGIER.obj" "DSP2833x_DMA.obj" "DSP2833x_DefaultIsr.obj" "DSP2833x_DisInt.obj" "DSP2833x_ECan.obj" "DSP2833x_ECap.obj" "DSP2833x_EPwm.obj" "DSP2833x_EQep.obj" "DSP2833x_GlobalVariableDefs.obj" "DSP2833x_Gpio.obj" "DSP2833x_I2C.obj" "DSP2833x_Mcbsp.obj" "DSP2833x_MemCopy.obj" "DSP2833x_PieCtrl.obj" "DSP2833x_PieVect.obj" "DSP2833x_Sci.obj" "DSP2833x_Spi.obj" "DSP2833x_SysCtrl.obj" "DSP2833x_Xintf.obj" "DSP2833x_usDelay.obj" "Modulator.obj" "PID.obj" "PWM_config.obj" "eQEP_lib.obj" "main.obj" "rampa.obj" 
	-$(RM) "DSP2833x_ADC_cal.pp" "DSP2833x_CSMPasswords.pp" "DSP2833x_CodeStartBranch.pp" "DSP2833x_DBGIER.pp" "DSP2833x_DisInt.pp" "DSP2833x_usDelay.pp" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

