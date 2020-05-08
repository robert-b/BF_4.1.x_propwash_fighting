/*
 * ButterFilter.c
 *
 *  Created on: 07.05.2020
 *      Author: brm
 */

#include <math.h>
#include "ButterFilter.h"

#define resonance 	(1.4f)	// 2nd order response

typedef enum passFilter
{
    Highpass,
     Lowpass,

} passFilter_t;

typedef struct ButterFilter
{
    float frequency;
    int sampleRate;
    passFilter_t passType;

    float c, a1, a2, a3, b1, b2;

    /// <summary>
    /// Array of input values, latest are in front
    /// </summary>
    float inputHistory[2];

    /// <summary>
    /// Array of output values, latest are in front
    /// </summary>
    float outputHistory[3];

} Butterfilter_t;

void createButterFilter(Butterfilter_t *this, int frequency, int sampleRate, passFilter_t passType);
float Update(Butterfilter_t *this, float newInput);


void createButterFilter(Butterfilter_t *this, int frequency, int sampleRate, passFilter_t passType)
{
	this->frequency = frequency;
	this->sampleRate = sampleRate;
	this->passType = passType;

	switch (this->passType)
	{
	case Lowpass:
		this->c = 1.0f / tanf(M_PI * frequency / sampleRate);
		this->a1 = 1.0f / (1.0f + resonance * this->c + this->c * this->c);
		this->a2 = 2.0f * this->a1;
		this->a3 = this->a1;
		this->b1 = 2.0f * (1.0f - this->c * this->c) * this->a1;
		this->b2 = (1.0f - resonance * this->c + this->c * this->c) * this->a1;
		break;
	case Highpass:
		this->c = tanf(M_PI * frequency / sampleRate);
		this->a1 = 1.0f / (1.0f + resonance * this->c + this->c * this->c);
		this->a2 = -2.0f * this->a1;
		this->a3 = this->a1;
		this->b1 = 2.0f * (this->c * this->c - 1.0f) * this->a1;
		this->b2 = (1.0f - resonance * this->c + this->c * this->c) * this->a1;
		break;
	}
}

float Update(Butterfilter_t *this, float newInput)
{
	const float newOutput = this->a1 * newInput
			+ this->a2 * this->inputHistory[0]
			+ this->a3 * this->inputHistory[1]
			- this->b1 * this->outputHistory[0]
			- this->b2 * this->outputHistory[1];

	this->inputHistory[1] = this->inputHistory[0];
	this->inputHistory[0] = newInput;

	this->outputHistory[2] = this->outputHistory[1];
	this->outputHistory[1] = this->outputHistory[0];
	this->outputHistory[0] = newOutput;

	return newOutput;
}

Butterfilter_t lpFilter[3];
Butterfilter_t hpFilter[3];

void createBandPassFilter(int lowerBandFrequency, int highBandFrequency, int sampleRate)
{
	int axis;

	for (axis = 0; axis < 3; axis++)
	{
		createButterFilter(&lpFilter[axis], lowerBandFrequency, sampleRate, Lowpass);
		createButterFilter(&hpFilter[axis], highBandFrequency, sampleRate, Highpass);
	}
}

float applyBandPassFilter(int axis, float input)
{
	const float val = Update(&lpFilter[axis], input);
	return Update(&hpFilter[axis], val);
}
