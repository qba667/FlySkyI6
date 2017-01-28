.align  2
.global main
.thumb
main:
BL				sensorMethod
BX 				lr
.org	0x53CC
appendUnits:
NOP
.org	0x6790
sensorMethod:
MOV		R0, R4
BL		getSensorIDByIndex
MOV		R7, R0
LDRB	R0, [R7]
CMP		R0, #0xFF
BEQ		noSensor
LDRB	R1, [R7,#1]
MOV     R2, R6
BL		appendUnits
LDRB	R0, [R7]
LDR     R1, =4
BL      getSensorNameOwn
MOVS    R2, R0
LDRB    R3, [R7,#1]
.org	0x6844
noSensor:
NOP
test:
.byte	0xFA
.org	0xF794
getSensorIDByIndex:
PUSH {LR}
CMP		R0, #3
BGE		userSensor
LDR		R1, =fixedSensors
B		getSensorID
userSensor:
SUB		R0, #3
LDR		R1, =0x2000002C
LDR		R1, [R1]
ADD		R1, #100
ADD		R1, #29
getSensorID:
LSL		R0, R0, #1
ADD		R0, R1, R0
POP {PC}
.org	0xFB30
getSensorNameOwn:
NOP
.balign 4
fixedSensors:
.byte	0xFA, 0x0, 0x7C, 0x0, 0x0,0x0





