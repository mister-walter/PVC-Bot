/** midi_parse.cpp - Library for parsing MIDI input over serial
 *  
 *  @author Tanuj Sane
 */

#include <Arduino.h>
#include "midi_parse.h"

byte cmd, pitch, vel;

void init_timer()
{
  Serial.begin(31250);
  // initialize timer1 
  cli(); // clear interrupts           
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;

  OCR1A = 31250;            // compare match register 16MHz/256/2Hz
  TCCR1B |= (1 << WGM12);   // CTC mode
  TCCR1B |= (1 << CS12);    // 256 prescaler 
  TIMSK1 |= (1 << OCIE1A);  // enable timer compare interrupt
  sei();             // enable all interrupts
}

ISR(TIMER1_COMPA_vect)          // timer compare interrupt service routine
{
  do{
    if(Serial.available()){
      cmd = Serial.read();
      pitch = Serial.read();
      vel = Serial.read();
    }
  }
  while (Serial.available() > 2); // at least 3 bytes available
}

void midi_parse(byte cmd){
  switch(cmd){
    case NOTE_ON:
      
    break;

    case NOTE_OFF:
    break;

    case AFTERTOUCH:
    break;

    case USER_CMD:
    break;
  }
}

