################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/home/jsb/PJB\ Uber\ Audio\ System/PJB_UAS_Firmware_STM32/Shared/AR1010.c \
/home/jsb/PJB\ Uber\ Audio\ System/PJB_UAS_Firmware_STM32/Shared/JSB_General.c \
/home/jsb/PJB\ Uber\ Audio\ System/PJB_UAS_Firmware_STM32/Shared/JSB_ILI9341.c \
/home/jsb/PJB\ Uber\ Audio\ System/PJB_UAS_Firmware_STM32/Shared/JSB_IR_RC5.c \
/home/jsb/PJB\ Uber\ Audio\ System/PJB_UAS_Firmware_STM32/Shared/JSB_SPDIFRX.c \
/home/jsb/PJB\ Uber\ Audio\ System/PJB_UAS_Firmware_STM32/Shared/JSB_UI_Elements.c \
/home/jsb/PJB\ Uber\ Audio\ System/PJB_UAS_Firmware_STM32/Shared/JSB_XPT2046.c \
/home/jsb/PJB\ Uber\ Audio\ System/PJB_UAS_Firmware_STM32/Shared/PJB_SST25V.c \
/home/jsb/PJB\ Uber\ Audio\ System/PJB_UAS_Firmware_STM32/Shared/PJB_STM32_ESP32_FM_DAB_ADC_DAC_PA.c \
/home/jsb/PJB\ Uber\ Audio\ System/PJB_UAS_Firmware_STM32/Shared/PJB_StreamUtils.c \
/home/jsb/PJB\ Uber\ Audio\ System/PJB_UAS_Firmware_STM32/Shared/Si468x/Si468x.c \
/home/jsb/PJB\ Uber\ Audio\ System/PJB_UAS_Firmware_STM32/Shared/Si468x/Si468x_DAB.c \
/home/jsb/PJB\ Uber\ Audio\ System/PJB_UAS_Firmware_STM32/Shared/Si468x/Si468x_FM.c \
../Src/dma.c \
../Src/freertos.c \
../Src/go.c \
../Src/gpio.c \
../Src/i2c.c \
../Src/main.c \
../Src/sai.c \
../Src/spdifrx.c \
../Src/spi.c \
../Src/stm32f7xx_hal_msp.c \
../Src/stm32f7xx_hal_timebase_tim.c \
../Src/stm32f7xx_it.c \
../Src/syscalls.c \
../Src/system_stm32f7xx.c \
../Src/tim.c \
../Src/usart.c 

OBJS += \
./Src/AR1010.o \
./Src/JSB_General.o \
./Src/JSB_ILI9341.o \
./Src/JSB_IR_RC5.o \
./Src/JSB_SPDIFRX.o \
./Src/JSB_UI_Elements.o \
./Src/JSB_XPT2046.o \
./Src/PJB_SST25V.o \
./Src/PJB_STM32_ESP32_FM_DAB_ADC_DAC_PA.o \
./Src/PJB_StreamUtils.o \
./Src/Si468x.o \
./Src/Si468x_DAB.o \
./Src/Si468x_FM.o \
./Src/dma.o \
./Src/freertos.o \
./Src/go.o \
./Src/gpio.o \
./Src/i2c.o \
./Src/main.o \
./Src/sai.o \
./Src/spdifrx.o \
./Src/spi.o \
./Src/stm32f7xx_hal_msp.o \
./Src/stm32f7xx_hal_timebase_tim.o \
./Src/stm32f7xx_it.o \
./Src/syscalls.o \
./Src/system_stm32f7xx.o \
./Src/tim.o \
./Src/usart.o 

C_DEPS += \
./Src/AR1010.d \
./Src/JSB_General.d \
./Src/JSB_ILI9341.d \
./Src/JSB_IR_RC5.d \
./Src/JSB_SPDIFRX.d \
./Src/JSB_UI_Elements.d \
./Src/JSB_XPT2046.d \
./Src/PJB_SST25V.d \
./Src/PJB_STM32_ESP32_FM_DAB_ADC_DAC_PA.d \
./Src/PJB_StreamUtils.d \
./Src/Si468x.d \
./Src/Si468x_DAB.d \
./Src/Si468x_FM.d \
./Src/dma.d \
./Src/freertos.d \
./Src/go.d \
./Src/gpio.d \
./Src/i2c.d \
./Src/main.d \
./Src/sai.d \
./Src/spdifrx.d \
./Src/spi.d \
./Src/stm32f7xx_hal_msp.d \
./Src/stm32f7xx_hal_timebase_tim.d \
./Src/stm32f7xx_it.d \
./Src/syscalls.d \
./Src/system_stm32f7xx.d \
./Src/tim.d \
./Src/usart.d 


