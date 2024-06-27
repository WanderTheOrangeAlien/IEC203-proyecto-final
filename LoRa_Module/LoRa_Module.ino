#include <serial_util.h>

#include <HardwareSerial.h>

HardwareSerial SerialPort(2);

const int rx2Pin = 16;
const int tx2Pin = 17;

char buffer[64];
char buffer2[64];

SerialUtil myUtil(NL_AND_CR,true,false);
SerialUtil hardwareUtil(&SerialPort,NL_AND_CR,true,false);

void setup() {
  Serial.print(HEX);
  pinMode(4,OUTPUT);
  SerialPort.begin(115200,SERIAL_8N1,rx2Pin,tx2Pin);
  Serial.begin(115200);
  delay(1000);
  Serial.println("AT");

}

void loop() {
  
  myUtil.getSerialInput(buffer,64,&onSerialInput);
  hardwareUtil.getSerialInput(buffer2,64,&onSerial2Input);

  // digitalWrite(4, HIGH);
  // delay(200);
  // digitalWrite(4, LOW);
  // delay(200);


}

void onSerialInput(){

  SerialPort.print(buffer);
  Serial.print("TX: ");
  Serial.println(buffer);

}

void onSerial2Input(){

  Serial.print("RX: ");
  Serial.println(buffer2);


}
