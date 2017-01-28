.align  2
.global main
.thumb
main:
BL				init
BL				saveTimer
BX 				lr
.align 2
.org	0x3E10
readEeprom:
PUSH    {LR}
POP    {PC}
.org	0x3E84
writeEeprom:
PUSH    {LR}
POP    {PC}
.org	0x6004
readSettings:
PUSH    {LR}
POP    {PC}
.org    0xf7ac
init:
PUSH		{LR}
BL				readSettings
LDR            	R0, =0x20000274
ADD				R0, #6
MOV				R1, #0x30
MOV				R2, #2
BL				readEeprom
POP			{PC}
saveTimer:
PUSH		{LR}
LDR            	R0, =0x20000274
ADD				R0, #6
MOV				R1, #0x30
MOV				R2, #2
BL				writeEeprom
POP			{PC}


