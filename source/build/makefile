################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include utilities/subdir.mk
-include startup/subdir.mk
-include source/subdir.mk
-include drivers/subdir.mk
-include board/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
fs-i6.bin \

SECONDARY_SIZE += \
fs-i6.siz \

SECONDARY_FLASH_SWE += \
fs-i6-swe.bin \

SECONDARY_SIZE_SWE += \
fs-i6-swe.siz \

PARAMS = 

# All Target
all: fs-i6.elf secondary-outputs 
all-swe: fs-i6-swe.elf secondary-outputs-swe
# Tool invocations

fs-i6.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Cross ARM C++ Linker'
	arm-none-eabi-g++ -mcpu=cortex-m0plus -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g -T "../MKL16Z64xxx4_flash.ld" -Xlinker --gc-sections -Wl,-Map,"fs-i6.map" --specs=nano.specs -specs=nosys.specs -Xlinker -z -Xlinker muldefs -o "fs-i6.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
fs-i6-swe.elf: PARAMS = -DSWE
fs-i6-swe.elf: $(OBJS) $(USER_OBJS) 
	@echo 'Building target: $@'
	@echo 'Invoking: Cross ARM C++ Linker'
	arm-none-eabi-g++ -mcpu=cortex-m0plus -mthumb -Os $(PARAMS) -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g -T "../MKL16Z64xxx4_flash.ld" -Xlinker --gc-sections -Wl,-Map,"fs-i6-swe.map" --specs=nano.specs -specs=nosys.specs -Xlinker -z -Xlinker muldefs -o "fs-i6-swe.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
fs-i6.bin: fs-i6.elf
	@echo 'Invoking: Cross ARM GNU Create Flash Image'
	arm-none-eabi-objcopy --update-section .interrupts=org.bin "fs-i6.elf"
	arm-none-eabi-objcopy -j .interrupts -j .mod_* -O binary "fs-i6.elf"  "fs-i6.bin"
	#python flysky2.py fs-i6.bin fs-i6
	python3 flysky3.py fs-i6.bin fs-i6
	@echo 'Finished building: $@'
	@echo ' '
fs-i6-swe.bin: fs-i6-swe.elf
	@echo 'Invoking: Cross ARM GNU Create Flash Image'
	arm-none-eabi-objcopy --update-section .interrupts=org.bin "fs-i6-swe.elf"
	arm-none-eabi-objcopy -j .interrupts -j .mod_* -O binary "fs-i6-swe.elf"  "fs-i6-swe.bin"
	#python flysky2.py fs-i6.bin fs-i6
	python3 flysky3.py fs-i6-swe.bin fs-i6-swe
	@echo 'Finished building: $@'
	@echo ' '
	
fs-i6.siz: fs-i6.elf
	@echo 'Invoking: Cross ARM GNU Print Size'
	arm-none-eabi-size --format=berkeley "fs-i6.elf"
	arm-none-eabi-size --format=sysv "fs-i6.elf" -x
	@echo 'Finished building: $@'
	@echo ' '
	
fs-i6-swe.siz: fs-i6-swe.elf
	@echo 'Invoking: Cross ARM GNU Print Size'
	arm-none-eabi-size --format=berkeley "fs-i6-swe.elf"
	arm-none-eabi-size --format=sysv "fs-i6-swe.elf" -x
	@echo 'Finished building: $@'
	@echo ' '
# Other Targets
clean:
	-$(RM) $(SECONDARY_SIZE)$(C++_DEPS)$(OBJS)$(C_DEPS)$(ASM_DEPS)$(CC_DEPS)$(SECONDARY_FLASH)$(CPP_DEPS)$(CXX_DEPS)$(C_UPPER_DEPS)$(S_UPPER_DEPS)$(SECONDARY_SIZE_SWE)$(SECONDARY_SIZE_SWE) fs-i6.elf fs-i6-swe.elf
	-@echo ' '

secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_SIZE)
secondary-outputs-swe: $(SECONDARY_FLASH_SWE) $(SECONDARY_SIZE_SWE)
.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
