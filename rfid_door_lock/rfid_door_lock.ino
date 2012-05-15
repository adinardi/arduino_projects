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
  if(RFIDReader.available()) {
     if (RFIDReader.read() == 10) { // start bit.
       bytesread = 0; // reset read state.
       tag = "";
       while (bytesread < 10) { // Loop until we get the full tag data.
         if (RFIDReader.available()) { // Have we gotten the next bit?
           val = RFIDReader.read(); // Get the next bit.

           if (val == 10 || val == 13) { // If we get a start or end bit -- dive out and stop trying to get more bits.
             break; 
           }
           
           tag.concat(val); // store the tag bit.
           bytesread++;
         }
       }
       
       if (bytesread == 10) { // got full tag length.
         digitalWrite(PIN_RFID_ENABLE, HIGH); // Stop reading rfid input.
         
         BlueTooth.println("Tag " + tag);
         
         processTag();
         
         digitalWrite(PIN_RFID_ENABLE, LOW); // read RFID input again.
       }
     }
  }
  
  int door_state = digitalRead(PIN_DOOR_SENSOR_SENSE);
//  BlueTooth.println(door_state == HIGH ? "Door state: HIGH" : "Door state: LOW");
  if (isOpen == 1 && door_state == HIGH) {
    delay(1000);
    // Check that the door is still closed (effectively debouncing).
    if (digitalRead(PIN_DOOR_SENSOR_SENSE) == HIGH) {
      lockDoor();
      isOpen = 0;
    }
  } else if (door_state == LOW) {
    // Wait to "de-bounce"
    delay(100);
    // Check again and proclaim OPEN if still really open.
    if (digitalRead(PIN_DOOR_SENSOR_SENSE) == LOW) {
      isOpen = 1;
    }
  }
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
    BlueTooth.println("DENIED"); 
  }
}

void openDoor() {
  BlueTooth.println("OPEN DOOR");
  door.attach(PIN_SERVO_SIGNAL);
  
  // Open
  door.write(35);
  delay(2000);
  
  door.detach();
}

void lockDoor() {
  BlueTooth.println("LOCK DOOR");
  door.attach(PIN_SERVO_SIGNAL);
  
  // Close
  door.write(160);
  delay(2000);
  
  door.detach();
}
