/*
 * Cal_config.h
 *
 *  Created on: 2019Äê6ÔÂ30ÈÕ
 *      Author: L9379
 */

#ifndef CAL_CONFIG_H_
#define CAL_CONFIG_H_
#include "DSP28x_Project.h"

__interrupt void adc_isr(void);
void start_sample(float *Voltage1,float *Voltage2,float *amplitude);
void cal_frequency(float *Voltage1,float *Voltage2,float *amplitude);
void phase_cal();
#endif /* CAL_CONFIG_H_ */
