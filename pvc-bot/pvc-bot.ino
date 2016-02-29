/** pvc-bot.ino - Driver file for PVC-Bot
 *  @author Tanuj Sane
 */

#define CLK 5
#define EN  6
#define RST 7
#define S1  0
#define MAX_MSG 3
#define BAUD_RATE 9600

#define START 1
#define STOP 2

// Function declarations
void init_timer();
void play_note();
void set(int b);
void clear();
void config();

// variables
byte serial_in;
int pitch, vel;

void setup() {
  Serial.begin(BAUD_RATE);
  Serial.flush();
  pinMode(13, OUTPUT);
  init_timer();
  config();
  clear();
}

void loop() {
  if (Serial.available() > 3){
    serial_in = Serial.read();
    switch(serial_in){
      case START:
        pitch = Serial.read();
        vel = Serial.read();
      break;
      
      case STOP:
        play_note(pitch, vel);
      break;
    }
  }    
}

void play_note(byte pitch, byte vel){
  OCR1A = 9600; /* TODO: Some math here for vel -> volume calculation */
  if(pitch < 60) set(1 << (pitch - 48));
  else if(pitch < 72) set(1 << (pitch - 60));
  else;
}

void init_timer()
{
  // initialize timer1 
  cli(); // clear interrupts           
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;
  OCR1A = BAUD_RATE; // fire interrupt at same frequency as Serial
  TCCR1B |= (1 << WGM12); 
  TCCR1B |= (1 << CS12); 
  TIMSK1 |= (1 << OCIE1A);  // enable timer compare interrupt
  sei(); // enable all interrupts
}

ISR(TIMER1_COMPA_vect){
  
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

