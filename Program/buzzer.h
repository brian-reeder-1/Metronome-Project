/*
 * buzzer.h: PWM functions for buzzer
 *
 * ----------------------------
 *  Created on: Nov 24, 2025
 *     Author: Brian Reeder
 *
 *  Contact:
 *   brianreeder124@gmail.com
 *   brian-reeder-1 (GitHub)
 * ----------------------------
 */

#ifndef BUZZER_H_
#define BUZZER_H_

// Initializes the buzzer for PWM (pulse width modulation)
void BuzzerInit();

// Sets the period and pulse width based on the pitch and volume
void BuzzerSet(int pitch_index, int volume);

#endif /* BUZZER_H_ */
