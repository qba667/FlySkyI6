.section .mod_configurePins_ASM, "a"
.syntax unified
.thumb
.cpu cortex-m0

.thumb_func
.global __call_configurePins_ASM
__call_configurePins_ASM:
    BL configurePINS2
