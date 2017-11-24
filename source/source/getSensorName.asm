.section .mod_getSensorName_ASM, "a"
.syntax unified
.thumb
.cpu cortex-m0

.thumb_func
.global __call_getSensorName_ASM
__call_getSensorName_ASM:
    BL getSensorName
