.section .mod_printTimer1_ASM, "a"
.syntax unified
.thumb
.cpu cortex-m0

.thumb_func
.global __call_printTimer1_ASM
__call_printTimer1_ASM:
    BL printTimer