# Each subdirectory must supply rules for building sources it contributes
Src/AR1010.o: /home/jsb/PJB\ Uber\ Audio\ System/PJB_UAS_Firmware_STM32/Shared/AR1010.c Src/subdir.mk
	arm-none-eabi-gcc -c "$<" -mcpu=cortex-m7 -std=gnu11 -g3 '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32F767xx -DXPT2046_Swap_YD_and_YU -c -I../Inc -I../../Shared -I/home/jsb/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.4//Drivers/STM32F7xx_HAL_Driver/Inc -I/home/jsb/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.4//Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I/home/jsb/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.4//Drivers/CMSIS/Device/ST/STM32F7xx/Include -I/home/jsb/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.4//Drivers/CMSIS/Include -I/home/jsb/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.4//Middlewares/Third_Party/FreeRTOS/Source/include -I/home/jsb/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.4//Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I/home/jsb/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.4//Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -IC:/ST/Firmware/STM32AudioMiddlewares/Codecs/SpiritDSP_MP3_Dec/inc -Og -ffunction-sections -fdata-sections -Wall -Wno-main -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"Src/AR1010.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=softfp -mthumb -o "$@"
Src/JSB_General.o: /home/jsb/PJB\ Uber\ Audio\ System/PJB_UAS_Firmware_STM32/Shared/JSB_General.c Src/subdir.mk
	arm-none-eabi-gcc -c "$<" -mcpu=cortex-m7 -std=gnu11 -g3 '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32F767xx -DXPT2046_Swap_YD_and_YU -c -I../Inc -I../../Shared -I/home/jsb/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.4//Drivers/STM32F7xx_HAL_Driver/Inc -I/home/jsb/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.4//Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I/home/jsb/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.4//Drivers/CMSIS/Device/ST/STM32F7xx/Include -I/home/jsb/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.4//Drivers/CMSIS/Include -I/home/jsb/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.4//Middlewares/Third_Party/FreeRTOS/Source/include -I/home/jsb/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.4//Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I/home/jsb/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.4//Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -IC:/ST/Firmware/STM32AudioMiddlewares/Codecs/SpiritDSP_MP3_Dec/inc -Og -ffunction-sections -fdata-sections -Wall -Wno-main -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"Src/JSB_General.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=softfp -mthumb -o "$@"
Src/JSB_ILI9341.o: /home/jsb/PJB\ Uber\ Audio\ System/PJB_UAS_Firmware_STM32/Shared/JSB_ILI9341.c Src/subdir.mk
	arm-none-eabi-gcc -c "$<" -mcpu=cortex-m7 -std=gnu11 -g3 '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32F767xx -DXPT2046_Swap_YD_and_YU -c -I../Inc -I../../Shared -I/home/jsb/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.4//Drivers/STM32F7xx_HAL_Driver/Inc -I/home/jsb/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.4//Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I/home/jsb/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.4//Drivers/CMSIS/Device/ST/STM32F7xx/Include -I/home/jsb/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.4//Drivers/CMSIS/Include -I/home/jsb/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.4//Middlewares/Third_Party/FreeRTOS/Source/include -I/home/jsb/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.4//Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I/home/jsb/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.4//Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -IC:/ST/Firmware/STM32AudioMiddlewares/Codecs/SpiritDSP_MP3_Dec/inc -Og -ffunction-sections -fdata-sections -Wall -Wno-main -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"Src/JSB_ILI9341.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=softfp -mthumb -o "$@"
