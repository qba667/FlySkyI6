.align  2
.global main
.thumb
main:
BL				getSensorName
BL				ms
BL				call1
BL				call2
BL				addedSensor
BX 				lr
.org	0x19F8
sprintf:
PUSH    {LR}
POP    {PC}
.org	0x2f9c
getSensorName:
PUSH 	{R1}
LDR		R6, =200
BL		timeCheck
POP		{R1}
MOV		R3, R0
.org	0x376A
call1:
BL		defaultSensorName
getSensorName1:
.org	0x5374
getSensor:
PUSH    {LR}
POP    {PC}
.org	0x5416
addedSensor:
MOV		R1, R6
BL		formatSensor
.org	0x671C
call2:
BL		defaultSensorName
.org	0x67ac
ms:
BL		timeCheck
.org	0xFB30
timeCheck:
PUSH {LR}
CMP		R0, #0
BLT		sensorUnknown
CMP		R0, #0xFA
BGE		sensorFA
CMP		R0, #0x7C
BGE		sensor7C
CMP		R0, #0xF
BGT		sensorUnknown
B		shiftAndSet
sensorFA:
SUB		R0, #0xFA
ADD		R1, #0xA8
B		shiftAndSet
sensor7C:
SUB		R0, #0x7C
ADD		R1, #0x88
B		shiftAndSet
shiftAndSet:
LSL		R0, R0, #3
ADD		R0, R1, R0
B		returnSensorName
sensorUnknown:
MOV		R0, #0xD8
ADD		R0, R1, R0
returnSensorName:
POP {PC}
defaultSensorName:
PUSH {R1, LR}
LDR		R1, =0xFA50
BL	timeCheck
POP {R1, PC}
.org	0xFB6C
formatSensor:
PUSH {LR}
CMP		R1, #0x4
BEQ		sensor4
CMP		R1, #0x5
BEQ		sensor5
CMP		R1, #0x6
BEQ		sensor6
CMP		R1, #0x7
BEQ		sensor7
CMP		R1, #0x8
BEQ		sensor8
CMP		R1, #0x9
BEQ		sensor9
CMP		R1, #0xA
BEQ		sensorA
CMP		R1, #0xB
BEQ		sensorB
CMP		R1, #0xC
BEQ		sensorC
CMP		R1, #0xD
BEQ		sensorD
CMP		R1, #0xE
BEQ		sensorE
CMP		R1, #0xF
BEQ		sensorF
CMP		R1, #0xFA
BEQ		SNRSensor
BGT		signalSensor
B		defSensor
POP {PC}
sensor4:
B		defSensor
NOP
NOP
NOP
NOP
NOP
NOP
NOP
sensor5:
B		defSensor
NOP
NOP
NOP
NOP
NOP
NOP
NOP
sensor6:
B		defSensor
NOP
NOP
NOP
NOP
NOP
NOP
NOP
sensor7:
B		defSensor
NOP
NOP
NOP
NOP
NOP
NOP
NOP
sensor8:
B		defSensor
NOP
NOP
NOP
NOP
NOP
NOP
NOP
sensor9:
B		defSensor
NOP
NOP
NOP
NOP
NOP
NOP
NOP
sensorA:
B		defSensor
NOP
NOP
NOP
NOP
NOP
NOP
NOP
sensorB:
B		defSensor
NOP
NOP
NOP
NOP
NOP
NOP
NOP
sensorC:
B		defSensor
NOP
NOP
NOP
NOP
NOP
NOP
NOP
sensorD:
B		defSensor
NOP
NOP
NOP
NOP
NOP
NOP
NOP
sensorE:
B		defSensor
NOP
NOP
NOP
NOP
NOP
NOP
NOP
sensorF:
B		defSensor
NOP
NOP
NOP
NOP
NOP
NOP
NOP
SNRSensor:
LDR		R1, =dBsensor
B		callSprintf
signalSensor:
LDR		R1, =dBmSensor
B		callSprintf
defSensor:
LDR		R1, =defaultSensor
B		callSprintf
callSprintf:
BL		sprintf
POP {PC}
.balign 4
dBsensor:  .asciz "%udB"
dBmSensor: .asciz "-%udBm"
defaultSensor: .asciz "%u"



