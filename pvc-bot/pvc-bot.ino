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
ISR(TIMER1_COMPA_vect);
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


