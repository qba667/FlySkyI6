.section .mod_extraChannels_ASM, "a"
.syntax unified
.thumb
.cpu cortex-m0

.thumb_func
.global __call_extraChannels_ASM
__call_extraChannels_ASM:
    BL createPacketCh1114
    ADD SP, SP, #0x8C
    POP {R4-R7,PC}
