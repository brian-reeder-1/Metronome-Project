/*
 * buzzer.c: PWM functions and globals for buzzer
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

// including headers to access pins on launchpad, timer functionality, and GPIO functionality through driverlib, and buzzer.h with prototypes
#include "buzzer.h"
#include "launchpad.h"
#include <stdbool.h>
#include <stdint.h>
#include <inc/hw_memmap.h>
#include <inc/hw_timer.h>
#include <driverlib/sysctl.h>
#include <driverlib/gpio.h>
#include <driverlib/pin_map.h>
#include <driverlib/timer.h>


// array of floats to store frequencies.
// inner menu, case 1, rotates through these frequencies.
float freq[6] =
{
    // frequency values for each note, in Hz
    {4186.01},  // C8
    {4300.01},  // C8 + 200 Hz

    {4698.64},  // D8
    {4898.64},  // D8 + 200 Hz

    {5587.65},  // F8
    {5787.65},  // F8 + 200 Hz
};


/*
 * Initialize the timer PWM functions connected to the Buzzer.
 * Buzzer is on jumper J17, so it has Pin 36 on the Grove Base BoosterPack,
 * which connects to PC5 (module 0, sub-timer B) on the LaunchPad
 *  \
 *   \--> [Pin usage: Grove base port J17, Tiva C PC5 (Port C, Pin 5)]
 *
 * WT0CCP1, wide timer module 0
 */
void BuzzerInit()
{
    // Enable Wide Timer 0 and GPIO Port C
    SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);

    // Connect pins to those timers
    GPIOPinTypeTimer(GPIO_PORTC_BASE, GPIO_PIN_5);
    GPIOPinConfigure(GPIO_PC5_WT0CCP1);

    // Select PWM for Wide Timer 0 sub-Timer B, using WTIMER0_BASE since we use PC5 of J17
    TimerConfigure(WTIMER0_BASE, (TIMER_CFG_SPLIT_PAIR | TIMER_CFG_B_PWM));

    // Invert the PWM waveform, so that the Match register value is the pulse width.
    // Otherwise, the pulse width will be (Load value) - (Match value).
    // The inversion is done by enabling output inversion on the PWM pins.
    TimerControlLevel(WTIMER0_BASE, TIMER_B, true /* output inversion */);

    // Enable Wide Timer 0's TimerB
    TimerEnable(WTIMER0_BASE, TIMER_B);
}


void BuzzerSet(int pitch_index, int volume)
{
    // convert our PWM period into period by dividing our frequency from the clocks period (50 MHz)
    int freq_pulse_period = (50000000 / freq[pitch_index]);

    // failsafe in case too large of a volume value was passed
    if(volume > 30) volume = 30;
    //else if(volume < 0) volume = 0;

    // get the pulse width by multiplying our computed pulse period by our duty cycle percent
    // duty cycle is volume/100 -- we multiply that fraction by the period to get the pulse's width
    int freq_pulse_width = (freq_pulse_period * volume) / 100;

    // set the PWM parameters for the buzzer
    TimerLoadSet(WTIMER0_BASE, TIMER_B, freq_pulse_period - 1);
    TimerMatchSet(WTIMER0_BASE, TIMER_B, freq_pulse_width - 1);
}



