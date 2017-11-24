.section .mod_displaySensors2_ASM, "a"
.syntax unified
.thumb
.cpu cortex-m0

.thumb_func
.global __call_displaySensors2_ASM
__call_displaySensors2_ASM:
    BL displaySensors
