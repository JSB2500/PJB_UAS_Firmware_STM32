################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/JSB/Cloud/Family/03\ John\ and\ Anne\ children/00\ Philip/03\ Surrey\ University/01\ Uni\ 2nd\ year/Uber\ audio\ system/Firmware/STM32/Shared/JSB_General.c \
C:/JSB/Cloud/Family/03\ John\ and\ Anne\ children/00\ Philip/03\ Surrey\ University/01\ Uni\ 2nd\ year/Uber\ audio\ system/Firmware/STM32/Shared/JSB_ILI9341.c \
C:/JSB/Cloud/Family/03\ John\ and\ Anne\ children/00\ Philip/03\ Surrey\ University/01\ Uni\ 2nd\ year/Uber\ audio\ system/Firmware/STM32/Shared/JSB_XPT2046.c \
C:/JSB/Cloud/Family/03\ John\ and\ Anne\ children/00\ Philip/03\ Surrey\ University/01\ Uni\ 2nd\ year/Uber\ audio\ system/Firmware/STM32/Shared/PJB_STM32_ESP32_FM_DAB_ADC_DAC_PA.c \
../Src/dma.c \
../Src/go.c \
../Src/gpio.c \
../Src/i2c.c \
../Src/main.c \
../Src/sai.c \
../Src/spi.c \
../Src/stm32f7xx_hal_msp.c \
../Src/stm32f7xx_it.c \
../Src/system_stm32f7xx.c \
../Src/tim.c 

OBJS += \
./Src/JSB_General.o \
./Src/JSB_ILI9341.o \
./Src/JSB_XPT2046.o \
./Src/PJB_STM32_ESP32_FM_DAB_ADC_DAC_PA.o \
./Src/dma.o \
./Src/go.o \
./Src/gpio.o \
./Src/i2c.o \
./Src/main.o \
./Src/sai.o \
./Src/spi.o \
./Src/stm32f7xx_hal_msp.o \
./Src/stm32f7xx_it.o \
./Src/system_stm32f7xx.o \
./Src/tim.o 

C_DEPS += \
./Src/JSB_General.d \
./Src/JSB_ILI9341.d \
./Src/JSB_XPT2046.d \
./Src/PJB_STM32_ESP32_FM_DAB_ADC_DAC_PA.d \
./Src/dma.d \
./Src/go.d \
./Src/gpio.d \
./Src/i2c.d \
./Src/main.d \
./Src/sai.d \
./Src/spi.d \
./Src/stm32f7xx_hal_msp.d \
./Src/stm32f7xx_it.d \
./Src/system_stm32f7xx.d \
./Src/tim.d 


