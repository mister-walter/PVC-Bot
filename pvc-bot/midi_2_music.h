/** midi_2_music.h - Library for parsing MIDI input over serial and controlling actuators
 *  
 *  @author Tanuj Sane
 */
#pragma once // guard
#include <Arduino.h>

// circuit driver constants
#define CLK 5
#define EN  6
#define RST 7
#define S1  0

 // command bytes //TODO: SET CHANNEL USING LOW-ORDER NIBBLE
#define NOTE_OFF 0x10
#define NOTE_ON 0x90

void init_timer();
void play_note();
void _do_nothing();
ISR(TIMER1_COMPA_vect);
void set(int b);
void clear();
void config();






