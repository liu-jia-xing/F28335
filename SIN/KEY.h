/*
 * KEY.h
 *
 *  Created on: 2017-10-19
 *      Author: Administrator
 */

#ifndef KEY_H_
#define KEY_H_
extern Uint16 Key = 0;
extern void Init_KeyGpio(void); //�������ų�ʼ��
extern void GetKey(void);      // �õ�����Key��ֵ

extern void KeyFun(void);      // ��������ѡ��
#endif /* KEY_H_ */
