/*
 * Embedded Metronome Project
 *
 * "metronome_main.c"
 * ----------------------------
 *  Created on: Nov 11, 2025
 *     Author: Brian Reeder
 *
 *  Contact:
 *   brianreeder124@gmail.com
 *   brian-reeder-1 (GitHub)
 * ----------------------------
 */

#include "launchpad.h"
#include "seg7.h"
#include "metronome.h"
#include "buzzer.h"
#include "rotary_angle_sensor.h"

/*
 * Global data structures and variables
 */

// 7 segment display object:
// 0xFF = 255, which leaves the digit on the 7-segment blank initially
Seg7Display seg7 = {{0xFF, 0xFF, 0xFF, 0xFF}, false};

// Event objects:
Event push_button_event;
Event metronome_event;
Event ras_data_event;

/*
 * Pushbutton callback function for Switch ISR (implements metronome menu)
 */
void PushButtonMenu(Event *event)
{
    // Process the pushbutton input

    // This block runs if outer menu is true
    // Has rotating options for menu and selection
    if (outer_menu == true)
    {
        switch (PushButtonRead())
        {
        case 1:                      // rotate
            count = (count + 1) % 5; // % 5 so we wrap/rotate all options

            // updates to menu screen as user rotates options
            Seg7Clear(&seg7);
            seg7.digit[2] = (TIME_SIGNATURES[count].display_name[0]) - '0'; // subtract '0' to get char as integer
            seg7.digit[1] = (TIME_SIGNATURES[count].display_name[2]) - '0';
            seg7.colon_on = true;
            Seg7Update(&seg7);

            break;

        case 2: // select
            RASTriggerReading();
            BPM = RASDataRead(); // get the starting BPM computed from the RAS's position

            // case 2 means user selected a time signature, we now compute the tempo information in milliseconds
            // we do this just before the schedule to save on processing power
            beat_time_ms = (60000 / BPM);                // gets total time of 1 beat (on and off time) -- truncation is accepted here
            buzz_on_time = beat_time_ms * 0.20;          // gets total time of buzz (25% of beat time is a buzz)
            buzz_off_time = beat_time_ms - buzz_on_time; // subtract from total time to get time the buzzer will be off in one beat

            outer_menu = false;               // sets outer menu to false, buttons in play mode will correspond to inner menu
            play = true;                      // play will be set to true
            time_signature_selection = count; // set the global selection based on current count
            count = 0;                        // reset count to 0 for use in metronome

            Seg7Clear(&seg7);      // reset the digits we used for the menu
            seg7.colon_on = false; // turn off the colon before metronome starts counting
            Seg7Update(&seg7);

            EventSchedule(&metronome_event, 2500); // schedule metronome

            break;
        }
    }
    else
    { // outer menu was false, so we check inner menu cases
        switch (PushButtonRead())
        {
        case 1: // change buzzer frequency

            // changes the frequency of the buzzer by altering the pitch index
            // pitch index is passed to BuzzerSet() in the metronome sequence
            pitch_index = (pitch_index + 2) % 6;

            break;

        case 2: // turn off current sequence, enter outer menu
            outer_menu = true;
            play = false;
            count = 0;
            BuzzerSet(0, 0); // turn off buzzer

            // clear the screen and re-display the menu options
            Seg7Clear(&seg7);
            seg7.digit[2] = (TIME_SIGNATURES[count].display_name[0]) - '0';
            seg7.digit[1] = (TIME_SIGNATURES[count].display_name[2]) - '0';
            seg7.colon_on = true;
            Seg7Update(&seg7);

            break;
        }
    }
}

// this function carries out the basic metronome sequencing via polling
// each call indicates the buzzer firing, or time in between each
void MetronomeSequence(Event *event)
{
    // if outer menu is false then user selected a time signature in the outer menu.
    // process will not run if the user is in the outer menu.
    if (outer_menu == false)
    {
        // if play is true, we want to buzz and count
        if (play)
        {
            seg7.digit[3] = (TIME_SIGNATURES[time_signature_selection]).pattern[count];
            Seg7Update(&seg7);

            // buzzer on, conditional check to emphasize first beat of each sequence
            if (count == 0)
                BuzzerSet(pitch_index + 1, 27);
            else
                BuzzerSet(pitch_index, 25);

            count = (count + 1) % (TIME_SIGNATURES[time_signature_selection].beats_per_bar); // increment count and wrap based on pattern length of selected signature
            play = false;                                                                    // play set to false, next event call will not buzz and wait the remaining time

            EventSchedule(event, event->time + buzz_on_time);
        }
        // if play is false, we want to turn off buzzer and not count
        else
        {
            // buzzer off
            BuzzerSet(0, 0);

            // only read for tempo update on the first beat of a new bar

            // trigger the RAS sampling process (causes an interrupt on the RAS)
            RASTriggerReading();
            uint32_t new_BPM = RASDataRead(); // check the new BPM computed from the RAS's position

            // only change the BPM if its different from the old one
            if (new_BPM > BPM + 2 || new_BPM < BPM - 2)
            {
                BPM = new_BPM;
                beat_time_ms = (60000 / BPM);
                buzz_on_time = beat_time_ms * 0.20;
                buzz_off_time = beat_time_ms - buzz_on_time;
            }

            play = true;
            EventSchedule(event, event->time + buzz_off_time);
        }
    }
}

/*
 * The main function
 */
void main(void)
{
    // initialize the launch pad, 7 segment display, buzzer, RAS
    LaunchPadInit();
    Seg7Init();
    BuzzerInit();
    RASInit();

    // initialize pushbutton for ISR and callback function
    EventInit(&push_button_event, PushButtonMenu);
    PushButtonEventRegister(&push_button_event);

    // initialize the metronome polling event with it's callback function
    EventInit(&metronome_event, MetronomeSequence);

    uprintf("%s\n\r", "Lab 9 Project: Metronome");

    // initial update to the menu screen before any action is taken by the user
    // first option is 4/4, so menu displays 4:4
    seg7.digit[2] = (TIME_SIGNATURES[0].display_name[0]) - '0';
    seg7.digit[1] = (TIME_SIGNATURES[0].display_name[2]) - '0';
    seg7.colon_on = true;
    Seg7Update(&seg7);

    // Loop forever
    while (true)
    {

        // Wait for interrupt
        asm("   wfi");

        // Execute scheduled callbacks
        EventExecute();
    }
}
