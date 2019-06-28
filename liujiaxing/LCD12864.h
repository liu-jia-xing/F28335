/*
 * LCD12864.h
 *
 *  Created on: 2018-8-4
 *      Author: Administrator
 */

#ifndef LCD12864_H_
#define LCD12864_H_

extern void InitLcdGpio(void);
extern void InitLcd(void);
extern void TestLcd(void);
extern void DispENG(unsigned char row,unsigned char col,unsigned char *puts,Uint16 nums);
#endif /* LCD12864_H_ */
