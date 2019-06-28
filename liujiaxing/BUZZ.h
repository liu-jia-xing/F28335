/*
 * BUZZ.h
 *
 *  Created on: 2019��6��18��
 *      Author: L9379
 */

#ifndef BUZZ_H_
#define BUZZ_H_
#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File
#define GEN_BUZZ_CLK    GpioDataRegs.GPBTOGGLE.bit.GPIO35 = 1   //����������IO��IO��ƽ��ת��������������
#define BUZZ_OFF        GpioDataRegs.GPBCLEAR.bit.GPIO35 = 1    //�رշ�����
void InitBuzzGpio(void);
interrupt void cpu_timer0_isr(void);
#endif /* BUZZ_H_ */
