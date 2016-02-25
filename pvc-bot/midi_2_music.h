/** midi_2_music.h - Library for parsing MIDI input over serial and controlling actuators
 *  
 *  @author Tanuj Sane
 */
#pragma once // guard

// circuit driver constants
#define CLK 5
#define EN  6
#define RST 7
#define S1  0

 // command bytes
#define NOTE_OFF 0x10
#define NOTE_ON 0x90
#define AFTERTOUCH 0xA0
#define USER_CMD 0xF0

enum USER_CMDS {
  
};

// note bytes
enum PITCH_PLEASE { // midi pitch values 36-60 
  c2 = 0x24, csh2, d2, dsh2, e2, f2, fsh2, g2, gsh2, a2, ash2, b2,
  c3, csh3, d3, dsh3, e3, f3, fsh3, g3, gsh3, a3, ash3, b3,
  c4
};

const byte vel = 100;

void init_timer();
byte midi_parse();
ISR(TIMER1_COMPA_vect);
void set(int b);
void clear();
void config();
void playNote();






