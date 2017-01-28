.align  2
.global main
.thumb
main:
BL				timeCheck
BX 				lr
.org	0x19F8
sprintf:
PUSH    {LR}
POP    {PC}
.org	0x2534
getKey:
PUSH    {LR}
POP    {PC}
.org	0x25B4
setupDMAandSendPacket:
PUSH    {LR}
POP    {PC}
.org	0x25BC
LCD_updateCALL:
PUSH    {LR}
POP    {PC}
.org	0x3E10
readEeprom:
PUSH    {LR}
POP    {PC}
.org	0x3E84
writeEeprom:
PUSH    {LR}
POP    {PC}
.org	0x505C
displayTextAt:
PUSH    {LR}
POP    {PC}
.org	0x6004
settingsValidation:
PUSH    {LR}
POP    {PC}
.org	0x6D5C
displayPageHeader:
PUSH    {LR}
POP    {PC}
.org	0xB4A0
displayGFX:
PUSH    {LR}
POP    {PC}
.org	0xF740
saveTimerSettings:
PUSH    {LR}
POP    {PC}
.org	0xF81C
timeCheck:
PUSH    		{R4-R7, LR}
LDR            	R7, =0x20000274
LDR             R6, =0xE000E018
LDR             R4, [R6]
LDR             R5, [R7]
CMP				R5, #0
BEQ				checkTimer
CMP				R4, R5
BGT				overflow
SUB				R5, R5, R4
B				checkTime
overflow:
LDR            	R3, =0xFFFFFF
SUB				R3, R4
ADD				R3, R5
MOV				R5, R3
checkTime:
LDR            	R3, =3200000
CMP				R5, R3
BLT				displayBuffer
SUB				R5, R5, R3
ADD				R5, R5, R4
CMP				R5, R3
STR				R5,	[R7]
LDRH			R4, [R7, #0x4]
ADD				R4, #1
STRH			R4, [R7, #0x4]
B				formatTime
checkTimer:
BL				timerActive
CMP				R0, #0
BEQ				displayDefault
LDR            	R3, =3200000
CMP				R4, R3
BGT				storeFirstCheck
LDR            	R3, =0xFFFFFF
ADD				R4, R4, R3
storeFirstCheck:
STR				R4,	[R7]
formatTime:
MOVS 			R0, R7
ADD				R0, #0xA
LDR				R1, =format
MOVS			R2, #0
MOVS			R3, #0
LDRH			R4, [R7, #0x4]
formatHours:
LDR				R5, =3600
CMP				R4, R5
BLT 			formatMinutes
ADD				R2, #1;
SUB				R4, R5;
B				formatHours
formatMinutes:
MOVS			R5, #60
CMP				R4, R5
BLT 			formatSeconds
ADD				R3, #1;
SUB				R4, R5;
B				formatMinutes
formatSeconds:
MOVS			R5, #0x3F
AND				R4, R4, R5
AND				R3, R3, R5
AND				R2, R2, R5
PUSH			{R4}
BL				sprintf
POP				{R4}
B				displayBuffer
displayDefault:
LDR 			R0, =defaultTimer
B				sendToLCD
displayBuffer:
MOVS 			R0, R7
ADD				R0, #0xA
sendToLCD:
MOVS			R1, #1
MOVS			R2, #9
MOVS			R3, #0
BL				displayTextAt
POP				{R4-R7,PC}
timerActive:
PUSH    		{R4-R7, LR}
LDR            	R7, =0x20000274
LDR            	R6, =0x1FFFFD9D
ADD				R7, #6
LDRH			R4, [R7, #2]
LDR				R2, =0xFFFF
CMP				R4, R2
BEQ				timerInActive
LDR				R2, =1000
CMP				R4, R2
BLT				timerInActive
LDR				R2, =2000
CMP				R4, R2
BGT				timerInActive
LDRH			R5, [R7, #0]
MOV				R2, #0xF
AND				R5, R5, R2
CMP				R5, #0
BLE				timerInActive
CMP				R5, #10
BGT				timerInActive
SUB				R7, #6
SUB				R5, #1
MOV				R3, #2
MUL				R5, R5, R3
ADD				R6, R5
LDRB			R2, [R6, #0]
LDRB			R1, [R6, #1]
LSL				R1, R1,#8
ORR				R2, R1, R2
CMP				R2, R4
BGE				channelOK
timerInActive:
MOV				R0, #0
B				retunTimerActive
channelOK:
MOV				R0, #1
retunTimerActive:
POP				{R4-R7,PC}

configPage:
PUSH    		{R4-R7, LR}
LDR            	R6, =0x20000274
ADD				R6, #6
LDRH			R5, [R6]
LDRH			R4, [R6,#2]
SUB				R6, #6
MOV				R2, #0xF
AND				R5, R5, R2
displayLoop:
BL				setupDMAandSendPacket
LDR				R0,=timerPageName
BL				displayPageHeader
MOV				R7, #0
LDR				R2, =0xFFFF
CMP				R4, R2
BEQ				timerInactiveDisplay
LDR				R2, =1000
CMP				R4, R2
BLT				timerInactiveDisplay
LDR				R2, =2000
CMP				R4, R2
BGT				timerInactiveDisplay
CMP				R5, #0
BLE				timerInactiveDisplay
CMP				R5, #10
BGT				timerInactiveDisplay
LDR				R0,=initialTimerVal
MOVS			R1, #0xA
MOVS			R2, #0xF
BL				displayTextAt
MOVS 			R0, R6
ADD				R0, #0xC
LDR            	R1, =statrtCond
MOVS            R2, R5
MOVS            R3, R4
BL				sprintf
MOVS 			R0, R6
ADD				R0, #0xC
MOVS			R1, #0xA
MOVS			R2, #0x1E
BL				displayTextAt
B				updateLCD
timerInactiveDisplay:
MOV				R7, #1
LDR 			R0, =off
MOVS			R1, #0xA
MOVS			R2, #0x1E
BL				displayTextAt
updateLCD:
BL				LCD_updateCALL
BL				getKey
CMP             R0, #9
BEQ				changeCH
CMP             R0, #23
BEQ				changeCH
CMP             R0, #8
BEQ				changeValSlow
CMP             R0, #24
BEQ				changeValFast
CMP				R0, #10
BEQ				leaveLoop
CMP				R0, #11
BEQ				leaveLoop
CMP				R0, #22
BEQ				resetTimer
CMP				R0, #25
BEQ				save
B				displayLoop
resetTimer:
MOV				R0, #0
STR				R0,	[R6]
STRH			R0,	[R6, #0x4]
B				leaveLoop
save:
CMP				R7, #1
BNE				continueSave
MOV				R5, #3
LDR				R4, =0xFFFF
continueSave:
ADD				R6, #6
STRH			R5, [R6]
STRH			R4, [R6,#2]
SUB				R6, #6
BL				saveTimerSettings
B				leaveLoop
changeValSlow:
ADD				R4, #10
B				checkVal
changeValFast:
ADD				R4, #100
checkVal:
CMP				R7, #1
BEQ				defaultVal
LDR				R2, =2000
CMP				R4, R2
BLE				displayLoop
LDR				R4, =0xFFFF
B				displayLoop
changeCH:
ADD				R5, #1
CMP				R7, #1
BEQ				defaultVal
CMP				R5, #10
BLE				displayLoop
MOV				R5, #1
B				displayLoop
defaultVal:
MOV				R5, #3
LDR				R4, =1000
B				displayLoop
leaveLoop:
POP				{R4-R7,PC}
.balign 4
off: .asciz "Off"				
initialTimerVal: .asciz "Start when:"
timerPageName: .asciz "Timer"
format: .asciz "%02u:%02u:%02u"
defaultTimer: .asciz "00:00:00"
statrtCond: .asciz "CH: %u > %u%"

