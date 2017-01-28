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
.org	0x5374
getSensor:
PUSH    {LR}
POP    {PC}
.org	0xF66E
timeCheck:
B		jumphere
NOP
comp7:
CMP             R0, #7
.org	0xF6EC
loc_F6EC:
MOV		R0,#0
returnChannelValue:
POP     {R3-R7,PC}
.org	0xF758
jumphere:
CMP             R0, #0
BEQ             loc_F6EC
CMP				R0, #11
BNE				comp7
MOV				R2, #0
MOV				R1, #0
MOV				R0, #254
BL				getSensor
LDR            	R1, =0x8000
MOV				R2, #50
CMP				R0, R1
BEQ				minval
MOV				R1, #200
CMP				R0, R2
BGT				above50
SUB				R0, R2, R0
MUL				R0, R1
B				returnChannelValue
above50:
SUB				R0, R0, R2
MUL				R0, R1
NEG				R0, R0
B				returnChannelValue
minval:		
LDR	    		R0, =-10000
NEG				R0, R0
LSL				R0, #3
ADD				R3, R0
B				returnChannelValue



