/*
 * KEY4x4.h
 *
 *  Created on: 2017-10-8
 *      Author: Administrator
 */

#ifndef KEY4X4_H_
#define KEY4X4_H_

typedef struct _data{
	unsigned char wavetype;
	unsigned long freq;
	float dianya;
}DATA;

extern void InitKeyGpio(void);  // ��ʼ����������
extern void GetKey(void);     // �õ����µİ�����ţ�����Key��
extern Uint16 Key;   // ������ţ�1-16���ֱ��ӦK1-K16������=0��ʾδ����

/*�����*/
extern void Change_WaveType();
extern void Key_Process();
extern void add_argu();
extern void sub_argu();
extern void select_argu();



#endif /* KEY4X4_H_ */
