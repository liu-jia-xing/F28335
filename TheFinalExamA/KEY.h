/*
 * KEY.h
 *
 *  Created on: 2019��5��30��
 *      Author: ToFFF
 */

#ifndef KEY_H_
#define KEY_H_

/**************************************�궨��************************************************/
//#define SET_KY4       GpioDataRegs.GPASET.bit.GPIO0 = 1                       //Y4����
#define SET_KY3         GpioDataRegs.GPBSET.bit.GPIO48 = 1                          //Y3����
#define SET_KY2         GpioDataRegs.GPBSET.bit.GPIO49 = 1                      //Y2����
#define SET_KY1         GpioDataRegs.GPBSET.bit.GPIO50 = 1                      //Y1����

//#define RST_KY4       GpioDataRegs.GPACLEAR.bit.GPIO0 = 1                     //Y4����
#define RST_KY3         GpioDataRegs.GPBCLEAR.bit.GPIO48 = 1                    //Y3����
#define RST_KY2         GpioDataRegs.GPBCLEAR.bit.GPIO49 = 1                        //Y2����
#define RST_KY1         GpioDataRegs.GPBCLEAR.bit.GPIO50 = 1                        //Y1����

//#define KX4_STATUS        GpioDataRegs.GPBDAT.bit.GPIO50                          //X4״̬
#define KX3_STATUS      GpioDataRegs.GPBDAT.bit.GPIO51                          //X3״̬
#define KX2_STATUS      GpioDataRegs.GPBDAT.bit.GPIO52                          //X2״̬
#define KX1_STATUS      GpioDataRegs.GPBDAT.bit.GPIO53                          //X1״̬

/*********************************************��������************************************************/
void InitKeyGpio(void);  // ��ʼ����������
void delay(Uint32 t);
void RstAllKY(void);   // ������������ΪL��ƽ
void GetKX(void);      // �õ�����״̬
void GetKX_On(void);   // �õ��������������б��
void SetKY(Uint16 y);  // ����ָ������ΪH��ƽ
void RstKY(Uint16 y);  // ����ָ������ΪL��ƽ
void GetKey(void);     // �õ����µİ�����ţ�����Key��


#endif /* KEY_H_ */
