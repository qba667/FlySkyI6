# FlySkyI6
FW disassembly and modifications of FlySky I6.

ASM can be compiled with arm-none-eabi toolchain.

ASM compilation:

`arm-none-eabi-as.exe -o fsi6.o fsi6.asm -mcpu=cortex-m0plus -mthumb -g3`

`arm-none-eabi-objcopy.exe -O binary fsi6.o fsi6.bin`

Docummented disassembly can be found in ASM/10ch.0x0000

Documentation was created with use modified script 

http://magiclantern.wikia.com/wiki/GPL_Tools/disasm.py

It is also to possibile to compile some parts of modifyed firmware with gcc (all c files are in source directory).
The project was oryginaly compiled with Freescale Kinetics. 

Compiled elements must be replaced later in oryginal file (addresses are described in source/MKL16Z64xxx4_flash.ld)

Command used for compilation (extraced from Freescale Kinetics)

`arm-none-eabi-gcc -mcpu=cortex-m0plus -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g -D"CPU_MKL16Z64VLH4" -I../startup -I../board -I../utilities -I../CMSIS -I../drivers -std=gnu99 -MMD -MP -MF"source/file.d" -MT"source/file.o" -c -o "source/file.o" "../source/file.c"`
