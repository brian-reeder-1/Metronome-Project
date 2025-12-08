/*
 * rotary_angle_sensor.h: ADC and ISR function prototypes for the RAS unit
 *
 * ----------------------------
 *  Created on: Nov 28, 2025
 *     Author: Brian Reeder
 * 
 *  Contact:
 *   brianreeder124@gmail.com
 *   brian-reeder-1 (GitHub)
 * ----------------------------
 */

#ifndef ROTARY_ANGLE_SENSOR_H_
#define ROTARY_ANGLE_SENSOR_H_

#include "launchpad.h"      // so event objects can be used in function prototypes


// Initialize ADC to use the RAS on jumper 5
void RASInit();

// Trigger RAS reading/sampling
void RASTriggerReading();

// Return RAS reading after properly interpreting raw data
int RASDataRead();

// The ISR function
void RotaryAngleSensorISR();


#endif /* ROTARY_ANGLE_SENSOR_H_ */
