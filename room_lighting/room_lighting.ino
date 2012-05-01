#define REDPIN 10
#define GREENPIN 9
#define BLUEPIN 11
#define LIGHT1PIN 12
#define SWITCHPIN 2

int r = 0;

void setup()
{
  Serial.begin(19200);

  pinMode(REDPIN, OUTPUT);
  pinMode(GREENPIN, OUTPUT);
  pinMode(BLUEPIN, OUTPUT);
  
  pinMode(LIGHT1PIN, OUTPUT);
  pinMode(SWITCHPIN, INPUT);
  digitalWrite(LIGHT1PIN, LOW);

  analogWrite(REDPIN, 255);
  analogWrite(GREENPIN, 255);
  analogWrite(BLUEPIN, 255);
}

void loop()
{
//  for (r = 10; r <= 255; r++) {
//    analogWrite(REDPIN, r);
//    analogWrite(GREENPIN, r);
//    analogWrite(BLUEPIN, r);
//    delay(5);
//  }
//
//  for (r = 255; r >= 10; r--) {
//    analogWrite(REDPIN, r);
//    analogWrite(GREENPIN, r);
//    analogWrite(BLUEPIN, r);
//    delay(5);
//  }

  if(digitalRead(SWITCHPIN) == HIGH) {
    digitalWrite(LIGHT1PIN, HIGH);
  } else {
    digitalWrite(LIGHT1PIN, LOW);
  }
}
