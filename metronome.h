/*
 * metronome.h: stores globals for metronome and system
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

#ifndef METRONOME_H_
#define METRONOME_H_

// this file is intended to improve organization of main.c

// global variables for use in metronome_main.c
// in depth descriptions can be found below:

uint32_t time_signature_selection = 0; // stores an index to be used with the time signature struct array; default for testing is 4/4 (index 0).

uint32_t count = 0; // count stores the count for the metronome sequence.
                    // count is also used in the outer menu to index through each menu option.

uint32_t BPM = 100; // BPM (beats per minute) is used to compute how often the buzzer will beep, default is 100 BPM.

uint32_t pitch_index = 0; // stores the pitch index to be passed to BuzzerSet in the metronome sequence

uint32_t beat_time_ms = 0;  // total time of one beat (buzzer on and off time)
uint32_t buzz_on_time = 0;  // time in ms the buzzer will be on   <---\__ (both computed in outer menu, case 2)
uint32_t buzz_off_time = 0; // time in ms the buzzer will be off  <-/

bool play = true; // used for metronome sequence, guides function to either play buzzer tone or turn off buzzer.

bool outer_menu = true; // used to direct the menu options in the push button ISR callback function.
                        // outer menu = true implies that the metronome sequence is not active. user has option between different time signatures.
                        // outer menu = false => inner menu; implies the metronome sequence is active. user has option between buzzer frequencies-
                        // and can stop the system.

// stores info for each time signature
typedef struct
{
    char *display_name;     // display name stores char info that is used for menu options on the 7 segment
    uint32_t beats_per_bar; // beats per bar stores number of beats per bar, which is used in the metronome sequencing
    uint32_t pattern[8];    // pattern stores the numbers to display on the 7 segment during sequencing
    uint32_t accents[8];
} TimeSignature;

// stores all of the time signatures (unchanged during runtime)
// currently 5 options are supported
TimeSignature TIME_SIGNATURES[] =
    {
        {"4:4", 4, {1, 2, 3, 4, 0, 0, 0, 0}},
        {"3:4", 3, {1, 2, 3, 0, 0, 0, 0, 0}},
        {"2:4", 2, {1, 2, 0, 0, 0, 0, 0, 0}},
        {"5:4", 5, {1, 2, 3, 4, 5, 0, 0, 0}},
        {"7:4", 7, {1, 2, 3, 4, 5, 6, 7, 0}},
        /*
        {"6:4", 6, {1, 2, 3, 4, 5, 6, 0, 0}},
        {"7:4", 4, {1, 2, 3, 4, 5, 6, 7, 0}}*/
};

#endif /* METRONOME_H_ */
