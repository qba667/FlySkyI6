################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../source/adc.c \
../source/alt.c \
../source/backlight.c \
../source/console.c \
../source/debug.c \
../source/flysky.c \
../source/font.c \
../source/lcd.c \
../source/main.c \
../source/mod.c \
../source/print.c \
../source/screen.c \
../source/uart.c

ASM_SRCS += \
../source/lmul.asm \
../source/configurePins.asm \
../source/extraChannels.asm \
../source/swe.asm \
../source/swb.asm \
../source/displaySensors1.asm \
../source/displaySensors2.asm \
../source/acFrame.asm \
../source/getSensorName.asm \
../source/getSensorName2.asm \
../source/printTimer.asm \
../source/printTimer1.asm \
../source/printTimer2.asm \
../source/formatSensorValue.asm \
../source/formatSensorValue2.asm \
../source/loadSettings.asm \
../source/callCreatePacket1.asm \
../source/callCreatePacket2.asm \
../source/voltTelemetry.asm


OBJS += \
./source/adc.o \
./source/alt.o \
./source/backlight.o \
./source/console.o \
./source/debug.o \
./source/flysky.o \
./source/font.o \
./source/lcd.o \
./source/lmul.o \
./source/main.o \
./source/mod.o \
./source/print.o \
./source/screen.o \
./source/uart.o \
./source/configurePins.o \
./source/extraChannels.o \
./source/swe.o \
./source/swb.o \
./source/displaySensors1.o \
./source/displaySensors2.o \
./source/acFrame.o \
./source/getSensorName.o \
./source/getSensorName2.o \
./source/printTimer.o \
./source/printTimer1.o \
./source/printTimer2.o \
./source/formatSensorValue.o \
./source/formatSensorValue2.o \
./source/loadSettings.o \
./source/callCreatePacket1.o \
./source/callCreatePacket2.o \
./source/voltTelemetry.o

C_DEPS += \
./source/adc.d \
./source/alt.d \
./source/backlight.d \
./source/console.d \
./source/debug.d \
./source/flysky.d \
./source/font.d \
./source/lcd.d \
./source/main.d \
./source/mod.d \
./source/print.d \
./source/screen.d \
./source/uart.d

ASM_DEPS += \
./source/lmul.d \
./source/configurePins.d \
./source/extraChannels.d \
./source/swe.d \
./source/swb.d \
./source/displaySensors1.d \
./source/displaySensors2.d \
./source/acFrame.d \
./source/getSensorName.d \
./source/getSensorName2.d \
./source/printTimer.d \
./source/printTimer1.d \
./source/printTimer2.d \
./source/formatSensorValue.d \
./source/formatSensorValue2.d \
./source/loadSettings.d \
./source/callCreatePacket1.d \
./source/callCreatePacket2.d \
./source/voltTelemetry.d

# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0plus -mthumb -Os $(PARAMS) -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g -D"CPU_MKL16Z64VLH4" -I../startup -I../board -I../utilities -I../CMSIS -I../drivers -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

source/%.o: ../source/%.asm
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU Assembler'
	arm-none-eabi-gcc -mcpu=cortex-m0plus -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '
