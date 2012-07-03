#include <Servo.h>
#include <SoftwareSerial.h>
#include "rfid_codes.h"

#define PIN_RFID_ENABLE 4
#define PIN_RFID_SERIAL 5 // RX
#define PIN_RFID_EMPTY_SERIAL 6 // would be TX (not really used)
#define PIN_DOOR_CONTROL 2

#define PIN_BT_RX 8
#define PIN_BT_TX 9

#define PIN_SERVO_SIGNAL 12

#define PIN_DOOR_SENSOR_ON 10
#define PIN_DOOR_SENSOR_SENSE 11

SoftwareSerial RFIDReader(PIN_RFID_SERIAL, PIN_RFID_EMPTY_SERIAL);
SoftwareSerial BlueTooth(PIN_BT_RX, PIN_BT_TX);

Servo door;

int  val = 0;
char code[10];
String tag = "";
int bytesread = 0;
int isOpen = 0;

bool isReadingTag = false;
long doorDebounceDelay = 100;
long lastDoorDebounceTime = 0;
int lastDoorState = HIGH;

void setup() {
  pinMode(PIN_DOOR_CONTROL, OUTPUT); // Door controller
  pinMode(PIN_RFID_SERIAL, INPUT); // RFID serial in
  pinMode(PIN_RFID_ENABLE, OUTPUT); // RFID enable
  pinMode(PIN_DOOR_SENSOR_ON, OUTPUT); // On signal
  pinMode(PIN_DOOR_SENSOR_SENSE, INPUT);

  digitalWrite(PIN_RFID_ENABLE, LOW); // Enable the RFID Reader.
  digitalWrite(PIN_DOOR_CONTROL, LOW); // Don't open the door.
  digitalWrite(PIN_DOOR_SENSOR_ON, HIGH); // Push out 5v

  Serial.begin(9600);
  RFIDReader.begin(2400);
  BlueTooth.begin(19200); //open serial software

  RFIDReader.listen();

  door.attach(PIN_SERVO_SIGNAL);
  door.write(160);
  delay(2000);
  door.detach();

  delay(1000);
  BlueTooth.write("AT\r");
}

void loop() {
  if (RFIDReader.available()) {
    int nextByte = RFIDReader.read();

    if (nextByte == 13) { // End of tag.
      isReadingTag = false; // Done reading.

      if (bytesread == 10) { // Make sure we got 10 bytes.
        digitalWrite(PIN_RFID_ENABLE, HIGH); // Stop reading rfid input.
        // BlueTooth.println("Got Tag: " + tag);
        processTag();
        digitalWrite(PIN_RFID_ENABLE, LOW); // read RFID input again.
      }

    } else if (nextByte == 10) { // Tag start.
      isReadingTag = true;
      bytesread = 0;
      tag = "";

    } else if (isReadingTag) { // If this wasn't the start or the end -- and we had a start, collect the bytes.
      tag.concat(nextByte);
      bytesread++;
    }
  }

  int door_state = digitalRead(PIN_DOOR_SENSOR_SENSE);

  if (door_state != lastDoorState) {
    lastDoorDebounceTime = millis();
  }

  if ((millis() - lastDoorDebounceTime) > doorDebounceDelay) {
    if (isOpen == 1 && door_state == HIGH) { // Door was last open, now apparently closed.
      delay(900);
      lockDoor();
      isOpen = 0;
    } else if (door_state == LOW) {
      isOpen = 1;
    }
  }

  lastDoorState = door_state;
}

void processTag() {
  int isValid = 0;

  for (int i = 0; rfid_tags[i] != 0; i++) {
    String check_tag = rfid_tags[i];
    if (check_tag == tag) {
      isValid = 1;
      break;
    }
  }

  if (isValid) {
    openDoor();
  } else {
    // BlueTooth.println("DENIED");
  }
}

void openDoor() {
  // BlueTooth.println("OPEN DOOR");
  door.attach(PIN_SERVO_SIGNAL);

  // Open
  door.write(35);
  delay(2000);

  door.detach();
}

void lockDoor() {
  // BlueTooth.println("LOCK DOOR");
  door.attach(PIN_SERVO_SIGNAL);

  // Close
  door.write(160);
  delay(2000);

  door.detach();
}
