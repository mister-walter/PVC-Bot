/** pvc-bot.ino - Driver file for PVC-Bot
 *  @author Tanuj Sane
 */

// circuit driver constants
#define CLK 5
#define EN  6
#define RST 7
#define S1  0

 // command bytes //TODO: SET CHANNEL USING LOW-ORDER NIBBLE
#define NOTE_OFF 0x10
#define NOTE_ON 0x90

// Function declarations
void init_timer();
void play_note();
void _do_nothing();
void set(int b);
void clear();
void config();

// variables
byte cmd, pitch;
byte off; // velocity replaced by on or off flag

int t12 = 0x0001; int t13 = 0x0001;

void setup() {
  init_timer();
  clear();
}

void loop() {
  
}

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

