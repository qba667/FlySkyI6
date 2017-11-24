.section .mod_parseAcFrame_ASM, "a"
.syntax unified
.thumb
.cpu cortex-m0

.thumb_func
.global __call_parseAcFrame_ASM
__call_parseAcFrame_ASM:
    BL acData
