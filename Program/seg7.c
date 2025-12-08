/*
 * seg7.c: Starter code for the digital interface for the 4-digit 7-segment display.
 *
 * ----------------------------
 *  Created on: Nov 11, 2025
 *     Author: Brian Reeder
 *
 *  Contact:
 *   brianreeder124@gmail.com
 *   brian-reeder-1 (GitHub)
 *
 *
 *  Adapted from code by:
 *          Zhao Zhang @ UIC
 * ----------------------------
 *
 */

#include <stdint.h>
#include <stdbool.h>
#include "seg7.h"

/*
 * 7-segment coding table. See https://en.wikipedia.org/wiki/Seven-segment_display. The segments
 * are named as A, B, C, D, E, F, G. In this coding table, segments A-G are mapped to bits 0-7.
 * Bit 7 is not used in the coding. This display uses active high signal, in which '1' turns ON a
 * segment, and '0' turns OFF a segment.
 *
 */
static uint8_t seg7_coding_table[36] = {
    // these binary digits indicate a segment is on or off.
    // order of segments input is reverse alphabetical, G --> A.
    0b00111111, // digit 0
    0b00000110, // digit 1
    0b01011011, // digit 2
    0b01001111, // digit 3
    0b01100110, // digit 4
    0b01101101, // digit 5
    0b01111101, // digit 6
    0b00000111, // digit 7
    0b01111111, // digit 8
    0b01100111, // digit 9

    // binary digits for English Alphabet
    0b01110111, // A
    0b01111100, // B
    0b00111001, // C
    0b01011110, // D
    0b01111001, // E
    0b01110001, // F
    0b00111101, // G
    0b01110110, // H
    0b00000100, // I
    0b00011110, // J
    0b01110101, // K
    0b00111000, // L
    0b00010101, // M
    0b01010100, // N
    0b00111111, // O
    0b01110011, // P
    0b01100111, // Q
    0b01010000, // R
    0b01101101, // S
    0b01111000, // T
    0b00111110, // U
    0b00111110, // V
    0b00011101, // W
    0b01110110, // X
    0b01101110, // Y
    0b01011011, // Z
};

/*
 * Update the 7-segment display using the Seg7Display structure.
 */
void Seg7Update(Seg7Display *seg7)
{
    uint8_t seg7_code[4];
    uint8_t colon_code;

    // Get the raw encoding for the colon
    colon_code = seg7->colon_on ? 0b10000000 : 0b00000000;

    // Get the raw encoding for 7-segment digits
    seg7_code[0] = seg7_coding_table[seg7->digit[0]] + colon_code;
    seg7_code[1] = seg7_coding_table[seg7->digit[1]] + colon_code;
    seg7_code[2] = seg7_coding_table[seg7->digit[2]] + colon_code;
    seg7_code[3] = seg7_coding_table[seg7->digit[3]] + colon_code;

    // Update the 7-segment
    Seg7RawUpdate(seg7_code);
}

/*
 * Function designed to reset the seg7 to blank digits
 */
void Seg7Clear(Seg7Display *seg7)
{
    seg7->digit[3] = 0xFF;
    seg7->digit[2] = 0xFF;
    seg7->digit[1] = 0xFF;
    seg7->digit[0] = 0xFF;
}
