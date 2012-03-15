#include <SoftwareSerial.h>

#define rxPin 8
#define txPin 9

#define REDPIN 5
#define GREENPIN 6
#define BLUEPIN 3


SoftwareSerial BlueTooth(rxPin,txPin);
char inByte;
String command;

void setup()
{
  Serial.begin(19200);
  BlueTooth.begin(19200); //open serial software
  
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  
  pinMode(REDPIN, OUTPUT);
  pinMode(GREENPIN, OUTPUT);
  pinMode(BLUEPIN, OUTPUT);
  
//  BlueTooth.print(“BLAM”); //Communication returns OK
  delay(1000);
  BlueTooth.write("AT\r");
}

void loop()
{
  boolean hasCommand = false;
  
  if (BlueTooth.available()) {
//    Serial.write(BlueTooth.read());
    inByte = BlueTooth.read();
    if (inByte == 10 || inByte == 13) {
      inByte = 0;
      hasCommand = true;
    }
    
    command.concat(inByte);
  }
  
  if (hasCommand) {
    Serial.println(command);
    
    if (command.startsWith("r")) {
      char color[4];
      command.substring(1).toCharArray(color, 4);
      Serial.println(atoi(color));
      analogWrite(REDPIN, atoi(color));
    }
    
    if (command.startsWith("g")) {
      char color[4];
      command.substring(1).toCharArray(color, 4);
      Serial.println(atoi(color));
      analogWrite(GREENPIN, atoi(color));
    }
    
    if (command.startsWith("b")) {
      char color[4];
      command.substring(1).toCharArray(color, 4);
      Serial.println(atoi(color));
      analogWrite(BLUEPIN, atoi(color));
    }
    
    command = "";
    hasCommand = false;
  }
  
//  if (Serial.available()) {
//    BlueTooth.write(Serial.read());
//  }
}
