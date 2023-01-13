#include <SPI.h>
#include "RF24.h"

RF24 radio(7,8);
int temp = 0;
int humidity;
int data[4];
float humedad;
int lluv;
int vel;
void setup() {

radio.begin();
Serial.begin(9600);
radio.openWritingPipe(0xF0F0F0F0D2LL);
radio.setChannel(115);
radio.setPALevel(RF24_PA_LOW);
radio.stopListening();

}

void loop() {

  temp = analogRead(A0);
  humidity = analogRead(A1);
  humedad = (humidity/1023.0)*100; 
  vel = analogRead(A2);
  lluv= analogRead(A3);
  data[0]= temp;
  data[1]= humidity;
  data[2]= vel;
  data[4]= lluv;

radio.write(&data, sizeof(data));

Serial.print(temp);
Serial.print(" ");
Serial.print(humidity);
Serial.print(" ");
Serial.print(vel);
Serial.print(" ");
Serial.println(lluv);

}
