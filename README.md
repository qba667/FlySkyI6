# FlySkyI6
FW disassembly and modifications of FlySky I6.

ASM can be compiled with arm-none-eabi toolchain.

ASM compilation:
arm-none-eabi-as.exe -o fsi6.o fsi6.asm -mcpu=cortex-m0plus -mthumb -g3
arm-none-eabi-objcopy.exe -O binary fsi6.o fsi6.bin
