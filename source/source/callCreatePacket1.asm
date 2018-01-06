.section .mod_callCreatePacket1_ASM, "a"
.syntax unified
.thumb
.cpu cortex-m0

.thumb_func
.global __call_CreatePacket1_ASM
__call_CreatePacket1_ASM:
    BL getAuxChannel
