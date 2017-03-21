/*
    Copyright 2016 fishpepper <AT> gmail.com

    This program is free software: you can redistribute it and/ or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http:// www.gnu.org/licenses/>.

    author: fishpepper <AT> gmail.com
*/

#include "adc.h"
#include "debug.h"
#include "console.h"
#include "fsl_adc16.h"
#include "fsl_port.h"
#include "delay.h"
static uint16_t adc_data[ADC_CHANNEL_COUNT];

static uint16_t adc_battery_voltage_raw_filtered;

static const uint8_t adac_channels[] = {
	7,
	6,
	5,
	11,
	3,
	7,
	4,
	14,
	15,
};


void adc_init(void){
	debug("adc: init\n"); debug_flush();
	adc_battery_voltage_raw_filtered = 0;

    for (int i = 0; i < ADC_CHANNEL_COUNT; i++) {
        adc_data[i] = 0;
    }
    adc_init_internal();
}

void adc_init_internal(void){
	CLOCK_EnableClock(kCLOCK_Adc0);
	CLOCK_EnableClock(kCLOCK_PortC);
	CLOCK_EnableClock(kCLOCK_PortD);
	CLOCK_EnableClock(kCLOCK_PortE);
	PORT_SetPinMux(PORTC, 0, kPORT_PinDisabledOrAnalog); /* PORTC0 (pin 43) is configured as ADC0_SE14 */
	PORT_SetPinMux(PORTC, 1, kPORT_PinDisabledOrAnalog); /* PORTC1 (pin 44) is configured as ADC0_SE15 */
	PORT_SetPinMux(PORTC, 2, kPORT_PinDisabledOrAnalog); /* PORTC2 (pin 45) is configured as ADC0_SE11 */
	PORT_SetPinMux(PORTD, 1, kPORT_PinDisabledOrAnalog); /* PORTD1 (pin 58) is configured as ADC0_SE5b */
	PORT_SetPinMux(PORTD, 5, kPORT_PinDisabledOrAnalog); /* PORTD5 (pin 62) is configured as ADC0_SE6b */
	PORT_SetPinMux(PORTD, 6, kPORT_PinDisabledOrAnalog); /* PORTD6 (pin 63) is configured as ADC0_SE7b */
	PORT_SetPinMux(PORTE, 22, kPORT_PinDisabledOrAnalog); /* PORTE22 (pin 11) is configured as ADC0_SE3 */
	PORT_SetPinMux(PORTE, 23, kPORT_PinDisabledOrAnalog); /* PORTE23 (pin 12) is configured as ADC0_SE7a */
	PORT_SetPinMux(PORTE, 29, kPORT_PinDisabledOrAnalog); /* PORTE29 (pin 17) is configured as ADC0_SE4b */

	adc16_config_t config = {
		kADC16_ReferenceVoltageSourceVref,
		kADC16_ClockSourceAlt0,
		false,
		kADC16_ClockDivider2,
		kADC16_Resolution12or13Bit,
		kADC16_LongSampleCycle6,
		true,
		false,
		true //not sure
	};
	//ADC16_Init(ADC0, &config);
	//ADC16_SetChannelMuxMode(ADC0, kADC16_ChannelMuxB);
	ADC0->CFG1 = 52;
	ADC0->CFG2 = 22;
	ADC0->SC2 = 0;
	//ADC0->SC3 = 133;
	ADC0->SC3 = 5;
	ADC16_DoAutoCalibration(ADC0);
}

void adc_test(void){
	for(int i =0; i < ADC_CHANNEL_COUNT; i++){
		if(i == 5) ADC0->CFG2 &= 0xFFFFFFEF; //select ADxxa
		else ADC0->CFG2 |= 0x10; //select ADxxb
		ADC0->SC1[0] = (adac_channels[i] | 0x40) & ADC_SC1_ADCH_MASK;
		//Write to SC1A to start conversion
		while (ADC0->SC2 & ADC_SC2_ADACT_MASK);
		while (!(ADC0->SC1[0] & ADC_SC1_COCO_MASK));
		adc_data[i] = ADC0->R[0];
	}
}

