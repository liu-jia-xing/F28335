/*
 * ADC_config.c
 *
 *  Created on: 2019��6��30��
 *      Author: L9379
 */

#include "ADC_config.h"
void adc_config()
{
    AdcRegs.ADCTRL3.bit.SMODE_SEL=0x1;      //ͬ������ģʽ
       AdcRegs.ADCTRL1.bit.SEQ_CASC=0x0;    //˫������ģʽ
       AdcRegs.ADCMAXCONV.all=0x0033;       //���ͨ����
       AdcRegs.ADCCHSELSEQ1.bit.CONV00=0x0; //ADC����ͨ��    ADCINA0��ADCINB0;
       AdcRegs.ADCTRL2.bit.SOC_SEQ1 = 1;
       AdcRegs.ADCTRL2.bit.INT_ENA_SEQ1 = 1;  // Enable SEQ1 interrupt (every EOS)
       AdcRegs.ADCTRL2.bit.INT_ENA_SEQ2 = 1;  // Enable SEQ2 interrupt (every EOS)
}
