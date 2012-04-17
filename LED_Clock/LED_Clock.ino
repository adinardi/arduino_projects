#include <Wire.h>
#include "RTClib.h"
#include "LPD8806.h"
#include "SPI.h"

#define PIXELS 60

RTC_DS1307 RTC; // gnd = 16, 5v = 17, data = 18, 19
LPD8806 strip = LPD8806(PIXELS); // data = 11, clock = 13

void setup () {
  Serial.begin(57600);
  Wire.begin();
  RTC.begin();
  strip.begin();
  strip.show();
  
  // Make 16 GND
  pinMode(16, OUTPUT);
  digitalWrite(16, LOW);
  
  // Make 17 5v
  pinMode(17, OUTPUT);
  digitalWrite(17, HIGH);

  if (!RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
  }
}

void loop () {
  DateTime now = RTC.now();
  
  // Clear all pixels
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, 0, 0, 0);
  }
  
  // Set seconds
  strip.setPixelColor(now.second(), 255, 0, 0);
  
  // Set minutes
  strip.setPixelColor(now.minute(), 0, 255, 0);
  
  // Set hours
  float hr = (float)now.hour();
  if (hr > 12) {
    hr -= 12;
  }
  hr += ((float)now.minute() / 60.0) ;
  strip.setPixelColor((hr / 12.0) * 60.0, 0, 0, 255);
  
//  Serial.print(now.second());
//  Serial.print(":");
//  Serial.print(now.minute());
//  Serial.print(":");
//  Serial.print(hr);
//  Serial.print("!");
//  Serial.println(((hr / 24.0) * 60.0));
  strip.show();
}
