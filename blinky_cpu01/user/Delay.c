/*
 * Delay.c
 *
 *  Created on: 2017-12-28
 *      Author: Administrator
 */

#include "F28x_Project.h"


// 150MHzʱ���²��ԡ�
void Delay_ms(Uint32 t); // �ӳ�1ms����ʾ����У��������׼ȷ
void Delay_us(Uint32 t); // �ӳ�1us����ʾ����У��������׼ȷ
//void DSP28x_usDelay(Uint32 Count); // �Դ����������ֻ������ֵ��20��֮һ��


// �ӳ�1ms����ʾ����У��������׼ȷ
void Delay_ms(Uint32 t)
{
	Uint32 i;
	for (i = 0; i < t; i++)
	{
		DSP28x_usDelay(30500);
	}
}

// �ӳ�1us����ʾ����У��������׼ȷ
void Delay_us(Uint32 t)
{
	Uint32 i;
	for (i = 0; i < t; i++)
	{
		DSP28x_usDelay(24);
	}
}

//DSP28x_usDelay(1);
