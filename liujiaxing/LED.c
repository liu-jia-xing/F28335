/*
 * LED.c
 *
 *  Created on: 2016-10-11
 *      Author: Administrator
 */
#include "DSP2833x_Device.h"
#include "DSP2833x_Examples.h"

#include "LED_M.h"   // LED×´Ì¬ºê¶¨Òå

void InitLedGpio(void)
{

	EALLOW;
	//LED1  D10<----GPIO0
	GpioCtrlRegs.GPAPUD.bit.GPIO0 = 0;   	// Enable pullup on GPIO0
    GpioDataRegs.GPASET.bit.GPIO0 = 1;   	// Load output latch
    GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 0;  	// GPIO11 = GPIO
    GpioCtrlRegs.GPADIR.bit.GPIO0 = 1;   	// GPIO11 = output
	//LED2  D11
	GpioCtrlRegs.GPAPUD.bit.GPIO1 = 0;   	// Enable pullup on GPIO1
    GpioDataRegs.GPASET.bit.GPIO1 = 1;   	// Load output latch
    GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 0;  	// GPIO11 = GPIO
    GpioCtrlRegs.GPADIR.bit.GPIO1 = 1;   	// GPIO11 = output
	//LED3  D12
	GpioCtrlRegs.GPAPUD.bit.GPIO2 = 0;   	// Enable pullup on GPIO2
    GpioDataRegs.GPASET.bit.GPIO2 = 1;   	// Load output latch
    GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 0;  	// GPIO11 = GPIO
    GpioCtrlRegs.GPADIR.bit.GPIO2 = 1;   	// GPIO11 = output
	//LED4  D13
    GpioCtrlRegs.GPAPUD.bit.GPIO3 = 0;   	// Enable pullup on GPIO3
    GpioDataRegs.GPASET.bit.GPIO3 = 1;   	// Load output latch
    GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 0;  	// GPIO11 = GPIO
    GpioCtrlRegs.GPADIR.bit.GPIO3 = 1;   	// GPIO11 = output
	//LED5  D14
	GpioCtrlRegs.GPAPUD.bit.GPIO4 = 0;   	// Enable pullup on GPIO4
    GpioDataRegs.GPASET.bit.GPIO4 = 1;   	// Load output latch
    GpioCtrlRegs.GPAMUX1.bit.GPIO4 = 0;  	// GPIO11 = GPIO
    GpioCtrlRegs.GPADIR.bit.GPIO4 = 1;   	// GPIO11 = output
	//LED6  D15
	GpioCtrlRegs.GPAPUD.bit.GPIO5 = 0;   	// Enable pullup on GPIO5
    GpioDataRegs.GPASET.bit.GPIO5 = 1;   	// Load output latch
    GpioCtrlRegs.GPAMUX1.bit.GPIO5 = 0;  	// GPIO11 = GPIO
    GpioCtrlRegs.GPADIR.bit.GPIO5 = 1;   	// GPIO11 = output
	//LED7  D16
	GpioCtrlRegs.GPAPUD.bit.GPIO12 = 0;   	// Enable pullup on GPIO12
    GpioDataRegs.GPASET.bit.GPIO12 = 1;   	// Load output latch
    GpioCtrlRegs.GPAMUX1.bit.GPIO12 = 0;  	// GPIO11 = GPIO
    GpioCtrlRegs.GPADIR.bit.GPIO12 = 1;   	// GPIO11 = output
	//LED8  D17
	GpioCtrlRegs.GPBPUD.bit.GPIO36 = 0;   	// Enable pullup on GPIO36
    GpioDataRegs.GPBSET.bit.GPIO36 = 1;   	// Load output latch
    GpioCtrlRegs.GPBMUX1.bit.GPIO36 = 0;  	// GPIO11 = GPIO
    GpioCtrlRegs.GPBDIR.bit.GPIO36  = 1;   	// GPIO11 = output

    EDIS;

    // LED³õÊ¼×´Ì¬
    LED1_OFF; LED2_OFF; LED3_OFF; LED4_OFF;
    LED5_OFF; LED6_OFF; LED7_OFF; LED8_OFF;
}

void init_led()
{
    LED1_OFF;
    LED2_OFF;
    LED3_OFF;
    LED4_OFF;
    LED5_OFF;
    LED6_OFF;
    LED7_OFF;
}

