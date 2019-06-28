/*
 * KEY.h
 *
 *  Created on: 2017-10-19
 *      Author: Administrator
 */

#ifndef KEY_H_
#define KEY_H_
extern Uint16 Key = 0;
extern void Init_KeyGpio(void); //按键引脚初始化
extern void GetKey(void);      // 得到按键Key的值

extern void KeyFun(void);      // 按键功能选择
#endif /* KEY_H_ */
