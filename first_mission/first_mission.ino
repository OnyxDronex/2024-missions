#include <SoftwareSerial.h>
#include <Adafruit_NeoPixel.h>

#include <Servo.h>
#include <Wire.h>

//PIN declaration
#define TXD_PIN 4
#define RXD_PIN 5
#define LED_RING_PIN 6

//etc
#define RING_LED_NUM 48
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
  PURPLE = 'p'
};

ColorItzhak in;

//------------------------------------------------------------
void setup() {
  Serial.begin(9600);
  myBlue.begin(9600);
  initSystem();
}


void loop() {
    if(myBlue.available() == 0){
      in = ColorItzhak(myBlue.read());
    }
    // in = ColorItzhak(Serial.read());
  

 

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


