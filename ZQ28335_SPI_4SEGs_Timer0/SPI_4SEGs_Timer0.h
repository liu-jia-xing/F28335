/*
 * SPI_4SEGs_Timer0.h
 *
 *  Created on: 2016-11-19
 *      Author: Administrator
 */

#ifndef SPI_4SEGS_TIMER0_H_
#define SPI_4SEGS_TIMER0_H_

extern  float tintfreq;           // ��ʱ�ж�Ƶ�ʣ�Hz
extern  float tintprd;            // ��ʱ�ж����ڣ�us

extern  void InitSpia54to57Gpio(void);  				//28335  SPIa����
extern  void Init4SEGsGpio(void);               // ��ʼ��4������ܵ�Ƭѡ����

extern  void InitSpiFun(void);		  // init SPI
extern  void InitSpiFifo(void);	  // Initialize the Spi FIFO

#endif /* SPI_4SEGS_TIMER0_H_ */
