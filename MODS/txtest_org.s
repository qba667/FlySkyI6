.align  2
.global main
.thumb
main:
BL				readTimerSettings
BL				timeCheck
BX 				lr
.org	0x19F8
sprintf:
PUSH    {LR}
POP    {PC}
.org	0x3E10
readEeprom:
PUSH    {LR}
POP    {PC}
.org	0x505C
displayTextAt:
PUSH    {LR}
POP    {PC}
.org	0xF81C
timeCheck:
PUSH    		{R4-R7, LR}
LDR            	R7, =0x20000294
LDR             R6, =0xE000E000
LDR             R6, [R6,#0x18]
LDR             R5, [R7]
CMP				R5, #0
BEQ				initSettings
SUB				R6, R6, R5
LDR            	R4, =8000000
countTicks:
CMP				R6, R4
BLT				displayTimer
SUB				R6, R6, R4
LDRH			R5, [R7, #0xA]
SUB				R5, R5, #1
STRH			R5, [R7, #0xA]
LDR             R3, =0xE000E000
LDR             R3, [R3,#0x18]
STR				R3,	[R7, #0]
B				countTicks
initSettings:
LDR				R1, =0x20000294
LDR				R2, =0xFF
STRH			R2, [R1, #0xA]
ADD				R1, #0x16
STRH			R2, [R1]
MOV				R2, #2
ADD				R1, #0x2
STRB			R2, [R1]
LDR				R2, =0x5DC
ADD				R1, #1
STRH			R2, [R1]
LDR             R3, =0xE000E000
LDR             R3, [R3,#0x18]
STR				R3,	[R7, #0]
displayTimer:
MOVS 			R0, R7
ADD				R0, #0xC
LDR				R1, =format
MOVS			R2, #0
MOVS			R3, #0
LDRH			R5, [R7, #0xA]
formatHours:
LDR				R6, =3600
CMP				R5, R6
BLT 			formatMinutes
ADD				R2, #1;
SUB				R5, R6;
B				formatHours
formatMinutes:
MOVS			R6, #60
CMP				R5, R6
BLT 			formatSeconds
ADD				R3, #1;
SUB				R5, R6;
B				formatMinutes
formatSeconds:
MOVS			R4, #0x3F
AND				R5, R5, R4
AND				R2, R2, R4
AND				R3, R3, R4
PUSH			{R5}
BL				sprintf
POP				{R5}
MOVS 			R0, R7
ADD				R0, #0xC
MOVS			R1, #40
MOVS			R2, #9
MOVS			R3, #0
BL				displayTextAt
POP				{r4-r7,PC}
format:    .ascii "%03u:%02u:%02u\0\0"
readTimerSettings:
PUSH    {R4, LR}
LDR				R4, =0x20000294
MOV				R0, R4
ADD				R0, #0x16
MOV				R1, #42
MOV				R2, #10
BL				readEeprom
MOV				R1, R4
ADD				R1, #0x16
LDRH			R1, [R3]
MOV				R2, R4
STRH			R1, [R2, #0xA]
POP    {R4, PC}
timerActive:
PUSH    {LR}
LDR				R3, =0x20000294
MOV				R0, R3
ADD				R0, #0x18
MOV				R1, R0
ADD				R1, #1
LDRH			R1, [R1]
LDRB 			R0, [R0]
MOV				R2, #2
MUL				R0, R0, R2
LDR				R3, =0x1FFFFD9D
ADD				R3, R0
LDRH			R2, [R3]
CMP				R2, R1
BGE				channelOK
LDR				R3, =0x20000294
MOV				R1, R3
ADD				R1, #0x16
LDRH			R1, [R1]
MOV				R2, R3
ADD				R2, #0x10
STRH			R1, [R2]
MOV				R0, #0
BEQ				retunTimerActive
channelOK:
MOV				R0, #1
retunTimerActive:
POP    {PC}

.org	0xF9A0, 0xFF
