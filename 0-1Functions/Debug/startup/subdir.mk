################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../startup/startup_stm32f401xx.s 

OBJS += \
./startup/startup_stm32f401xx.o 


# Each subdirectory must supply rules for building sources it contributes
startup/%.o: ../startup/%.s
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Assembler'
	@echo %cd%
	arm-none-eabi-as -mcpu=cortex-m4 -mthumb -I"C:/Users/Filippo/workspace/stm32f401c-disco_stdperiph_lib" -I"C:/Users/Filippo/workspace/0-1Functions/inc" -I"C:/Users/Filippo/workspace/stm32f401c-disco_stdperiph_lib/CMSIS/core" -I"C:/Users/Filippo/workspace/stm32f401c-disco_stdperiph_lib/CMSIS/device" -I"C:/Users/Filippo/workspace/stm32f401c-disco_stdperiph_lib/StdPeriph_Driver/inc" -I"C:/Users/Filippo/workspace/stm32f401c-disco_stdperiph_lib/Utilities/STM32F401-Discovery" -g -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


