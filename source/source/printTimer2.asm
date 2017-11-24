.section .mod_printTimer2_ASM, "a"
.syntax unified
.thumb
.cpu cortex-m0

.thumb_func
.global __call_printTimer2_ASM
__call_printTimer2_ASM:
    BL printTimer