void adc_test2(void) {

        console_clear();
        debug("ADC TEST");
        debug_put_newline();
        //debug_put_fixed2(adc_get_battery_voltage());
        //debug(" V\n");
        uint32_t i;
        for (i = 0; i < ADC_CHANNEL_COUNT; i++) {
            debug_put_uint8(i+0); debug_putc('=');
            debug_put_uint16(adc_data[i]);
            if (i&1) {
                debug_put_newline();
            } else {
                debug(" ");
            }
        }
        console_render();
        //screen_update();
        //led_button_r_toggle();

        // delay_us(10*1000);
        //wdt_reset();
}


/*
 int __fastcall adcMethod2(int a1)
{
  unsigned int v1; // r0@4
  int result; // r0@5

  byte_20000165 = 1;
  if ( a1 )                                     // a1 is always true adcChannelIndex is set to 0
  {
    adcChannelIndex = 0;
  }
  else if ( adcChannelIndex == 5 )
  {
    v1 = v4003B00C & 0xFFFFFFEF;                // ADC Configuration Register 2 (ADC0_CFG2)
    goto LABEL_5;
  }
  v1 = v4003B00C | 0x10;
LABEL_5:
  v4003B00C = v1;
  result = ADCH_Values[adcChannelIndex] | 0x40;
  v4003B000 = ADCH_Values[adcChannelIndex] | 0x40;
  return result;
}
 * */
/*
 int adc_setup()
{
  unsigned int channel; // r3@1
  char not_20000164; // r2@1
  int offset; // r4@1
  int result; // r0@7
  unsigned int v4; // r3@10

  channel = adcChannelIndex;
  not_20000164 = adcFlag ^ 1;
  offset = adcChannelIndex;
  if ( adcFlag )
  {
    *(_DWORD *)((char *)&ch1_ADC_VAL + offset * 4) = (unsigned int)(ADC_Results[offset] + v4003B010) >> 1;// // GET ADC ResultRegister
    if ( channel == 7 )
    {
      adcChannelIndex = 0;
      adcFlag = not_20000164;
      byte_20000165 = 0;
      result = 64;                              // BIN 1000000
      v4003B000 &= 0xFFFFFFBF;                  // unset bit 7
                                                // ADC Status and Control Registers 1 (ADC0_SC1A)
      return result;
    }
  }
  else
  {
    ADC_Results[offset] = v4003B010;            // GET ADC ResultRegister
  }
  if ( channel >= 7 )
  {
    adcChannelIndex = 0;
    adcFlag = not_20000164;
  }
  else
  {
    adcChannelIndex = channel + 1;
  }
  byte_20000165 = 1;
  if ( adcChannelIndex == 5 )
    v4 = v4003B00C & 0xFFFFFFEF;                // unset 5th bit - ADxxa channels are selected.
  else
    v4 = v4003B00C | 0x10;                      // set 5th bit - ADxxb channels are selected.
  v4003B00C = v4;                               // ADC configuration register
  result = ADCH_Values[adcChannelIndex] | 0x40; // set to ADCH_values[channel] COCO flag always 1
  v4003B000 = ADCH_Values[adcChannelIndex] | 0x40;
  return result;
}
 * */

