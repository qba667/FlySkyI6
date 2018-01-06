.section .mod_callCreatePacket2_ASM, "a"
.syntax unified
.thumb
.cpu cortex-m0

.thumb_func
.global __call_CreatePacket2_ASM
__call_CreatePacket2_ASM:
    BL getAuxChannel
