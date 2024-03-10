#include <SoftwareSerial.h>
#include <Adafruit_NeoPixel.h>
#include <Wire.h>

#define SENSOR_ADDRESS 0x48

#include <Servo.h>
#include <Wire.h>

//PIN declaration
#define ECHO_PIN 8
#define LED_RING_PIN 2

//etc
#define RING_LED_NUM 12


//objects?

Adafruit_NeoPixel ring = Adafruit_NeoPixel(RING_LED_NUM, LED_RING_PIN, NEO_GRB + NEO_KHZ800);




int distance;
unsigned long currentMillis;
unsigned long previousMillis;

int roomtemp;

//------------------------------------------------------------
void setup() {
  Serial.begin(9600);
  Wire.begin();
  initSystem();

  roomtemp = readTemperature();
}



void loop() {
  currentMillis = millis();
  float temperature = readTemperature();
  int d = getCurrentDistance();
  int num;

  if (d > 50) {
    num = 1;
    num = (d - 50) / 25;
  } else {
    num = 0;
  }

  if (temperature > roomtemp + 2) {
    blink(200, 0, 0, 300, num);

  } else if (temperature < roomtemp - 0.5) {
    blink(0, 0, 200, 300, num);

  } else {
    turnOnRings(0, 200, 0, num);
  }
  Serial.print("distance = ");
  Serial.print(d);

  Serial.print(" temp = ");
  Serial.println(temperature);
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


void turnOnRings(int red, int green, int blue, int num) {
  ring.clear();
  if (num != 0) {
    ring.fill(ring.Color(red, green, blue), 0, num);
  }
  ring.show();  // Update Neopixel Ring
}



int getCurrentDistance() {

  word duration, distance;
  duration = pulseIn(ECHO_PIN, HIGH);
  distance = duration / 58;
  return distance;
}

void blink(int r, int g, int b, int interval, int num) {
  static bool state = false;

  if (currentMillis - previousMillis >= interval) {

    previousMillis = currentMillis;

    state = !state;
    ring.clear();
    if (num != 0) {
      if (state) {
        ring.fill(ring.Color(r, g, b), 0, num);
      }

      ring.show();
    }
  }
}


float readTemperature() {
  Wire.beginTransmission(SENSOR_ADDRESS);
  Wire.write(0);
  Wire.endTransmission();

  delay(100);

  Wire.requestFrom(SENSOR_ADDRESS, 2);
  if (Wire.available() == 2) {
    byte msb = Wire.read();
    byte lsb = Wire.read();

    int tempRaw = ((msb << 8) | lsb) >> 4;
    float temperatureC = tempRaw * 0.0625;

    return temperatureC;
  } else {

    Serial.println("Error: Data not available");
    return -273.15;
  }
}