Src/JSB_IR_RC5.o: /home/jsb/PJB\ Uber\ Audio\ System/PJB_UAS_Firmware_STM32/Shared/JSB_IR_RC5.c Src/subdir.mk
	arm-none-eabi-gcc -c "$<" -mcpu=cortex-m7 -std=gnu11 -g3 '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32F767xx -DXPT2046_Swap_YD_and_YU -c -I../Inc -I../../Shared -I/home/jsb/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.4//Drivers/STM32F7xx_HAL_Driver/Inc -I/home/jsb/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.4//Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I/home/jsb/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.4//Drivers/CMSIS/Device/ST/STM32F7xx/Include -I/home/jsb/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.4//Drivers/CMSIS/Include -I/home/jsb/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.4//Middlewares/Third_Party/FreeRTOS/Source/include -I/home/jsb/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.4//Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I/home/jsb/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.4//Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -IC:/ST/Firmware/STM32AudioMiddlewares/Codecs/SpiritDSP_MP3_Dec/inc -Og -ffunction-sections -fdata-sections -Wall -Wno-main -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"Src/JSB_IR_RC5.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=softfp -mthumb -o "$@"
Src/JSB_SPDIFRX.o: /home/jsb/PJB\ Uber\ Audio\ System/PJB_UAS_Firmware_STM32/Shared/JSB_SPDIFRX.c Src/subdir.mk
	arm-none-eabi-gcc -c "$<" -mcpu=cortex-m7 -std=gnu11 -g3 '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32F767xx -DXPT2046_Swap_YD_and_YU -c -I../Inc -I../../Shared -I/home/jsb/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.4//Drivers/STM32F7xx_HAL_Driver/Inc -I/home/jsb/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.4//Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I/home/jsb/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.4//Drivers/CMSIS/Device/ST/STM32F7xx/Include -I/home/jsb/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.4//Drivers/CMSIS/Include -I/home/jsb/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.4//Middlewares/Third_Party/FreeRTOS/Source/include -I/home/jsb/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.4//Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I/home/jsb/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.4//Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -IC:/ST/Firmware/STM32AudioMiddlewares/Codecs/SpiritDSP_MP3_Dec/inc -Og -ffunction-sections -fdata-sections -Wall -Wno-main -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"Src/JSB_SPDIFRX.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=softfp -mthumb -o "$@"
Src/JSB_UI_Elements.o: /home/jsb/PJB\ Uber\ Audio\ System/PJB_UAS_Firmware_STM32/Shared/JSB_UI_Elements.c Src/subdir.mk
	arm-none-eabi-gcc -c "$<" -mcpu=cortex-m7 -std=gnu11 -g3 '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32F767xx -DXPT2046_Swap_YD_and_YU -c -I../Inc -I../../Shared -I/home/jsb/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.4//Drivers/STM32F7xx_HAL_Driver/Inc -I/home/jsb/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.4//Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I/home/jsb/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.4//Drivers/CMSIS/Device/ST/STM32F7xx/Include -I/home/jsb/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.4//Drivers/CMSIS/Include -I/home/jsb/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.4//Middlewares/Third_Party/FreeRTOS/Source/include -I/home/jsb/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.4//Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I/home/jsb/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.4//Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -IC:/ST/Firmware/STM32AudioMiddlewares/Codecs/SpiritDSP_MP3_Dec/inc -Og -ffunction-sections -fdata-sections -Wall -Wno-main -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"Src/JSB_UI_Elements.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=softfp -mthumb -o "$@"
Src/JSB_XPT2046.o: /home/jsb/PJB\ Uber\ Audio\ System/PJB_UAS_Firmware_STM32/Shared/JSB_XPT2046.c Src/subdir.mk
	arm-none-eabi-gcc -c "$<" -mcpu=cortex-m7 -std=gnu11 -g3 '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32F767xx -DXPT2046_Swap_YD_and_YU -c -I../Inc -I../../Shared -I/home/jsb/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.4//Drivers/STM32F7xx_HAL_Driver/Inc -I/home/jsb/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.4//Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I/home/jsb/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.4//Drivers/CMSIS/Device/ST/STM32F7xx/Include -I/home/jsb/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.4//Drivers/CMSIS/Include -I/home/jsb/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.4//Middlewares/Third_Party/FreeRTOS/Source/include -I/home/jsb/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.4//Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I/home/jsb/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.4//Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -IC:/ST/Firmware/STM32AudioMiddlewares/Codecs/SpiritDSP_MP3_Dec/inc -Og -ffunction-sections -fdata-sections -Wall -Wno-main -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"Src/JSB_XPT2046.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=softfp -mthumb -o "$@"
