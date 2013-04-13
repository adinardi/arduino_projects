#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE A0
#define COMPRESSOR_PIN 2
#define HIGH_TEMP 45 // 70
#define LOW_TEMP 37 //65

OneWire ds(ONE_WIRE);
DallasTemperature sensors(&ds);
bool compressorOn = false;
unsigned long compressorOnTime = 0;
unsigned long compressorOffTime = 0;
int offset = 7;

void setup(void) {
  Serial.begin(9600);
  pinMode(COMPRESSOR_PIN, OUTPUT);
  digitalWrite(COMPRESSOR_PIN, LOW);

  sensors.setResolution(TEMP_12_BIT);
  sensors.begin();
}

void loop(void) {
    sensors.requestTemperatures();
    float temp = sensors.getTempFByIndex(0);
    Serial.print("Current temp: ");
    Serial.println(temp);
    Serial.print(" / ");
    temp -= offset;
    Serial.println(temp);

    if (temp > HIGH_TEMP && compressorOn == false) {
        // TURN ON COMPRESSOR
        digitalWrite(COMPRESSOR_PIN, HIGH);

        compressorOnTime = millis();
        compressorOn = true;

        Serial.print("Compressor turned ON. Ran for minutes: ");
        Serial.println(((compressorOnTime - compressorOffTime) / 1000) / 60);
    } else if (temp < LOW_TEMP && compressorOn == true) {
        // TURN OFF COMPRESSOR
        digitalWrite(COMPRESSOR_PIN, LOW);

        compressorOffTime = millis();
        compressorOn = false;

        Serial.print("Compressor turned OFF. Ran for minutes: ");
        Serial.println(((compressorOffTime - compressorOnTime) / 1000) / 60);
    } // If temp was >LOW_TEMP and <HIGH_TEMP don't change anything.
}
