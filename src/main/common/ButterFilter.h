/*
 * ButterFilter.h
 *
 *  Created on: 07.05.2020
 *      Author: brm
 */

#ifndef SRC_MAIN_COMMON_BUTTERFILTER_H_
#define SRC_MAIN_COMMON_BUTTERFILTER_H_


void  createBandPassFilter(int lowerBandFrequency, int highBandFrequency, int sampleRate);
float applyBandPassFilter(int axis, float input);



#endif /* SRC_MAIN_COMMON_BUTTERFILTER_H_ */
