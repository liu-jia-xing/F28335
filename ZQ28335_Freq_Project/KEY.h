/*
 * KEY.h
 *
 *  Created on: 2018-8-4
 *      Author: Administrator
 */

#ifndef KEY_H_
#define KEY_H_

extern void InitKeyGpio(void); // 按键引脚初始化
extern void GetKey(void);  // 得到按键值，放在Uint16 Key中
extern void KeyFun(void);        //按键功能设定函数
#endif /* KEY_H_ */
