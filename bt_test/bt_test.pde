#include <NewSoftSerial.h>

NewSoftSerial bt(7, 6);

void setup()
{
  Serial.begin(57600);
  Serial.println("Starting");
  bt.begin(19200);
  
  pinMode(7, INPUT);
  pinMode(6, OUTPUT);
 
  bt.print("AT\r");
  delay(3000);
  
  bt.print("ATR0\r");
  delay(1000);
  
//  bt.print("ATI1\r");
//  delay(3000);
//  while(bt.available())
//  {
//    Serial.print((char)bt.read());
//  }
//  
//  delay(2000);
//  
  bt.print("ATO1\r");
  delay(3000);
  while(bt.available())
  {
     Serial.print((char)bt.read()); 
  }
}

void loop()
{
  Serial.println("\n\nBT Devices");

  bt.print("ATF?\r");
  delay(3000);
  String data = String("");
  while (data.indexOf("Inquiry End") < 0 || data.indexOf("Found.") < 0)
  {
    while(bt.available())
    {
       char in = bt.read();
       Serial.print(in);
       data.concat(in); 
    }
  }

  delay(2000);
}
