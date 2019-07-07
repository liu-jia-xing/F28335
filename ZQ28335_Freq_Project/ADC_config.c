/*
 * ADC_config.c
 *
 *  Created on: 2019年6月30日
 *      Author: L9379
 */

#include "ADC_config.h"
void adc_config()
{
    AdcRegs.ADCTRL3.bit.SMODE_SEL=0x1;      //同步采样模式
       AdcRegs.ADCTRL1.bit.SEQ_CASC=0x0;    //双排序器模式
       AdcRegs.ADCMAXCONV.all=0x0033;       //最大通道数
       AdcRegs.ADCCHSELSEQ1.bit.CONV00=0x0; //ADC输入通道    ADCINA0和ADCINB0;
       AdcRegs.ADCTRL2.bit.SOC_SEQ1 = 1;
       AdcRegs.ADCTRL2.bit.INT_ENA_SEQ1 = 1;  // Enable SEQ1 interrupt (every EOS)
       AdcRegs.ADCTRL2.bit.INT_ENA_SEQ2 = 1;  // Enable SEQ2 interrupt (every EOS)
}
