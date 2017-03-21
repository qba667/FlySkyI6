#include "uart.h"
#include "MKL16Z4.h"
#include "fsl_common.h"
#include "fsl_dma.h"
#include "fsl_dmamux.h"
#include "fsl_uart.h"
const char testdata[]={"test"};

void sendDataDMA3(const char *buf, uint32_t length){

	SIM->SCGC6 |= SIM_SCGC6_DMAMUX_MASK;
	SIM->SCGC7 |= SIM_SCGC7_DMA_MASK;

	DMAMUX0->CHCFG[3] = 0x00;
	DMAMUX0->CHCFG[3] |= 0x7;

	DMA0->DMA[3].DCR &= ~(DMA_DCR_SSIZE_MASK | DMA_DCR_DSIZE_MASK);
	DMA0->DMA[3].DAR = (uint32_t)&UART2->D;
	DMA0->DMA[3].DCR = DMA_DCR_EINT_MASK|DMA_DCR_CS_MASK|DMA_DCR_SINC_MASK|DMA_DCR_SSIZE(1)|DMA_DCR_DSIZE(1);
	DMA0->DMA[3].SAR = (uint32_t) buf;
	DMA0->DMA[3].DSR_BCR = DMA_DSR_BCR_BCR(length);
	DMA0->DMA[3].DCR |= DMA_DCR_ERQ_MASK;
	//DMA0->DMA[3].DCR |= DMA_DCR_START_MASK;
	//disabled for debug
	//NVIC->ISER[0U] = (uint32_t)(1UL << 3);
	DMAMUX0->CHCFG[3] |= DMAMUX_CHCFG_ENBL_MASK;

}
void DMA3_IRQHandler(void){
	volatile uint32_t* dma_dsr_bcr0_reg = &DMA0->DMA[3].DSR_BCR;
	uint32_t dma_dsr_bcr0_val = *dma_dsr_bcr0_reg;

	if (((dma_dsr_bcr0_val & DMA_DSR_BCR_DONE_MASK) == DMA_DSR_BCR_DONE_MASK)
	     | ((dma_dsr_bcr0_val & DMA_DSR_BCR_BES_MASK) == DMA_DSR_BCR_BES_MASK)
	     | ((dma_dsr_bcr0_val & DMA_DSR_BCR_BED_MASK) == DMA_DSR_BCR_BED_MASK)
	     | ((dma_dsr_bcr0_val & DMA_DSR_BCR_CE_MASK) == DMA_DSR_BCR_CE_MASK))
	{
		DMA0->DMA[3].DSR_BCR |= DMA_DSR_BCR_DONE_MASK;                //Clear Done bit
		DMA0->DMA[3].DSR_BCR= DMA_DSR_BCR_BCR(0);      //Reset BC
		DMA0->DMA[3].DCR &= ~DMA_DCR_ERQ_MASK;
	}

	DMAMUX0->CHCFG[3] &= ~(DMAMUX_CHCFG_ENBL_MASK);
	NVIC->ISER[0U] = (uint32_t)(1UL << 3);
	sendDataDMA3(testdata, 10);
}

//#define UART_CLOCK /()
void rxTest2(void){
	SIM->SCGC4 |=  SIM_SCGC4_UART2_MASK;
	SIM->SCGC5 |=  SIM_SCGC5_PORTE_MASK;
	PORTE->PCR[16] |= 3 << 8; //4004D040
	PORTE->PCR[17] |= 3 << 8; //4004D044

	// 48000000 / (115200 * 16) = 26 115384
	// 48000000 / (9600 * 16) = 312
	//uint32_t targetRate = 9600; //115200
	//uint32_t srcClock_Hz = 48000000;

	/* Calculate the baud rate modulo divisor, sbr*/
	//uint16_t sbr = 48000000 / (targetRate * 16); = 26
	//uint16_t sbr = 11;
	uint16_t sbr = 26;
	UART2->C2 &= ~(UART_C2_TE_MASK | UART_C2_RE_MASK);
	UART2->BDH = (UART2->BDH & ~UART_BDH_SBR_MASK) | (uint8_t)(sbr >> 8);
	UART2->BDL = (uint8_t)sbr;
	/* Set bit count and parity mode. */
	UART2->C1 = UART2->C1 & ~(UART_C1_PE_MASK | UART_C1_PT_MASK | UART_C1_M_MASK);
	UART2->BDH = (UART2->BDH & ~UART_BDH_SBNS_MASK) | UART_BDH_SBNS((uint8_t)0);

	UART2->C2 |= UART_C2_TE_MASK | UART_C2_TIE_MASK;
	UART2->C4 |= UART_C4_TDMAS_MASK;
	//disabled for debug
	sendDataDMA3(testdata, (uint32_t)2U);
}
