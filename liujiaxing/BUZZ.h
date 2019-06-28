/*
 * BUZZ.h
 *
 *  Created on: 2019年6月18日
 *      Author: L9379
 */

#ifndef BUZZ_H_
#define BUZZ_H_
#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File
#define GEN_BUZZ_CLK    GpioDataRegs.GPBTOGGLE.bit.GPIO35 = 1   //蜂鸣器控制IO，IO电平翻转，产生控制脉冲
#define BUZZ_OFF        GpioDataRegs.GPBCLEAR.bit.GPIO35 = 1    //关闭蜂鸣器
void InitBuzzGpio(void);
interrupt void cpu_timer0_isr(void);
#endif /* BUZZ_H_ */
