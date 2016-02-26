/** midi_2_music.cpp - Library for parsing MIDI input over serial and controlling actuators
 *  
 *  @author Tanuj Sane
 */

#include "midi_2_music.h"

byte cmd, pitch;
byte off; // velocity replaced by on or off flag

int t12 = 0x0001; int t13 = 0x0001;

void init_timer()
{
  Serial.begin(31250);
  // initialize timer1 
  cli(); // clear interrupts           
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;
  OCR1A = 31250; // fire interrupt at same frequency as Serial
  TCCR1B |= (1 << WGM12); 
  TCCR1B |= (1 << CS12); 
  TIMSK1 |= (1 << OCIE1A);  // enable timer compare interrupt
  sei(); // enable all interrupts
}

ISR(TIMER1_COMPA_vect) // timer compare interrupt service routine
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

void play_note(){
  switch(cmd){
    case NOTE_ON:
      if(pitch < 48) set(1 << (pitch - 36));
      else if (pitch < 61) set(1 << (pitch - 48));
      else;
    break;

    case NOTE_OFF:
        _do_nothing();
    break;
  }
}

void _do_nothing(){
  return;
}

void set(int b){
  PORTB |= b;
  PORTD |= 1 << EN;
  delay(1);
  PORTD &= ~(1 << EN);
  delay(1);
  PORTD &= ~(1 << RST);
  delay(1);
  PORTD |= 1 << RST;
}

void clear(){
  PORTD |= (1 << EN);
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


