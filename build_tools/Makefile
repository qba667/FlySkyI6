AS =		arm-none-eabi-gcc
LD =		arm-none-eabi-ld
CPP = 		arm-none-eabi-cpp -P
OBJCOPY = 	arm-none-eabi-objcopy
PERL =		perl 
RM = 		rm
CP = 		cp
GREP =		grep

PUBLIC_FOLDER = ../public

ASFLAGS = 	-mcpu=cortex-m0plus -mthumb -g3
LDFLAGS =	
OBJS = 		flysky.o
LINKER_SCRIPTS = linkerINITIAL.ld linkerFINAL.ld

# All Target
all: flysky.hex

#Assemble
.s.o:
	$(AS) $(ASFLAGS) -o $@ -c $<
    
#Create ROM image    
%.hex: %.elf
	$(OBJCOPY) -O binary $<  $@	

# build the first custom linker scripts by running the template 
# through the c pre-processor - no crc
linkerINITIAL.ld:linker.ld
	$(CPP) -o $@ $<

# Link - first a phoney version, so we can calculate the CRC
phoney.elf: $(OBJS) linkerINITIAL.ld
	$(LD) $(LDFLAGS) -T linkerINITIAL.ld  -o $@ $(OBJS)

# calculate the CRC from the phoney linked binary
crc.txt:phoney.hex linker.ld crc16.pl
	$(PERL) crc16.pl phoney.hex > $@

# build the second custom linker scripts by running the template 
# through the c pre-processor - with crc
linkerFINAL.ld:linker.ld crc.txt
	$(CPP) -D__CRC_CALCULATED -o $@ $<
	
# Link - second using the CRC
flysky.elf: $(OBJS) linkerFINAL.ld
	$(LD) $(LDFLAGS) -T linkerFINAL.ld  -o $@ $(OBJS)
	
clean:
	$(RM) $(OBJS) linkerINITIAL.ld linkerFINAL.ld *.elf *.hex
	
	
	
