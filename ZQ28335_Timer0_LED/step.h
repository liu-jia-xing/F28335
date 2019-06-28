/*
 * step.h
 *
 *  Created on: 2019年5月20日
 *      Author: L9379
 */

#ifndef STEP_H_
#define STEP_H_
#include "DSP2833x_Device.h"
#include "DSP2833x_Examples.h"
/**************************************宏定义************************************************/
#define PHA_ON      GpioDataRegs.GPASET.bit.GPIO8 = 1       //输出高电平到A相
#define PHA_OFF     GpioDataRegs.GPACLEAR.bit.GPIO8 = 1     //输出低电平到A相
#define PHB_ON      GpioDataRegs.GPASET.bit.GPIO10 = 1      //输出高电平到B相
#define PHB_OFF     GpioDataRegs.GPACLEAR.bit.GPIO10 = 1    //输出低电平到B相
#define PHC_ON      GpioDataRegs.GPASET.bit.GPIO13 = 1      //输出高电平到C相
#define PHC_OFF     GpioDataRegs.GPACLEAR.bit.GPIO13 = 1    //输出低电平到C相
#define PHD_ON      GpioDataRegs.GPASET.bit.GPIO14 = 1      //输出高电平到D相
#define PHD_OFF     GpioDataRegs.GPACLEAR.bit.GPIO14 = 1    //输出低电平到D相

#define DELAY_TIME  750000                                  //延时时间
#define STEP_TIME   15000                                   //步距
/*****************************************************************************************************/

/************************************定义步进电机运行时序**********************************************/
// 步进模式说明：16进制数0xABCD,每4位控制一相电平，其中高8位控制F28335输出低电平引脚时序，低8位控制F28335输出高电平引脚时序
// 数字是A,代表控制A相，数字是B,代表控制B相，0保持不变
//例如：0x0D0A----->D相变为低电平，A相变为高电平
//    0x0000----->保持电平状态
//    0x0A0B----->A相变为低电平，B相变为高电平


#endif /* STEP_H_ */