Src/PJB_SST25V.o: /home/jsb/PJB\ Uber\ Audio\ System/PJB_UAS_Firmware_STM32/Shared/PJB_SST25V.c Src/subdir.mk
	arm-none-eabi-gcc -c "$<" -mcpu=cortex-m7 -std=gnu11 -g3 '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32F767xx -DXPT2046_Swap_YD_and_YU -c -I../Inc -I../../Shared -I/home/jsb/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.4//Drivers/STM32F7xx_HAL_Driver/Inc -I/home/jsb/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.4//Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I/home/jsb/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.4//Drivers/CMSIS/Device/ST/STM32F7xx/Include -I/home/jsb/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.4//Drivers/CMSIS/Include -I/home/jsb/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.4//Middlewares/Third_Party/FreeRTOS/Source/include -I/home/jsb/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.4//Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I/home/jsb/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.4//Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -IC:/ST/Firmware/STM32AudioMiddlewares/Codecs/SpiritDSP_MP3_Dec/inc -Og -ffunction-sections -fdata-sections -Wall -Wno-main -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"Src/PJB_SST25V.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=softfp -mthumb -o "$@"
Src/PJB_STM32_ESP32_FM_DAB_ADC_DAC_PA.o: /home/jsb/PJB\ Uber\ Audio\ System/PJB_UAS_Firmware_STM32/Shared/PJB_STM32_ESP32_FM_DAB_ADC_DAC_PA.c Src/subdir.mk
	arm-none-eabi-gcc -c "$<" -mcpu=cortex-m7 -std=gnu11 -g3 '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32F767xx -DXPT2046_Swap_YD_and_YU -c -I../Inc -I../../Shared -I/home/jsb/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.4//Drivers/STM32F7xx_HAL_Driver/Inc -I/home/jsb/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.4//Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I/home/jsb/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.4//Drivers/CMSIS/Device/ST/STM32F7xx/Include -I/home/jsb/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.4//Drivers/CMSIS/Include -I/home/jsb/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.4//Middlewares/Third_Party/FreeRTOS/Source/include -I/home/jsb/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.4//Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I/home/jsb/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.4//Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -IC:/ST/Firmware/STM32AudioMiddlewares/Codecs/SpiritDSP_MP3_Dec/inc -Og -ffunction-sections -fdata-sections -Wall -Wno-main -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"Src/PJB_STM32_ESP32_FM_DAB_ADC_DAC_PA.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=softfp -mthumb -o "$@"
Src/PJB_StreamUtils.o: /home/jsb/PJB\ Uber\ Audio\ System/PJB_UAS_Firmware_STM32/Shared/PJB_StreamUtils.c Src/subdir.mk
	arm-none-eabi-gcc -c "$<" -mcpu=cortex-m7 -std=gnu11 -g3 '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32F767xx -DXPT2046_Swap_YD_and_YU -c -I../Inc -I../../Shared -I/home/jsb/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.4//Drivers/STM32F7xx_HAL_Driver/Inc -I/home/jsb/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.4//Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I/home/jsb/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.4//Drivers/CMSIS/Device/ST/STM32F7xx/Include -I/home/jsb/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.4//Drivers/CMSIS/Include -I/home/jsb/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.4//Middlewares/Third_Party/FreeRTOS/Source/include -I/home/jsb/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.4//Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I/home/jsb/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.4//Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -IC:/ST/Firmware/STM32AudioMiddlewares/Codecs/SpiritDSP_MP3_Dec/inc -Og -ffunction-sections -fdata-sections -Wall -Wno-main -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"Src/PJB_StreamUtils.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=softfp -mthumb -o "$@"
