/* CANNOT BE USED DIRECTLY - MUST RUN THROUGH CPP
 * DEFINING __CRC_CALCULATED OR NOT
 */

/* Specify the memory areas */
MEMORY
{
  m_bootloader   (RX)  : ORIGIN = 0x00000000, LENGTH = 0x00001800
  m_rom          (RX)  : ORIGIN = 0x00001800, LENGTH = 0x0000E800
  m_ram          (RW)  : ORIGIN = 0x1FFFF800, LENGTH = 0x00002000
}

SECTIONS
{

  .bootloader :
  {
    . = 0x00000000;
    *(.bootloader)

  } > m_bootloader
  
  .text :
  {
     . = 0x00000000;
     KEEP(*(.startup))
     . = 0x00000F80;
     KEEP(*(.signature))
     . = 0x00001000;
     *(.text)
  } > m_rom =0x00

  .crc :
  {
    __CRC = .;
#ifdef __CRC_CALCULATED
     SHORT(
#include "crc.txt"     
     );
#endif
  } > m_rom =0xFF
  
}

