################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/main.c \
../src/syscalls.c \
../src/system_stm32f4xx.c 

OBJS += \
./src/main.o \
./src/syscalls.o \
./src/system_stm32f4xx.o 

C_DEPS += \
./src/main.d \
./src/syscalls.d \
./src/system_stm32f4xx.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -DSTM32F401VCTx -DSTM32F4 -DSTM32F401C_DISCO -DSTM32 -DUSE_STDPERIPH_DRIVER -DSTM32F401xx -I"C:/Users/Filippo/workspace/stm32f401c-disco_stdperiph_lib" -I"C:/Users/Filippo/workspace/0-1Functions/inc" -I"C:/Users/Filippo/workspace/stm32f401c-disco_stdperiph_lib/CMSIS/core" -I"C:/Users/Filippo/workspace/stm32f401c-disco_stdperiph_lib/CMSIS/device" -I"C:/Users/Filippo/workspace/stm32f401c-disco_stdperiph_lib/StdPeriph_Driver/inc" -I"C:/Users/Filippo/workspace/stm32f401c-disco_stdperiph_lib/Utilities/STM32F401-Discovery" -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


