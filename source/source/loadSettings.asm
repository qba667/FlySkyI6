.section .mod_loadSettings_ASM, "a"
.syntax unified
.thumb
.cpu cortex-m0

.thumb_func
.global __call_loadSettings_ASM
__call_loadSettings_ASM:
    BL loadSettings
