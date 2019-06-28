/*
 * Delay.c
 *
 *  Created on: 2017-12-28
 *      Author: Administrator
 */

#include "F28x_Project.h"


// 150MHz时钟下测试。
void Delay_ms(Uint32 t); // 延迟1ms，用示波器校正，基本准确
void Delay_us(Uint32 t); // 延迟1us，用示波器校正，基本准确
//void DSP28x_usDelay(Uint32 Count); // 自带函数，大概只有设置值的20分之一。


// 延迟1ms，用示波器校正，基本准确
void Delay_ms(Uint32 t)
{
	Uint32 i;
	for (i = 0; i < t; i++)
	{
		DSP28x_usDelay(30500);
	}
}

// 延迟1us，用示波器校正，基本准确
void Delay_us(Uint32 t)
{
	Uint32 i;
	for (i = 0; i < t; i++)
	{
		DSP28x_usDelay(24);
	}
}

//DSP28x_usDelay(1);
