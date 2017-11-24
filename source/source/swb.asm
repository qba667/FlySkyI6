.section .mod_swB_ASM, "a"
.syntax unified
.thumb
.cpu cortex-m0

.thumb_func
.global __call_swB_ASM
__call_swB_ASM:
    BL swBasADC
