#include <SoftwareSerial.h>
#include <Adafruit_NeoPixel.h>


#include <Servo.h>
#include <Wire.h>

//PIN declaration
#define ECHO_PIN 8
#define LED_RING_PIN 2

//etc
#define RING_LED_NUM 12


//objects?

Adafruit_NeoPixel ring = Adafruit_NeoPixel(RING_LED_NUM, LED_RING_PIN, NEO_GRB + NEO_KHZ800);



//delayshit








int distance;
unsigned long currentMillis;
unsigned long previousMillis;

//------------------------------------------------------------
void setup() {
  Serial.begin(9600);
  initSystem();
}



void loop() {
  currentMillis = millis();
  int d = getCurrentDistance();
  turnOnRings(200,0,200,d);
  delay(200);
}
//------------------------------------------------------------



void initSystem() {
  initPin();
  initRing();
}


void initPin() {
  pinMode(ECHO_PIN, INPUT);
  pinMode(LED_RING_PIN, OUTPUT);
}


void initRing() {
  ring.begin();
  ring.clear();
  ring.show();
}

//updates

void turnOnRings(int red, int green, int blue, int distance) {
  int num;
  ring.clear();
  if(distance > 50){
    num =1;
    num *= (distance-50)/25;
  }else{
    num = -1;
  }
    blink(red,green,blue,500,num);

}




//geters

int getCurrentDistance() {

  word duration, distance;
  duration = pulseIn(ECHO_PIN, HIGH);
  distance = duration / 58;
  Serial.print("distance = ");
  Serial.println(distance);
  return distance;
}

void blink(int r, int g,int b, int interval, int num) {


  if (currentMillis - previousMillis >= interval) {
    
    previousMillis = currentMillis;

    static bool state = false;
    state = !state;

    if (state) {
      ring.fill(ring.Color(r, g, b), 0, num);
    } else {
      ring.fill(ring.Color(r, g, b), 0, num);
    }
    ring.show(); // Update Neopixel Ring
  }
}