Src/Si468x.o: /home/jsb/PJB\ Uber\ Audio\ System/PJB_UAS_Firmware_STM32/Shared/Si468x/Si468x.c Src/subdir.mk
	arm-none-eabi-gcc -c "$<" -mcpu=cortex-m7 -std=gnu11 -g3 '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32F767xx -DXPT2046_Swap_YD_and_YU -c -I../Inc -I../../Shared -I/home/jsb/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.4//Drivers/STM32F7xx_HAL_Driver/Inc -I/home/jsb/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.4//Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I/home/jsb/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.4//Drivers/CMSIS/Device/ST/STM32F7xx/Include -I/home/jsb/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.4//Drivers/CMSIS/Include -I/home/jsb/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.4//Middlewares/Third_Party/FreeRTOS/Source/include -I/home/jsb/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.4//Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I/home/jsb/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.4//Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -IC:/ST/Firmware/STM32AudioMiddlewares/Codecs/SpiritDSP_MP3_Dec/inc -Og -ffunction-sections -fdata-sections -Wall -Wno-main -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"Src/Si468x.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=softfp -mthumb -o "$@"
Src/Si468x_DAB.o: /home/jsb/PJB\ Uber\ Audio\ System/PJB_UAS_Firmware_STM32/Shared/Si468x/Si468x_DAB.c Src/subdir.mk
	arm-none-eabi-gcc -c "$<" -mcpu=cortex-m7 -std=gnu11 -g3 '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32F767xx -DXPT2046_Swap_YD_and_YU -c -I../Inc -I../../Shared -I/home/jsb/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.4//Drivers/STM32F7xx_HAL_Driver/Inc -I/home/jsb/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.4//Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I/home/jsb/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.4//Drivers/CMSIS/Device/ST/STM32F7xx/Include -I/home/jsb/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.4//Drivers/CMSIS/Include -I/home/jsb/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.4//Middlewares/Third_Party/FreeRTOS/Source/include -I/home/jsb/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.4//Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I/home/jsb/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.4//Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -IC:/ST/Firmware/STM32AudioMiddlewares/Codecs/SpiritDSP_MP3_Dec/inc -Og -ffunction-sections -fdata-sections -Wall -Wno-main -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"Src/Si468x_DAB.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=softfp -mthumb -o "$@"
Src/Si468x_FM.o: /home/jsb/PJB\ Uber\ Audio\ System/PJB_UAS_Firmware_STM32/Shared/Si468x/Si468x_FM.c Src/subdir.mk
	arm-none-eabi-gcc -c "$<" -mcpu=cortex-m7 -std=gnu11 -g3 '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32F767xx -DXPT2046_Swap_YD_and_YU -c -I../Inc -I../../Shared -I/home/jsb/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.4//Drivers/STM32F7xx_HAL_Driver/Inc -I/home/jsb/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.4//Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I/home/jsb/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.4//Drivers/CMSIS/Device/ST/STM32F7xx/Include -I/home/jsb/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.4//Drivers/CMSIS/Include -I/home/jsb/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.4//Middlewares/Third_Party/FreeRTOS/Source/include -I/home/jsb/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.4//Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I/home/jsb/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.4//Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -IC:/ST/Firmware/STM32AudioMiddlewares/Codecs/SpiritDSP_MP3_Dec/inc -Og -ffunction-sections -fdata-sections -Wall -Wno-main -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"Src/Si468x_FM.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=softfp -mthumb -o "$@"
