/* ����PWM1��Ƶ��Ϊ200Hz������CMPA��CMPB��ֵ��
 * �ı�PWM1A��PWM1B·��ռ�ձȣ��Ӷ��ı�LED1��LED2������
 * SYSCLKʱ�Ӿ�HSPCLKDIV��CLKDIV��Ƶ�󣬵õ�TBCLK
 * ������������PWM1_Freq�ı�Ƶ�ʣ�����CMPA��CMPB�ı�ռ�ձȣ�LED������L��Ч��
 * �������޸Ĳ���ͨ������������20111129
 */


#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File
#include "PWM1Out.h"


void main(void)
{
   InitSysCtrl();

   // EPWMģ��ÿ�����2·���Σ��ֱ���A��B���֣�Ĭ�����Ƿ��ࣻ
   // EPWM1ģ���EPWM1A���������GPIO0, EPWM1B���������GPIO1
   InitEPwm1Gpio();    // ������Ҫ������Σ��ɲ���ʼ����ģ������
 
   DINT;
   InitPieCtrl();
   IER = 0x0000;
   IFR = 0x0000;
   InitPieVectTable();

   SetupEPwm1Parm();

    for(;;);
}
//===========================================================================
// No more.
//===========================================================================