# Each subdirectory must supply rules for building sources it contributes
Src/JSB_General.o: C:/JSB/Cloud/Family/03\ John\ and\ Anne\ children/00\ Philip/03\ Surrey\ University/01\ Uni\ 2nd\ year/Uber\ audio\ system/Firmware/STM32/Shared/JSB_General.c Src/subdir.mk
	arm-none-eabi-gcc -c "$<" -mcpu=cortex-m7 -std=gnu11 -g3 '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../Inc -I../../Shared -IC:/ST/Firmware/STM32Cube_FW_F7_V1.15.0/Drivers/STM32F7xx_HAL_Driver/Inc -IC:/ST/Firmware/STM32Cube_FW_F7_V1.15.0/Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -IC:/ST/Firmware/STM32Cube_FW_F7_V1.15.0/Drivers/CMSIS/Device/ST/STM32F7xx/Include -IC:/ST/Firmware/STM32Cube_FW_F7_V1.15.0/Drivers/CMSIS/Include -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/JSB_General.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/JSB_ILI9341.o: C:/JSB/Cloud/Family/03\ John\ and\ Anne\ children/00\ Philip/03\ Surrey\ University/01\ Uni\ 2nd\ year/Uber\ audio\ system/Firmware/STM32/Shared/JSB_ILI9341.c Src/subdir.mk
	arm-none-eabi-gcc -c "$<" -mcpu=cortex-m7 -std=gnu11 -g3 '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../Inc -I../../Shared -IC:/ST/Firmware/STM32Cube_FW_F7_V1.15.0/Drivers/STM32F7xx_HAL_Driver/Inc -IC:/ST/Firmware/STM32Cube_FW_F7_V1.15.0/Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -IC:/ST/Firmware/STM32Cube_FW_F7_V1.15.0/Drivers/CMSIS/Device/ST/STM32F7xx/Include -IC:/ST/Firmware/STM32Cube_FW_F7_V1.15.0/Drivers/CMSIS/Include -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/JSB_ILI9341.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/JSB_XPT2046.o: C:/JSB/Cloud/Family/03\ John\ and\ Anne\ children/00\ Philip/03\ Surrey\ University/01\ Uni\ 2nd\ year/Uber\ audio\ system/Firmware/STM32/Shared/JSB_XPT2046.c Src/subdir.mk
	arm-none-eabi-gcc -c "$<" -mcpu=cortex-m7 -std=gnu11 -g3 '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../Inc -I../../Shared -IC:/ST/Firmware/STM32Cube_FW_F7_V1.15.0/Drivers/STM32F7xx_HAL_Driver/Inc -IC:/ST/Firmware/STM32Cube_FW_F7_V1.15.0/Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -IC:/ST/Firmware/STM32Cube_FW_F7_V1.15.0/Drivers/CMSIS/Device/ST/STM32F7xx/Include -IC:/ST/Firmware/STM32Cube_FW_F7_V1.15.0/Drivers/CMSIS/Include -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/JSB_XPT2046.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/PJB_STM32_ESP32_FM_DAB_ADC_DAC_PA.o: C:/JSB/Cloud/Family/03\ John\ and\ Anne\ children/00\ Philip/03\ Surrey\ University/01\ Uni\ 2nd\ year/Uber\ audio\ system/Firmware/STM32/Shared/PJB_STM32_ESP32_FM_DAB_ADC_DAC_PA.c Src/subdir.mk
	arm-none-eabi-gcc -c "$<" -mcpu=cortex-m7 -std=gnu11 -g3 '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../Inc -I../../Shared -IC:/ST/Firmware/STM32Cube_FW_F7_V1.15.0/Drivers/STM32F7xx_HAL_Driver/Inc -IC:/ST/Firmware/STM32Cube_FW_F7_V1.15.0/Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -IC:/ST/Firmware/STM32Cube_FW_F7_V1.15.0/Drivers/CMSIS/Device/ST/STM32F7xx/Include -IC:/ST/Firmware/STM32Cube_FW_F7_V1.15.0/Drivers/CMSIS/Include -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/PJB_STM32_ESP32_FM_DAB_ADC_DAC_PA.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/%.o Src/%.su: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc -c "$<" -mcpu=cortex-m7 -std=gnu11 -g3 '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../Inc -I../../Shared -IC:/ST/Firmware/STM32Cube_FW_F7_V1.15.0/Drivers/STM32F7xx_HAL_Driver/Inc -IC:/ST/Firmware/STM32Cube_FW_F7_V1.15.0/Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -IC:/ST/Firmware/STM32Cube_FW_F7_V1.15.0/Drivers/CMSIS/Device/ST/STM32F7xx/Include -IC:/ST/Firmware/STM32Cube_FW_F7_V1.15.0/Drivers/CMSIS/Include -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/JSB_General.d ./Src/JSB_General.o ./Src/JSB_General.su ./Src/JSB_ILI9341.d ./Src/JSB_ILI9341.o ./Src/JSB_ILI9341.su ./Src/JSB_XPT2046.d ./Src/JSB_XPT2046.o ./Src/JSB_XPT2046.su ./Src/PJB_STM32_ESP32_FM_DAB_ADC_DAC_PA.d ./Src/PJB_STM32_ESP32_FM_DAB_ADC_DAC_PA.o ./Src/PJB_STM32_ESP32_FM_DAB_ADC_DAC_PA.su ./Src/dma.d ./Src/dma.o ./Src/dma.su ./Src/go.d ./Src/go.o ./Src/go.su ./Src/gpio.d ./Src/gpio.o ./Src/gpio.su ./Src/i2c.d ./Src/i2c.o ./Src/i2c.su ./Src/main.d ./Src/main.o ./Src/main.su ./Src/sai.d ./Src/sai.o ./Src/sai.su ./Src/spi.d ./Src/spi.o ./Src/spi.su ./Src/stm32f7xx_hal_msp.d ./Src/stm32f7xx_hal_msp.o ./Src/stm32f7xx_hal_msp.su ./Src/stm32f7xx_it.d ./Src/stm32f7xx_it.o ./Src/stm32f7xx_it.su ./Src/system_stm32f7xx.d ./Src/system_stm32f7xx.o ./Src/system_stm32f7xx.su ./Src/tim.d ./Src/tim.o ./Src/tim.su

.PHONY: clean-Src

