/*
 * KEY.h
 *
 *  Created on: 2019年5月30日
 *      Author: ToFFF
 */

#ifndef KEY_H_
#define KEY_H_

/**************************************宏定义************************************************/
//#define SET_KY4       GpioDataRegs.GPASET.bit.GPIO0 = 1                       //Y4拉高
#define SET_KY3         GpioDataRegs.GPBSET.bit.GPIO48 = 1                          //Y3拉高
#define SET_KY2         GpioDataRegs.GPBSET.bit.GPIO49 = 1                      //Y2拉高
#define SET_KY1         GpioDataRegs.GPBSET.bit.GPIO50 = 1                      //Y1拉高

//#define RST_KY4       GpioDataRegs.GPACLEAR.bit.GPIO0 = 1                     //Y4拉低
#define RST_KY3         GpioDataRegs.GPBCLEAR.bit.GPIO48 = 1                    //Y3拉低
#define RST_KY2         GpioDataRegs.GPBCLEAR.bit.GPIO49 = 1                        //Y2拉低
#define RST_KY1         GpioDataRegs.GPBCLEAR.bit.GPIO50 = 1                        //Y1拉低

//#define KX4_STATUS        GpioDataRegs.GPBDAT.bit.GPIO50                          //X4状态
#define KX3_STATUS      GpioDataRegs.GPBDAT.bit.GPIO51                          //X3状态
#define KX2_STATUS      GpioDataRegs.GPBDAT.bit.GPIO52                          //X2状态
#define KX1_STATUS      GpioDataRegs.GPBDAT.bit.GPIO53                          //X1状态

/*********************************************函数声明************************************************/
void InitKeyGpio(void);  // 初始化按键引脚
void delay(Uint32 t);
void RstAllKY(void);   // 设置所有列线为L电平
void GetKX(void);      // 得到行线状态
void GetKX_On(void);   // 得到按键按下所在行编号
void SetKY(Uint16 y);  // 设置指定列线为H电平
void RstKY(Uint16 y);  // 设置指定列线为L电平
void GetKey(void);     // 得到按下的按键编号，放在Key中


#endif /* KEY_H_ */
