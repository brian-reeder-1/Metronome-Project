/*
 * rotary_angle_sensor.c: ADC and ISR functions for the RAS unit
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

#include "rotary_angle_sensor.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <inc/hw_memmap.h>
#include <inc/hw_ints.h>
#include <driverlib/sysctl.h>
#include <driverlib/interrupt.h>
#include <driverlib/adc.h>


// RAS reading states
typedef struct {
    uint32_t raw_data;          // the raw reading from the ADC connected to the RAS
    uint32_t new_BPM;           // new BPM computed based on the RAS reading
} TempSensorState;

TempSensorState rotary_angle_sensor;    // struct object for runtime processes


/*
 * Initialize ADC to use the external RAS
 *
 * Resources: ADC0, sequence #1, channel 7 (AIN7), aka: ADC_CTL_CH7 <-- corresponds to PD0 on the grover booster, which connects to pin23 on jumper j5
 * Configurations: processor trigger, interrupt enabled on trigger, use step 0 only
 */
void RASInit()
{
    // Enable the ADC0 peripheral
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);

    // Configure ADC0's sequencer #1
    ADCSequenceConfigure(ADC0_BASE, 1 /* sequencer */, ADC_TRIGGER_PROCESSOR, 0 /* priority */);

    // Configure step 0 of sequencer 1 to use the RAS, with
    // interrupt enable, as the end of the sequence.
    // Use ADC_CTL_CH7 for input 7 on the ADC, which corresponds to PD0.
    // PD0 connects to p23 (signal) on jumper J5.
    // "IE" configures the sensor so when we do a sampling and finish, the interrupt flag is set and the ISR function runs.
    ADCSequenceStepConfigure(ADC0_BASE, 1 /* sequencer */, 0 /* step */,
                             ADC_CTL_CH7 | ADC_CTL_IE | ADC_CTL_END);

    // Configure the interrupt system for RAS
    ADCIntRegister(ADC0_BASE, 1 /* sequencer */, RotaryAngleSensorISR);
    ADCIntEnable(ADC0_BASE, 1 /* sequencer */);

    // Enable ADC0, sequencer 1
    ADCSequenceEnable(ADC0_BASE, 1 /* sequencer */);
}


/*
 * The ISR function: Read the data, mark data ready flag, and
 * schedule the callback event
 */
void RotaryAngleSensorISR()
{
    // Get the data <--> [0, 4095]
    ADCSequenceDataGet(ADC0_BASE, 1 /* sequencer */, &rotary_angle_sensor.raw_data /* pointer to data buffer */);

    float ras_BPM_ratio = (rotary_angle_sensor.raw_data / 4095.0);
    rotary_angle_sensor.new_BPM = 150 - (ras_BPM_ratio * 100);

    // IMPORTANT: Clear the interrupt flag
    ADCIntClear(ADC0_BASE, 1 /* sequencer */);
}


/*
 * Trigger RAS reading
 */
void RASTriggerReading()
{
    // Trigger the sampling
    ADCProcessorTrigger(ADC0_BASE, 1 /* sequencer */);
}


/*
 * Returns the new BPM that was computed during the previous manual trigger and ISR
 */
int RASDataRead()
{
    return rotary_angle_sensor.new_BPM;
}
