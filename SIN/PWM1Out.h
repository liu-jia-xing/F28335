/*
 * PWM1Out.h
 *
 *  Created on: 2016-11-24
 *      Author: Administrator
 */

#ifndef PWM1OUT_H_
#define PWM1OUT_H_

#define TIMES      198 // 采样次数
#define BUF_SIZE   64  // 采样缓冲区大小


//extern unsigned char  distance[6];
//extern float Distance;
// Global variable for this example
extern float PWM1_Freq;
extern Uint16 TBPrd1;
extern float sanjiao[BUF_SIZE];
extern unsigned char game[BUF_SIZE];
extern unsigned char duty[BUF_SIZE];       //将函数值从-1到1转化为0到1

extern void SetupEPwm1Parm(void);
extern void cpu_timer0_isr(void);

#endif /* PWM1OUT_H_ */
