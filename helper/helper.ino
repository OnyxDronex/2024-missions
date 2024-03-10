#include <SoftwareSerial.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_TCS34725.h>

#include <Servo.h>
#include <Wire.h>

//PIN declaration
#define ECHO_PIN 3
#define TXD_PIN 4
#define RXD_PIN 5
#define LED_RING_PIN 6
#define LED_STRIP_PIN 11
#define LAZER_PIN 2
#define LDR_PIN A0


//Delays
#define RING_LED_DELAY 300
#define DISTANCE_DELAY 200
#define RING_LED_DELAY 300
#define SERVO_DELAY 1000

//etc
#define RING_LED_NUM 48
#define STRIP_LED_NUM 48
#define RED 120
#define BLUE 120
#define GREEN 0
#define DISTANCE_TO_ACTIVATE 200

//objects?
SoftwareSerial myBlue(TXD_PIN, RXD_PIN);
Adafruit_NeoPixel ring = Adafruit_NeoPixel(RING_LED_NUM, LED_RING_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip = Adafruit_NeoPixel(STRIP_LED_NUM, LED_STRIP_PIN, NEO_GRB + NEO_KHZ800);

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS,
                                          TCS34725_GAIN_4X);

Servo servo1;
Servo servo2;

//delayshit
long currentMillis;

long ledPreviousMillis;
long ledoffPreviousMillis;
long distancePreviousMillis;
long servoPreviousMillis;

int blinks = 0;
int distance;
int currentLed = 0;


//flags
bool servo1Flag = false;
bool servo2Flag = false;
bool blinkFlag = false;
bool turnOnRingsWithDelayFlag = false;
bool isOn = false;
char in;








//------------------------------------------------------------
void setup() {
  Serial.begin(9600);
  myBlue.begin(9600);
  initSystem();
}



void loop() {
  updateSystem();
  if (myBlue.available() > 0) {
    in = (char)myBlue.read();
  }
}
//------------------------------------------------------------




void initSystem() {
  initPin();
  initServo();
  initColorSensor();
  initRing();
}


void initPin() {
  pinMode(ECHO_PIN, INPUT);
  pinMode(LED_RING_PIN, OUTPUT);
  pinMode(LED_STRIP_PIN, OUTPUT);
  pinMode(LAZER_PIN, OUTPUT);
  pinMode(LDR_PIN, INPUT);
}

void initColorSensor() {
  if (tcs.begin()) {
    Serial.println("Found sensor");

  } else {
    Serial.println("No TCS34725 found ");
    while (1)
      ;  // halt!
  }
}

void initServo() {
  servo1.attach(10);
  servo2.attach(9);
  servo1.write(0);
  servo2.write(90);
}

void initRing() {
  ring.begin();
  ring.clear();
  ring.show();
}

void initStrip() {
  strip.begin();
  strip.clear();
  turnOnStrip();
  strip.show();
}










//updates
void updateSystem() {
  updateTime();
  updateServo1();
  updateServo2();
}

void updateTime() {
  currentMillis = millis();
}


void updateServo1() {
  if (servo1Flag) {
    servo1.write(90);
    servoPreviousMillis = currentMillis;
    servo1Flag = false;
  }
  if (currentMillis - servoPreviousMillis >= SERVO_DELAY && !servo1Flag) {
    servo1.write(0);
    blinks = 0;
  }
}

void updateServo2() {
  if (servo2Flag) {
    servo2.write(0);
    servoPreviousMillis = currentMillis;
    servo2Flag = false;
  }
  if (currentMillis - servoPreviousMillis >= SERVO_DELAY && !servo2Flag) {
    servo2.write(90);
    blinks = 0;
  }
}

void updateBlink() {
  if (blinkFlag && blinks < 3) {

    if (currentMillis - ledPreviousMillis >= RING_LED_DELAY && !isOn) {
      ledPreviousMillis = currentMillis;
      isOn = true;
      for (int i = 0; i < RING_LED_NUM; i++) {
        ring.setPixelColor(i, ring.Color(RED, GREEN, BLUE));
      }
      ring.show();
    }
    if (currentMillis - ledPreviousMillis >= RING_LED_DELAY && isOn) {
      ledPreviousMillis = currentMillis;
      isOn = false;
      ring.clear();
      ring.show();
      blinks++;
    }
  } else {
    blinkFlag = false;
  }
}


void turnOnRingsWithDelayUpdate() {
  if (turnOnRingsWithDelayFlag) {
    if (currentMillis - ledPreviousMillis >= RING_LED_DELAY) {
      ledPreviousMillis = currentMillis;
      currentLed++;
      if (currentLed > RING_LED_NUM) {
        currentLed = 0;
        ring.clear();
      }
      ring.setPixelColor(currentLed, ring.Color(RED, GREEN, BLUE));
      ring.show();
    }
  }
}








//enables

void openServo1() {
  servo1Flag = true;
  // Serial.println("servo1");
}



void openServo2() {
  servo2Flag = true;
  // Serial.print("servo2");
}



void blink() {
  blinkFlag = true;
}

void turnOnRingsWithDelay() {
  turnOnRingsWithDelayFlag = true;
}








//geters
String getColor() {
  uint16_t r, g, b, c;
  tcs.getRawData(&r, &g, &b, &c);

  word red, blue, green;
  red = (float)c / r;
  green = (float)c / g;
  blue = (float)c / b;

  if (blue < red && blue < green) {
    return "blue";
  }
  if (green < red && blue > green) {
    return "green";
  }
  if (blue > red && red < green) {
    return "red";
  }
  return "none";
}


int getCurrentDistance() {

  distancePreviousMillis = currentMillis;
  word duration, distance;
  duration = pulseIn(ECHO_PIN, HIGH);
  distance = duration / 58;
  // Serial.print("distance = ");
  // Serial.println(distance);
  return distance;
}

int getLDRValue(){
  return analogRead(LDR_PIN);
}


void turnOnLazer() {
  digitalWrite(LAZER_PIN, HIGH);
}

void turnOffLazer() {
  digitalWrite(LAZER_PIN, LOW);
}




void turnOnStrip() {
  for (int i = 0; i < 5; i++) {
    strip.setPixelColor(i, strip.Color(255, 0, 0));
  }
  for (int i = 5; i < 12; i++) {
    strip.setPixelColor(i, strip.Color(0, 255, 0));
  }
  strip.setPixelColor(12, strip.Color(255, 0, 0));
}



