/*
 * LED_M.h
 *
 *  Created on: 2016-10-25
 *      Author: Administrator
 */

#ifndef LED_M_H_
#define LED_M_H_


// LED״̬�궨��

#define LED1_OFF	GpioDataRegs.GPASET.bit.GPIO0   = 1	 //LED D10 ����
#define LED1_ON 	GpioDataRegs.GPACLEAR.bit.GPIO0 = 1	 //LED D10 Ϩ��

#define LED2_OFF	GpioDataRegs.GPASET.bit.GPIO1   = 1	 //LED D11 ����
#define LED2_ON 	GpioDataRegs.GPACLEAR.bit.GPIO1 = 1	 //LED D11 Ϩ��

#define LED3_OFF	GpioDataRegs.GPASET.bit.GPIO2   = 1	 //LED D12 ����
#define LED3_ON 	GpioDataRegs.GPACLEAR.bit.GPIO2 = 1	 //LED D12 Ϩ��

#define LED4_OFF	GpioDataRegs.GPASET.bit.GPIO3   = 1	 //LED D13 ����
#define LED4_ON 	GpioDataRegs.GPACLEAR.bit.GPIO3 = 1	 //LED D13 Ϩ��

#define LED5_OFF	GpioDataRegs.GPASET.bit.GPIO4   = 1	 //LED D14 ����
#define LED5_ON 	GpioDataRegs.GPACLEAR.bit.GPIO4 = 1	 //LED D14 Ϩ��

#define LED6_OFF	GpioDataRegs.GPASET.bit.GPIO5   = 1	 //LED D15 ����
#define LED6_ON 	GpioDataRegs.GPACLEAR.bit.GPIO5 = 1	 //LED D15 Ϩ��

#define LED7_OFF	GpioDataRegs.GPASET.bit.GPIO12   = 1 //LED D16 ����
#define LED7_ON 	GpioDataRegs.GPACLEAR.bit.GPIO12 = 1 //LED D16 Ϩ��

#define LED8_OFF	GpioDataRegs.GPBSET.bit.GPIO36   = 1 //LED D17 ����
#define LED8_ON 	GpioDataRegs.GPBCLEAR.bit.GPIO36 = 1 //LED D17 Ϩ��



#endif /* LED_M_H_ */
