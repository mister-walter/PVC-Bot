/** pvc-bot.ino - Driver file for PVC-Bot
 *  @author Tanuj Sane
 */

#define CLK 5
#define EN  6
#define RST 7
#define S1  0
#define MAX_MSG 3
#define BAUD_RATE 9600

// Function declarations
void init_timer();
void play_note();
void set(int b);
void clear();
void config();
byte hexify(String buf[], int size);

#define START 0x01
#define STOP 0x02

// variables
byte serial_in = 0x00;
byte pitch, vel;

void setup() {
  Serial.begin(BAUD_RATE);
  Serial.flush();
  init_timer();
  config();
  clear();
}

void loop() {
  if (Serial.available()) serial_in = Serial.read();
  switch(serial_in){
    case START:
      pitch = Serial.read() - '0';
      vel = Serial.read() - '0';
    break;
    
    case STOP: default:
      pitch = 0x00;
      vel = 0x00;
    break;
 }

  play_note(pitch, vel);
}

void play_note(byte pitch, byte vel){
  if(pitch < 48) set(1 << (pitch - 36));
  else if(pitch < 60) set(1 << (pitch - 48));
  else ;
  OCR1A = 9600; /* TODO: Some math here for vel -> volume calculation */
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

ISR(TIMER1_COMPA_vect) // timer compare interrupt service routine
{
  
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

