.section .mod_printTimer_ASM, "a"
.syntax unified
.thumb
.cpu cortex-m0

.thumb_func
.global __call_printTimer_ASM
__call_printTimer_ASM:
    BL printTimer
