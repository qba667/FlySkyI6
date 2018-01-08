.section .mod_voltTelemetry_ASM, "a"
.syntax unified
.thumb
.cpu cortex-m0

.thumb_func
.global __call_voltTelemetry_ASM
__call_voltTelemetry_ASM:
    CMP R0, #0x00
    BEQ voltADJ
    CMP R0, #0x03
    BEQ voltADJ
    B validSendor
voltADJ:
    MOV R0,R4
    BL adjustVoltage
    LDRB R0, [R4]
validSendor:
    CMP R0, #0xFE
