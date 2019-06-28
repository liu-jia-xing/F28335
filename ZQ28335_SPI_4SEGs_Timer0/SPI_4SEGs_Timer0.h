/*
 * SPI_4SEGs_Timer0.h
 *
 *  Created on: 2016-11-19
 *      Author: Administrator
 */

#ifndef SPI_4SEGS_TIMER0_H_
#define SPI_4SEGS_TIMER0_H_

extern  float tintfreq;           // 定时中断频率，Hz
extern  float tintprd;            // 定时中断周期，us

extern  void InitSpia54to57Gpio(void);  				//28335  SPIa引脚
extern  void Init4SEGsGpio(void);               // 初始化4个数码管的片选引脚

extern  void InitSpiFun(void);		  // init SPI
extern  void InitSpiFifo(void);	  // Initialize the Spi FIFO

#endif /* SPI_4SEGS_TIMER0_H_ */
