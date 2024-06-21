################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/TSL2591/DEV_Config.c \
../Core/TSL2591/TSL2591.c 

OBJS += \
./Core/TSL2591/DEV_Config.o \
./Core/TSL2591/TSL2591.o 

C_DEPS += \
./Core/TSL2591/DEV_Config.d \
./Core/TSL2591/TSL2591.d 


# Each subdirectory must supply rules for building sources it contributes
Core/TSL2591/%.o Core/TSL2591/%.su Core/TSL2591/%.cyclo: ../Core/TSL2591/%.c Core/TSL2591/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../Core/TSL2591 -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-TSL2591

clean-Core-2f-TSL2591:
	-$(RM) ./Core/TSL2591/DEV_Config.cyclo ./Core/TSL2591/DEV_Config.d ./Core/TSL2591/DEV_Config.o ./Core/TSL2591/DEV_Config.su ./Core/TSL2591/TSL2591.cyclo ./Core/TSL2591/TSL2591.d ./Core/TSL2591/TSL2591.o ./Core/TSL2591/TSL2591.su

.PHONY: clean-Core-2f-TSL2591

