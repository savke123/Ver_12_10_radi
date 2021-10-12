################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/Third_Party/MQTT/MQTTPacket/MQTTConnectClient.c \
../Middlewares/Third_Party/MQTT/MQTTPacket/MQTTConnectServer.c \
../Middlewares/Third_Party/MQTT/MQTTPacket/MQTTDeserializePublish.c \
../Middlewares/Third_Party/MQTT/MQTTPacket/MQTTFormat.c \
../Middlewares/Third_Party/MQTT/MQTTPacket/MQTTPacket.c \
../Middlewares/Third_Party/MQTT/MQTTPacket/MQTTSerializePublish.c \
../Middlewares/Third_Party/MQTT/MQTTPacket/MQTTSubscribeClient.c \
../Middlewares/Third_Party/MQTT/MQTTPacket/MQTTSubscribeServer.c \
../Middlewares/Third_Party/MQTT/MQTTPacket/MQTTUnsubscribeClient.c \
../Middlewares/Third_Party/MQTT/MQTTPacket/MQTTUnsubscribeServer.c 

OBJS += \
./Middlewares/Third_Party/MQTT/MQTTPacket/MQTTConnectClient.o \
./Middlewares/Third_Party/MQTT/MQTTPacket/MQTTConnectServer.o \
./Middlewares/Third_Party/MQTT/MQTTPacket/MQTTDeserializePublish.o \
./Middlewares/Third_Party/MQTT/MQTTPacket/MQTTFormat.o \
./Middlewares/Third_Party/MQTT/MQTTPacket/MQTTPacket.o \
./Middlewares/Third_Party/MQTT/MQTTPacket/MQTTSerializePublish.o \
./Middlewares/Third_Party/MQTT/MQTTPacket/MQTTSubscribeClient.o \
./Middlewares/Third_Party/MQTT/MQTTPacket/MQTTSubscribeServer.o \
./Middlewares/Third_Party/MQTT/MQTTPacket/MQTTUnsubscribeClient.o \
./Middlewares/Third_Party/MQTT/MQTTPacket/MQTTUnsubscribeServer.o 

C_DEPS += \
./Middlewares/Third_Party/MQTT/MQTTPacket/MQTTConnectClient.d \
./Middlewares/Third_Party/MQTT/MQTTPacket/MQTTConnectServer.d \
./Middlewares/Third_Party/MQTT/MQTTPacket/MQTTDeserializePublish.d \
./Middlewares/Third_Party/MQTT/MQTTPacket/MQTTFormat.d \
./Middlewares/Third_Party/MQTT/MQTTPacket/MQTTPacket.d \
./Middlewares/Third_Party/MQTT/MQTTPacket/MQTTSerializePublish.d \
./Middlewares/Third_Party/MQTT/MQTTPacket/MQTTSubscribeClient.d \
./Middlewares/Third_Party/MQTT/MQTTPacket/MQTTSubscribeServer.d \
./Middlewares/Third_Party/MQTT/MQTTPacket/MQTTUnsubscribeClient.d \
./Middlewares/Third_Party/MQTT/MQTTPacket/MQTTUnsubscribeServer.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/Third_Party/MQTT/MQTTPacket/%.o: ../Middlewares/Third_Party/MQTT/MQTTPacket/%.c Middlewares/Third_Party/MQTT/MQTTPacket/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 '-DMBEDTLS_CONFIG_FILE="mbedtls_config.h"' -DUSE_HAL_DRIVER -DDEBUG -DSTM32F429xx -c -I../Middlewares/Third_Party/LwIP/src/include/netif/ppp -I../Middlewares/Third_Party/MQTT -I../Middlewares/Third_Party/LwIP/src/include/compat/stdc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Middlewares/Third_Party/MQTT/MQTTPacket -I../Middlewares/Third_Party/LwIP/system/arch -I../Middlewares/Third_Party/LwIP/src/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/Third_Party/LwIP/src/include/compat/posix -I../Middlewares/Third_Party/LwIP/src/include/lwip -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/arpa -I../Middlewares/Third_Party/LwIP/system -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/LwIP/src/include/lwip/priv -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Middlewares/Third_Party/LwIP/src/include/lwip/prot -I../Middlewares/Third_Party/LwIP/src/include/lwip/apps -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/sys -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/mbedTLS/include/mbedtls -I../Inc -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/net -I../Middlewares/Third_Party/mbedTLS/include -I../Middlewares/Third_Party/LwIP/src/include/netif -O1 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

