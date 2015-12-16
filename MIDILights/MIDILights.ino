#include <WS2812.h>

#define LED_COUNT 73
#define LED_PIN MOSI
#define MAX_FX 36
#define OFF {g:0, r:0, b:0}
#define LOW_THRESH 60
#define HIGH_THRESH 65

typedef struct {
  cRGB color;
  byte position;
  boolean direction = true;
  boolean active = false;
} NOTE;

WS2812 LED(LED_COUNT);
NOTE FX[MAX_FX];
byte CLEAR[MAX_FX];
byte MAX_BRIGHT = 128;
byte DECAY = 4;
byte INTERVAL = 25;
boolean DIRECTION = false;
byte NEW_PITCH, PREV_PITCH = 0;
byte VELOCITY = 0;

void setup() {
  Serial.begin(9600);
  LED.setOutput(LED_PIN);
  for(byte i = 0; i < LED_COUNT; i++){
    LED.set_crgb_at(i, OFF);
  }
  LED.sync();
}

void loop() {
  static unsigned long time1 = 0, time2 = 0;
  static byte green = 0, red = 0, blue = 0, intensity = 0;
  
  if(millis() - time1 > 2*INTERVAL) {
    time1 = millis();
    if(VELOCITY > 0)
      createEffect(36, DIRECTION, green*intensity, red*intensity, blue*intensity);
  }

  if(millis() - time2 > INTERVAL) {
    time2 = millis();
    updateEffects();
  }

  if(Serial.available() > 1) {
    byte leading_input = Serial.read();
    if(leading_input < 1 || leading_input > 5)
      return;
    byte input_value = Serial.read();
    switch(leading_input){
      case 1:
        NEW_PITCH = input_value;
        break;
      case 2:
        VELOCITY = input_value;
        if(VELOCITY > 0){
          if(NEW_PITCH != PREV_PITCH){
            PREV_PITCH = NEW_PITCH;
            DIRECTION = !DIRECTION;
          }
          intensity = MAX_BRIGHT;
          green = 0; red = 0; blue = 0;
          if(NEW_PITCH < LOW_THRESH) blue = 1;
          else if(NEW_PITCH < HIGH_THRESH) green = 1;
          else red = 1;
          createEffect(36, DIRECTION, green*intensity, red*intensity, blue*intensity);
        }
        break;
      case 3:
        MAX_BRIGHT = input_value;
        break;
      case 4:
        DECAY = input_value;
        break;
      case 5:
        INTERVAL = input_value;
        break;
    }
  }
}

void doEffect(byte index){
  LED.set_crgb_at(FX[index].position, FX[index].color);
  if(FX[index].color.g >= DECAY)
    FX[index].color.g -= DECAY;
  else
    FX[index].color.g = 0;
  if(FX[index].color.r >= DECAY)
    FX[index].color.r -= DECAY;
  else
    FX[index].color.r = 0;
  if(FX[index].color.b >= DECAY)
    FX[index].color.b -= DECAY;
  else
    FX[index].color.b = 0;
  CLEAR[getEmptyClearQueue()] = FX[index].position;
}

void updateEffects(){
  clearEffects();
  for(byte i = 0; i < MAX_FX; i++)
    if(FX[i].active){
      doEffect(i);
      FX[i].position += FX[i].direction ? 1 : -1;
      if(FX[i].position >= LED_COUNT)
        FX[i].active = false;
    }
  LED.sync();
}

void createEffect(byte _position, byte _direction, byte _g, byte _r, byte _b){
  byte index = getEmptyEffect();
  FX[index].active = true;
  FX[index].direction = _direction;
  FX[index].position = _position;
  FX[index].color.g = _g;
  FX[index].color.r = _r;
  FX[index].color.b = _b;
}

byte getEmptyEffect(){
  for(byte i = 0; i < MAX_FX; i++)
    if(!FX[i].active)
      return i;
  return 0;
}

byte getEmptyClearQueue(){
  for(byte i = 0; i < MAX_FX; i++)
    if(CLEAR[i] == 255)
      return i;
  return 0;
}

void clearEffects(){
  for(byte i = 0; i < MAX_FX; i++)
    if(CLEAR[i] != 255){
      LED.set_crgb_at(CLEAR[i], OFF);
      CLEAR[i] = 255;
    }
}

