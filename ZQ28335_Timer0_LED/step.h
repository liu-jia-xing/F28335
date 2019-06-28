/*
 * step.h
 *
 *  Created on: 2019��5��20��
 *      Author: L9379
 */

#ifndef STEP_H_
#define STEP_H_
#include "DSP2833x_Device.h"
#include "DSP2833x_Examples.h"
/**************************************�궨��************************************************/
#define PHA_ON      GpioDataRegs.GPASET.bit.GPIO8 = 1       //����ߵ�ƽ��A��
#define PHA_OFF     GpioDataRegs.GPACLEAR.bit.GPIO8 = 1     //����͵�ƽ��A��
#define PHB_ON      GpioDataRegs.GPASET.bit.GPIO10 = 1      //����ߵ�ƽ��B��
#define PHB_OFF     GpioDataRegs.GPACLEAR.bit.GPIO10 = 1    //����͵�ƽ��B��
#define PHC_ON      GpioDataRegs.GPASET.bit.GPIO13 = 1      //����ߵ�ƽ��C��
#define PHC_OFF     GpioDataRegs.GPACLEAR.bit.GPIO13 = 1    //����͵�ƽ��C��
#define PHD_ON      GpioDataRegs.GPASET.bit.GPIO14 = 1      //����ߵ�ƽ��D��
#define PHD_OFF     GpioDataRegs.GPACLEAR.bit.GPIO14 = 1    //����͵�ƽ��D��

#define DELAY_TIME  750000                                  //��ʱʱ��
#define STEP_TIME   15000                                   //����
/*****************************************************************************************************/

/************************************���岽���������ʱ��**********************************************/
// ����ģʽ˵����16������0xABCD,ÿ4λ����һ���ƽ�����и�8λ����F28335����͵�ƽ����ʱ�򣬵�8λ����F28335����ߵ�ƽ����ʱ��
// ������A,�������A�࣬������B,�������B�࣬0���ֲ���
//���磺0x0D0A----->D���Ϊ�͵�ƽ��A���Ϊ�ߵ�ƽ
//    0x0000----->���ֵ�ƽ״̬
//    0x0A0B----->A���Ϊ�͵�ƽ��B���Ϊ�ߵ�ƽ


#endif /* STEP_H_ */
