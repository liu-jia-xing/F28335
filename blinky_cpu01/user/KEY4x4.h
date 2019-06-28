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

extern void InitKeyGpio(void);  // 初始化按键引脚
extern void GetKey(void);     // 得到按下的按键编号，放在Key中
extern Uint16 Key;   // 按键编号，1-16，分别对应K1-K16按键，=0表示未按下

/*谷汶峻*/
extern void Change_WaveType();
extern void Key_Process();
extern void add_argu();
extern void sub_argu();
extern void select_argu();



#endif /* KEY4X4_H_ */
