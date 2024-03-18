#include <SoftwareSerial.h>
#include <Adafruit_NeoPixel.h>

#include <Servo.h>
#include <Wire.h>

//PIN declaration
#define TXD_PIN 4
#define RXD_PIN 5
#define LED_RING_PIN 6

//etc
#define RING_LED_NUM 12
#define DISTANCE_TO_ACTIVATE 200

//objects?
SoftwareSerial myBlue(TXD_PIN, RXD_PIN);
Adafruit_NeoPixel ring(RING_LED_NUM, LED_RING_PIN, NEO_GRB + NEO_KHZ800);

enum ColorItzhak: char{
  RED = 'r',
  YELLOW = 'y',
  BLUE = 'b',
  ORANGE = 'o',
  GREEN = 'g',
  PURPLE = 'p',
  RGB = 'n'
};

ColorItzhak in;

//------------------------------------------------------------
void setup() {
  Serial.begin(9600);
  myBlue.begin(9600);
  initSystem();
}


void loop() {
    // if(myBlue.available() == 0){
    //   in = ColorItzhak(myBlue.read());
    // }
    in = ColorItzhak(Serial.read());
  

 

  switch (in) {
    case RED:
      turnLed(255,0,0);
      break;
    case YELLOW:
      turnLed(200,200,0);
      break;
    case BLUE:
      turnLed(0,0,255);
      break;
    case PURPLE:
      turnLed(70,0,142);
      break;
    case ORANGE:
      turnLed(255,70,0);
      break;
    case GREEN:
      turnLed(0,255,0);
      break;
    case RGB:
      rainbowCycle(5);
      break;

  }
}
//------------------------------------------------------------


void initSystem() {
  initPin();
  initRing();
}


void initPin() {
  pinMode(LED_RING_PIN, OUTPUT);
}

void initRing() {
  ring.begin();
  ring.clear();
  ring.show();
}

void turnLed(int red, int green, int blue){

  ring.clear();

  

    ring.fill(ring.Color(red, green, blue), 0, RING_LED_NUM);

  
  ring.show(); 
}


void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*1; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< ring.numPixels(); i++) {
      ring.setPixelColor(i, Wheel(((i * 256 / ring.numPixels()) + j) & 255));
    }
    ring.show();
    delay(wait);
  }
}

uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return ring.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return ring.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return ring.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