/*
 *
 *
void adc_init(void) {
    debug("adc: init\n"); debug_flush();

    adc_battery_voltage_raw_filtered = 0;

    adc_init_rcc();
    adc_init_gpio();
    adc_init_mode();
    adc_init_dma();

    // init values(for debugging)
    uint32_t i;
    for (i = 0; i < ADC_CHANNEL_COUNT; i++) {
        adc_data[i] = 0;
    }
}

uint16_t adc_get_channel(uint32_t id) {
    // fetch correct adc channel based on hw revision
    if (config_hw_revision == CONFIG_HW_REVISION_I6S) {
        // FS-i6S mapping:
        switch (id) {
            case (CHANNEL_ID_AILERON)  : return adc_data[0];
            case (CHANNEL_ID_ELEVATION): return adc_data[1];
            case (CHANNEL_ID_THROTTLE) : return adc_data[2];
            case (CHANNEL_ID_RUDDER)   : return adc_data[3];
            case (CHANNEL_ID_CH0)      : return adc_data[4];
            case (CHANNEL_ID_CH1)      : return adc_data[5];
            case (CHANNEL_ID_CH2)      : return adc_data[8];
            case (CHANNEL_ID_CH3)      : return adc_data[9];
        }
    } else if (config_hw_revision == CONFIG_HW_REVISION_EVOLUTION) {
        // TGY Evolution mapping:
        switch (id) {
            case (CHANNEL_ID_AILERON)  : return 4095 - adc_data[3];
            case (CHANNEL_ID_ELEVATION): return 4095 - adc_data[2];
            case (CHANNEL_ID_THROTTLE) : return 4095 - adc_data[1];
            case (CHANNEL_ID_RUDDER)   : return 4095 - adc_data[0];
            case (CHANNEL_ID_CH0)      : return adc_data[5];
            case (CHANNEL_ID_CH1)      : return adc_data[8];
            case (CHANNEL_ID_CH2)      : return adc_data[6];
            case (CHANNEL_ID_CH3)      : return adc_data[4];
        }
    } else {
        // undefined!
        debug("adc: invalid hw revision ");
        debug_put_uint8(config_hw_revision);
        debug(" given!\n"); debug_flush();
        return 0;
    }
}

uint8_t *adc_get_channel_name(uint8_t i, bool short_descr) {
    switch (i) {
        default                     : return ((short_descr) ? "?" : "???");
        case (CHANNEL_ID_AILERON)   : return ((short_descr) ? "A" : "AIL");
        case (CHANNEL_ID_ELEVATION) : return ((short_descr) ? "E" : "ELE");
        case (CHANNEL_ID_THROTTLE)  : return ((short_descr) ? "T" : "THR");
        case (CHANNEL_ID_RUDDER)    : return ((short_descr) ? "R" : "RUD");
        case (CHANNEL_ID_CH0)       : return ((short_descr) ? "0" : "CH0");
        case (CHANNEL_ID_CH1)       : return ((short_descr) ? "1" : "CH1");
        case (CHANNEL_ID_CH2)       : return ((short_descr) ? "2" : "CH2");
        case (CHANNEL_ID_CH3)       : return ((short_descr) ? "3" : "CH3");
    }
}


#define ADC_RESCALE_TARGET_RANGE 3200
// return the adc channel rescaled from 0...4095 to -TARGET_RANGE...+TARGET_RANGE
// switches are scaled manually, sticks use calibration data
int32_t adc_get_channel_rescaled(uint8_t idx) {
    int32_t divider;

    // fetch raw stick value (0..4095)
    int32_t value = adc_get_channel(idx);

    // sticks are ch0..3 and use calibration coefficents:
    if (idx < 4) {
        // apply center calibration value:
        value = value - (int16_t)storage.stick_calibration[idx][1];

        // now rescale this to +/- TARGET_RANGE
        // fetch divider
        if (value < 0) {
            divider = storage.stick_calibration[idx][1] - storage.stick_calibration[idx][0];
        } else {
            divider = storage.stick_calibration[idx][2] - storage.stick_calibration[idx][1];
        }
        // apply the scale
        value = (value * ADC_RESCALE_TARGET_RANGE) / divider;
    } else {
        // for sticks we do not care about scaling/calibration (for now)
        // min is 0, max from adc is 4095 -> rescale this to +/- 3200
        // rescale to 0...6400
        value = (ADC_RESCALE_TARGET_RANGE * 2 * value) / 4096;
        value = value - ADC_RESCALE_TARGET_RANGE;
    }

    // apply the scale factor:
    switch (idx) {
        default:
            // do not apply scale
            break;
        case (CHANNEL_ID_AILERON):
        case (CHANNEL_ID_ELEVATION):
            value = (value * storage.model[storage.current_model].stick_scale) / 100;
            break;
    }

    // limit value to -3200 ... 3200
    value = max(-ADC_RESCALE_TARGET_RANGE, min(ADC_RESCALE_TARGET_RANGE, value));

    return value;
}

uint16_t adc_get_channel_packetdata(uint8_t idx) {
    // frsky packets send us * 1.5
    // where 1000 us =   0%
    //       2000 us = 100%
    // -> remap +/-3200 to 1500..3000
    // 6400 => 1500 <=> 64 = 15
    int32_t val = adc_get_channel_rescaled(idx);
    val = (15 * val) / 64;
    val = val + 2250;
    return (uint16_t) val;
}


static void adc_init_rcc(void) {
    debug("adc: init rcc\n"); debug_flush();

    // ADC CLOCK = 24 / 4 = 6MHz
    RCC_ADCCLKConfig(RCC_ADCCLK_PCLK_Div2);

    // enable ADC clock
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

    // enable dma clock
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    // periph clock enable for port
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOD, ENABLE);
}

static void adc_init_gpio(void) {
    debug("adc: init gpio\n"); debug_flush();

    GPIO_InitTypeDef gpio_init;
    GPIO_StructInit(&gpio_init);

    // set up analog inputs ADC0...ADC7(PA0...PA7)
    gpio_init.GPIO_Pin  = 0b11111111;
    gpio_init.GPIO_Mode = GPIO_Mode_AN;
    GPIO_Init(GPIOA, &gpio_init);

    // set up analog inputs ADC8, ADC9(PB0, PB1)
    gpio_init.GPIO_Pin  = 0b11;
    gpio_init.GPIO_Mode = GPIO_Mode_AN;
    GPIO_Init(GPIOB, &gpio_init);


    // battery voltage is on PC0(ADC10)
    gpio_init.GPIO_Pin  = 0b1;
    gpio_init.GPIO_Mode = GPIO_Mode_AN;
    GPIO_Init(GPIOC, &gpio_init);
}

uint32_t adc_get_battery_voltage(void) {
    // return a fixed point number of the battery voltage
    // 1230 = 12.3 V
    // raw data is 0 .. 4095 ~ 0 .. 3300mV
    // Vadc = raw * 3300 / 4095
    uint32_t raw = adc_battery_voltage_raw_filtered;
    // the voltage divider is 5.1k / 10k
    // Vadc = Vbat * R2 / (R1+R2) = Vbat * 51/ 151
    // -> Vbat = Vadc * (R1-R2) / R2
    // -> Vout = raw * 3300 * (151 / 51) / 4095
    //         = (raw * (3300 * 151) ) / (4095 * 51)
    uint32_t mv = (raw * (3300 * 151) ) / (4095 * 51);
    return mv / 10;
}

static void adc_init_mode(void) {
    debug("adc: init mode\n"); debug_flush();

    ADC_InitTypeDef adc_init;
    ADC_StructInit(&adc_init);

    // ADC configuration
    adc_init.ADC_ContinuousConvMode   = ENABLE;  // ! select continuous conversion mode
    adc_init.ADC_ExternalTrigConv     = 0;
    adc_init.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;  // select no ext triggering
    adc_init.ADC_DataAlign            = ADC_DataAlign_Right;  // r 12-bit data alignment in ADC reg
    adc_init.ADC_Resolution           = ADC_Resolution_12b;
    adc_init.ADC_ScanDirection        = ADC_ScanDirection_Upward;

    // load structure values to control and status registers
    ADC_Init(ADC1, &adc_init);

    // configure each channel
    ADC_ChannelConfig(ADC1, ADC_Channel_0, ADC_SampleTime_41_5Cycles);
    ADC_ChannelConfig(ADC1, ADC_Channel_1, ADC_SampleTime_41_5Cycles);
    ADC_ChannelConfig(ADC1, ADC_Channel_2, ADC_SampleTime_41_5Cycles);
    ADC_ChannelConfig(ADC1, ADC_Channel_3, ADC_SampleTime_41_5Cycles);
    ADC_ChannelConfig(ADC1, ADC_Channel_4, ADC_SampleTime_41_5Cycles);
    ADC_ChannelConfig(ADC1, ADC_Channel_5, ADC_SampleTime_41_5Cycles);
    ADC_ChannelConfig(ADC1, ADC_Channel_6, ADC_SampleTime_41_5Cycles);
    ADC_ChannelConfig(ADC1, ADC_Channel_7, ADC_SampleTime_41_5Cycles);
    ADC_ChannelConfig(ADC1, ADC_Channel_8, ADC_SampleTime_41_5Cycles);
    ADC_ChannelConfig(ADC1, ADC_Channel_9, ADC_SampleTime_41_5Cycles);
    ADC_ChannelConfig(ADC1, ADC_Channel_10, ADC_SampleTime_41_5Cycles);

    // enable ADC
    ADC_Cmd(ADC1, ENABLE);

    // enable DMA for ADC
    ADC_DMACmd(ADC1, ENABLE);
}

static void adc_init_dma(void) {
    debug("adc: init dma\n"); debug_flush();

    DMA_InitTypeDef  dma_init;
    DMA_StructInit(&dma_init);

    // reset DMA1 channe1 to default values
    DMA_DeInit(ADC_DMA_CHANNEL);

    // set up dma to convert 2 adc channels to two mem locations:
    // channel will be used for memory to memory transfer
    dma_init.DMA_M2M                 = DMA_M2M_Disable;
    // setting normal mode(non circular)
    dma_init.DMA_Mode                = DMA_Mode_Circular;
    // medium priority
    dma_init.DMA_Priority            = DMA_Priority_High;
    // source and destination 16bit
    dma_init.DMA_PeripheralDataSize  = DMA_PeripheralDataSize_HalfWord;
    dma_init.DMA_MemoryDataSize      = DMA_MemoryDataSize_HalfWord;
    // automatic memory destination increment enable.
    dma_init.DMA_MemoryInc           = DMA_MemoryInc_Enable;
    // source address increment disable
    dma_init.DMA_PeripheralInc       = DMA_PeripheralInc_Disable;
    // Location assigned to peripheral register will be source
    dma_init.DMA_DIR                 = DMA_DIR_PeripheralSRC;
    // chunk of data to be transfered
    dma_init.DMA_BufferSize          = ADC_CHANNEL_COUNT;
    // source and destination start addresses
    dma_init.DMA_PeripheralBaseAddr  = (uint32_t)&ADC1->DR;
    dma_init.DMA_MemoryBaseAddr      = (uint32_t)adc_data;
    // send values to DMA registers
    DMA_Init(ADC_DMA_CHANNEL, &dma_init);

    // enable the DMA1 - Channel1
    DMA_Cmd(ADC_DMA_CHANNEL, ENABLE);

    // start conversion:
    adc_dma_arm();
}

static void adc_dma_arm(void) {
    ADC_StartOfConversion(ADC1);
}

void adc_process(void) {
    // adc dma finished?
    if (DMA_GetITStatus(ADC_DMA_TC_FLAG)) {
        if (adc_battery_voltage_raw_filtered == 0) {
            // initialise with current value
            adc_battery_voltage_raw_filtered = adc_data[10];
        } else {
            // low pass filter battery voltage
            adc_battery_voltage_raw_filtered = adc_battery_voltage_raw_filtered +
                                     (4 * (adc_data[10] - adc_battery_voltage_raw_filtered)) / 128;
        }

        // fine, arm DMA again:
        adc_dma_arm();
    } else {
        // oops this should not happen
        debug_putc('D');
        // cancel and re arm dma ???
    }
}

void adc_test(void) {
    while (1) {
        console_clear();
        debug("ADC TEST  BAT: ");
        debug_put_fixed2(adc_get_battery_voltage());
        debug(" V\n");
        uint32_t i;
        adc_process();
        for (i = 0; i < ADC_CHANNEL_COUNT; i++) {
            debug_put_uint8(i+0); debug_putc('=');
            debug_put_hex16(adc_get_channel(i+0));
            if (i&1) {
                debug_put_newline();
            } else {
                debug(" ");
            }
        }
        console_render();
        screen_update();
        led_button_r_toggle();

        // delay_us(10*1000);
        wdt_reset();
        delay_ms(100);
    }
}
*/