Src/%.o Src/%.su Src/%.cyclo: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc -c "$<" -mcpu=cortex-m7 -std=gnu11 -g3 '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32F767xx -DXPT2046_Swap_YD_and_YU -c -I../Inc -I../../Shared -I/home/jsb/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.4//Drivers/STM32F7xx_HAL_Driver/Inc -I/home/jsb/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.4//Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I/home/jsb/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.4//Drivers/CMSIS/Device/ST/STM32F7xx/Include -I/home/jsb/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.4//Drivers/CMSIS/Include -I/home/jsb/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.4//Middlewares/Third_Party/FreeRTOS/Source/include -I/home/jsb/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.4//Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I/home/jsb/STM32Cube/Repository/STM32Cube_FW_F7_V1.17.4//Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -IC:/ST/Firmware/STM32AudioMiddlewares/Codecs/SpiritDSP_MP3_Dec/inc -Og -ffunction-sections -fdata-sections -Wall -Wno-main -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=softfp -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/AR1010.cyclo ./Src/AR1010.d ./Src/AR1010.o ./Src/AR1010.su ./Src/JSB_General.cyclo ./Src/JSB_General.d ./Src/JSB_General.o ./Src/JSB_General.su ./Src/JSB_ILI9341.cyclo ./Src/JSB_ILI9341.d ./Src/JSB_ILI9341.o ./Src/JSB_ILI9341.su ./Src/JSB_IR_RC5.cyclo ./Src/JSB_IR_RC5.d ./Src/JSB_IR_RC5.o ./Src/JSB_IR_RC5.su ./Src/JSB_SPDIFRX.cyclo ./Src/JSB_SPDIFRX.d ./Src/JSB_SPDIFRX.o ./Src/JSB_SPDIFRX.su ./Src/JSB_UI_Elements.cyclo ./Src/JSB_UI_Elements.d ./Src/JSB_UI_Elements.o ./Src/JSB_UI_Elements.su ./Src/JSB_XPT2046.cyclo ./Src/JSB_XPT2046.d ./Src/JSB_XPT2046.o ./Src/JSB_XPT2046.su ./Src/PJB_SST25V.cyclo ./Src/PJB_SST25V.d ./Src/PJB_SST25V.o ./Src/PJB_SST25V.su ./Src/PJB_STM32_ESP32_FM_DAB_ADC_DAC_PA.cyclo ./Src/PJB_STM32_ESP32_FM_DAB_ADC_DAC_PA.d ./Src/PJB_STM32_ESP32_FM_DAB_ADC_DAC_PA.o ./Src/PJB_STM32_ESP32_FM_DAB_ADC_DAC_PA.su ./Src/PJB_StreamUtils.cyclo ./Src/PJB_StreamUtils.d ./Src/PJB_StreamUtils.o ./Src/PJB_StreamUtils.su ./Src/Si468x.cyclo ./Src/Si468x.d ./Src/Si468x.o ./Src/Si468x.su ./Src/Si468x_DAB.cyclo ./Src/Si468x_DAB.d ./Src/Si468x_DAB.o ./Src/Si468x_DAB.su ./Src/Si468x_FM.cyclo ./Src/Si468x_FM.d ./Src/Si468x_FM.o ./Src/Si468x_FM.su ./Src/dma.cyclo ./Src/dma.d ./Src/dma.o ./Src/dma.su ./Src/freertos.cyclo ./Src/freertos.d ./Src/freertos.o ./Src/freertos.su ./Src/go.cyclo ./Src/go.d ./Src/go.o ./Src/go.su ./Src/gpio.cyclo ./Src/gpio.d ./Src/gpio.o ./Src/gpio.su ./Src/i2c.cyclo ./Src/i2c.d ./Src/i2c.o ./Src/i2c.su ./Src/main.cyclo ./Src/main.d ./Src/main.o ./Src/main.su ./Src/sai.cyclo ./Src/sai.d ./Src/sai.o ./Src/sai.su ./Src/spdifrx.cyclo ./Src/spdifrx.d ./Src/spdifrx.o ./Src/spdifrx.su ./Src/spi.cyclo ./Src/spi.d ./Src/spi.o ./Src/spi.su ./Src/stm32f7xx_hal_msp.cyclo ./Src/stm32f7xx_hal_msp.d ./Src/stm32f7xx_hal_msp.o ./Src/stm32f7xx_hal_msp.su ./Src/stm32f7xx_hal_timebase_tim.cyclo ./Src/stm32f7xx_hal_timebase_tim.d ./Src/stm32f7xx_hal_timebase_tim.o ./Src/stm32f7xx_hal_timebase_tim.su ./Src/stm32f7xx_it.cyclo ./Src/stm32f7xx_it.d ./Src/stm32f7xx_it.o ./Src/stm32f7xx_it.su ./Src/syscalls.cyclo ./Src/syscalls.d ./Src/syscalls.o ./Src/syscalls.su ./Src/system_stm32f7xx.cyclo ./Src/system_stm32f7xx.d ./Src/system_stm32f7xx.o ./Src/system_stm32f7xx.su ./Src/tim.cyclo ./Src/tim.d ./Src/tim.o ./Src/tim.su ./Src/usart.cyclo ./Src/usart.d ./Src/usart.o ./Src/usart.su

.PHONY: clean-Src

