/*
 * LED_M.h
 *
 *  Created on: 2016-10-25
 *      Author: Administrator
 */

#ifndef LED_M_H_
#define LED_M_H_


// LED×´Ì¬ºê¶¨Òå

#define LED1_OFF	GpioDataRegs.GPASET.bit.GPIO0   = 1	 //LED D10 µãÁÁ
#define LED1_ON 	GpioDataRegs.GPACLEAR.bit.GPIO0 = 1	 //LED D10 Ï¨Ãð

#define LED2_OFF	GpioDataRegs.GPASET.bit.GPIO1   = 1	 //LED D11 µãÁÁ
#define LED2_ON 	GpioDataRegs.GPACLEAR.bit.GPIO1 = 1	 //LED D11 Ï¨Ãð

#define LED3_OFF	GpioDataRegs.GPASET.bit.GPIO2   = 1	 //LED D12 µãÁÁ
#define LED3_ON 	GpioDataRegs.GPACLEAR.bit.GPIO2 = 1	 //LED D12 Ï¨Ãð

#define LED4_OFF	GpioDataRegs.GPASET.bit.GPIO3   = 1	 //LED D13 µãÁÁ
#define LED4_ON 	GpioDataRegs.GPACLEAR.bit.GPIO3 = 1	 //LED D13 Ï¨Ãð

#define LED5_OFF	GpioDataRegs.GPASET.bit.GPIO4   = 1	 //LED D14 µãÁÁ
#define LED5_ON 	GpioDataRegs.GPACLEAR.bit.GPIO4 = 1	 //LED D14 Ï¨Ãð

#define LED6_OFF	GpioDataRegs.GPASET.bit.GPIO5   = 1	 //LED D15 µãÁÁ
#define LED6_ON 	GpioDataRegs.GPACLEAR.bit.GPIO5 = 1	 //LED D15 Ï¨Ãð

#define LED7_OFF	GpioDataRegs.GPASET.bit.GPIO12   = 1 //LED D16 µãÁÁ
#define LED7_ON 	GpioDataRegs.GPACLEAR.bit.GPIO12 = 1 //LED D16 Ï¨Ãð

#define LED8_OFF	GpioDataRegs.GPBSET.bit.GPIO36   = 1 //LED D17 µãÁÁ
#define LED8_ON 	GpioDataRegs.GPBCLEAR.bit.GPIO36 = 1 //LED D17 Ï¨Ãð



#endif /* LED_M_H_ */
