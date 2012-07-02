#define REDPIN 10
#define GREENPIN 9
#define BLUEPIN 11
#define LIGHT1PIN 12
#define SWITCHPIN 2
#define TOUCH_Y1 A2
#define TOUCH_X2 A3
#define TOUCH_Y2 A4
#define TOUCH_X1 A5

int r = 0;
int onState = 0;
int processedStateChange = 0;

void setup()
{
  Serial.begin(19200);

  pinMode(REDPIN, OUTPUT);
  pinMode(GREENPIN, OUTPUT);
  pinMode(BLUEPIN, OUTPUT);
  
  pinMode(LIGHT1PIN, OUTPUT);
  pinMode(SWITCHPIN, INPUT);
  digitalWrite(LIGHT1PIN, LOW);

  analogWrite(REDPIN, 0);
  analogWrite(GREENPIN, 0);
  analogWrite(BLUEPIN, 0);
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

//  Serial.print("X:");
//  Serial.print(getTouchX());
//  Serial.print(", Y:");
//  Serial.println(getTouchY());
//  boolean hasTouch = (getTouchX() || getTouchY());
  boolean hasTouch = (getTouchY() > 50);
  if (hasTouch && processedStateChange == 0) {
    processedStateChange = 1;
    if (onState == 0) {
      onState = 1;
      digitalWrite(LIGHT1PIN, HIGH);
      analogWrite(REDPIN, 255);
      analogWrite(GREENPIN, 255);
      analogWrite(BLUEPIN, 255);
    } else if (onState == 1) {
      onState = 0;
      digitalWrite(LIGHT1PIN, LOW);
      analogWrite(REDPIN, 0);
      analogWrite(GREENPIN, 0);
      analogWrite(BLUEPIN, 0);
    }
  } else if (!hasTouch) {
     processedStateChange = 0;
  }
  
  delay(100);

//  if (digitalRead(SWITCHPIN) == HIGH && processedStateChange == 0) {
//    delay(5);
//    if (digitalRead(SWITCHPIN) == LOW) {
//      return;
//    }
//    processedStateChange = 1;
//    if (onState == 0) {
//      onState = 1;
//      digitalWrite(LIGHT1PIN, HIGH);
//      analogWrite(REDPIN, 255);
//      analogWrite(GREENPIN, 255);
//      analogWrite(BLUEPIN, 255);
//    } else if (onState == 1) {
//      onState = 0;
//      digitalWrite(LIGHT1PIN, LOW);
//      analogWrite(REDPIN, 0);
//      analogWrite(GREENPIN, 0);
//      analogWrite(BLUEPIN, 0);
//    }
//  } else if (digitalRead(SWITCHPIN) == LOW) {
//    processedStateChange = 0; 
//  }
}

void fadeUpLEDs() {
  for (r = 10; r <= 255; r++) {
    analogWrite(REDPIN, r);
    analogWrite(GREENPIN, r);
    analogWrite(BLUEPIN, r);
    delay(1);
  }
}

int getTouchX() {
  pinMode(TOUCH_X1, OUTPUT);
  pinMode(TOUCH_X2, OUTPUT);
  pinMode(TOUCH_Y1, INPUT);
  pinMode(TOUCH_Y2, INPUT);
  
  digitalWrite(TOUCH_Y1, LOW);
  digitalWrite(TOUCH_Y2, LOW);
  
  digitalWrite(TOUCH_X1, HIGH);
  digitalWrite(TOUCH_X2, LOW);
  
  delay(10);
  return analogRead(TOUCH_Y2);
}

int getTouchY() {
  pinMode(TOUCH_Y1, OUTPUT);
  pinMode(TOUCH_Y2, OUTPUT);
  pinMode(TOUCH_X1, INPUT);
  pinMode(TOUCH_X2, INPUT);
  
  digitalWrite(TOUCH_X1, LOW);
  digitalWrite(TOUCH_X2, LOW);
  
  digitalWrite(TOUCH_Y1, HIGH);
  digitalWrite(TOUCH_Y2, LOW);
  
  delay(10);
  return analogRead(TOUCH_X1);
}
