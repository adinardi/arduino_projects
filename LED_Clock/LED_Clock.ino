#include <Wire.h>
#include "RTClib.h"
#include "LPD8806.h"
#include "SPI.h"

#define PIXELS 60

RTC_DS1307 RTC; // gnd = 16, 5v = 17, data = 18, 19
LPD8806 strip = LPD8806(PIXELS); // data = 11, clock = 13

int lastSecond = 0;

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
  
  float hr = (float)now.hour();
  if (hr > 12) {
    hr -= 12;
  }
  hr += ((float)now.minute() / 60.0);
  int hourPixel = (hr / 12.0) * 60.0;
  
  int secondPixel = now.second();
  int minutePixel = now.minute();
  
  // Set seconds
  strip.setPixelColor(secondPixel, 127, 0, 0);
  
  // Set minutes
  strip.setPixelColor(minutePixel, (secondPixel == minutePixel ? 127 : 0), 127, 0);
  
  // Set hours
  strip.setPixelColor(hourPixel, (secondPixel == hourPixel ? 127 : 0), (minutePixel == hourPixel ? 127 : 0), 127);
  
//  Serial.print(now.second());
//  Serial.print(":");
//  Serial.print(now.minute());
//  Serial.print(":");
//  Serial.print(hr);
//  Serial.print("!");
//  Serial.println(((hr / 24.0) * 60.0));
  strip.show();
  
  if (lastSecond != secondPixel) {
    // If the second just changed -- pulse the minute.
    pulse(minutePixel, (secondPixel == minutePixel ? 127 : 0), 127, 0);
    lastSecond = secondPixel;
  }
}

void pulse (int pixel, float r, float g, float b) {
  float minDiv = 1.0;
  float maxDiv = 3.0;
  float currentDiv = 1.0;
  float stepDiv = 0.25;

  while (currentDiv < maxDiv) {
    currentDiv += stepDiv;
    strip.setPixelColor(pixel, r / currentDiv, g / currentDiv, b / currentDiv);
    strip.show();
    delay(25);
  }
  
  while (currentDiv > minDiv) {
    currentDiv -= stepDiv;
    strip.setPixelColor(pixel, r / currentDiv, g / currentDiv, b / currentDiv);
    strip.show();
    delay(25);
  }

  strip.setPixelColor(pixel, r, g, b);
  strip.show();
}
