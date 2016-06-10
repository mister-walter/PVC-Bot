/** pvc-bot.ino - Driver file for PVC-Bot
 *  @author Tanuj Sane
 */

#define CLK 5
#define EN  6
#define RST 7
#define S1  0
#define MAX_MSG 3
#define BAUD_RATE 38400

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
int pitch, vel, set_val;

void setup() {
  Serial.begin(BAUD_RATE);
  config();
  clear();
}

void loop() {
  if (Serial.available() > 3){
    if(Serial.peek() == START){
      Serial.read(); // pop start byte
      pitch = Serial.read();
      vel = Serial.read();
      if(Serial.peek() == STOP) {
        Serial.read(); // pop stop byte to make it feel important
        play_note(pitch, vel);
      }
    }
  }    
}
void play_note(byte p, byte v){
  if(v == 0) return;
  else{
    if(p < 60) set_val = 1 << (p - 48);
    else if(p < 72) set_val = 1 << (p - 60);
    set(set_val);
    delay(8);
    clear();
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
  //delay(1);
  PORTD &= ~(1 << EN);
  //delay(1);
  PORTD &= ~(1 << RST);
  //delay(1);
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

