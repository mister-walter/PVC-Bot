/** midi_parse.h - Library for parsing MIDI input over serial
 *  
 *  @author Tanuj Sane
 */
#pragma once // guard

 // command bytes
#define NOTE_OFF 0x10
#define NOTE_ON 0x90
#define AFTERTOUCH 0xA0
#define USER_CMD 0xF0

// note bytes
enum NOTES {
  
};

// velocity bytes
#define NOTE_VEL 100 // TODO: Find that value or ignore completely

void init_timer();
void midi_parse();
ISR(TIMER1_COMPA_vect);






