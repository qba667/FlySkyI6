.section .mod_swE_ASM, "a"
.syntax unified
.thumb
.cpu cortex-m0

.thumb_func
.global __call_swE_ASM
__call_swE_ASM:
    BL swEHandling
