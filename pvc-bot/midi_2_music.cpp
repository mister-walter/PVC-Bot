/** midi_2_music.cpp - Library for parsing MIDI input over serial and controlling actuators
 *  
 *  @author Tanuj Sane
 */

#include <Arduino.h>
#include "midi_2_music.h"

byte cmd, pitch;
byte off; // velocity replaced by on or off flag

int row12, row13;

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
      off = Serial.read();
    }
  }
  while (Serial.available() > 2); // at least 3 bytes available
}

byte midi_parse(){
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

void set(int b){
  int i = 0x800;
  while(i){
    if(b & i) PORTB |= 1;
    else PORTB &= ~(1);
    PORTD |= 1 << CLK;
    i >>= 1;
    PORTD &= ~(1 << CLK);
  }
  PORTD |= 1 << EN;
  delay(1);
  PORTD &= ~(1 << EN);
  delay(1);
  PORTD &= ~(1 << RST);
  delay(1);
  PORTD |= 1 << RST;
}

void clear(){
  PORTD |= 1 << EN;
  delay(1);
  PORTD &= ~(1 << EN);
}

void config(){
  DDRD |= 1 << CLK;
  DDRD |= 1 << EN;
  DDRD |= 1 << RST;
  DDRB |= 1 << S1;
  PORTD |= 1 << RST;
}


