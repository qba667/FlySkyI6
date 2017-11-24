.section .mod_displaySensors1_ASM, "a"
.syntax unified
.thumb
.cpu cortex-m0

.thumb_func
.global __call_displaySensors1_ASM
__call_displaySensors1_ASM:
    BL displaySensors
