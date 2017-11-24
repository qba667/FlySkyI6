.section .mod_formatSensorValue_ASM, "a"
.syntax unified
.thumb
.cpu cortex-m0

.thumb_func
.global __call_formatSensorValue_ASM
__call_formatSensorValue_ASM:
    BL formatSensorValue
