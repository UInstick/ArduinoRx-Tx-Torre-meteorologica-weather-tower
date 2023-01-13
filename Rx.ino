
#include <SPI.h>
#include <Ethernet.h>
#include <SD.h>
#include "RF24.h"
#include <ArduinoJson.h>
//RF24l021 radio RX
int data[4];  //Vector de datos recibidos 
RF24 radio(7, 8);// Pines de habilitacion de Radio
//Ethernet
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 0, 177); //IP de arduino
EthernetServer server(80); //Puerto de comunicacion

void setup() {
  Serial.begin(9600);
  //Ethernet
  Ethernet.begin(mac, ip); //Inicializamos el shield ethernet
  server.begin(); //Inicializamos el servidor
  //Radio RX
  radio.begin();
  radio.openReadingPipe(1, 0xF0F0F0F0D2LL);//Modo de operacion RX
  radio.setChannel(115); // Canal de transmision 115
  radio.setPALevel(RF24_PA_LOW); // Establecemos la potencia de amplificador en bajo
  radio.startListening(); // Modo RX
  
  
  //Modulo SD
  Serial.print("Iniciando tarjeta SD...");
  if (!SD.begin(4)) { //Preguntamos si hay una tarjeta en el puerto
    Serial.println("Inizializacion fallida");
   return;
   
  }
  Serial.println("Tarjeta detectada");
}

void loop() {
  //Modo de grabado para la tarjeta SD
  int start;
  start = Serial.read();
  if(start == '1'){
    SD.begin(4);
    Serial.print("Modo guardado");
    }
 //Creacion del documento contenedor de la DATA    
  File myFile;
  myFile = SD.open("data.txt", FILE_WRITE); //abrimos  el archivo
  EthernetClient client = server.available();
  
 if (radio.available()) { //Condicionamos el programa para que solo guarde informacion cuando el radio este conectado

  radio.read(&data, sizeof(data)); //Recepsionamos el vector de informacion

  //Conversion a Json
  StaticJsonDocument<200> doc; //Establecemos el tamaño de memoria para el doc Json
      doc["sensor1"] = data[0];
      doc["sensor2"] = data[1];
      doc["sensor3"] = data[2];
      doc["sensor4"] = data[3];
      String output; //Transformamos a String para imprimirlo
      serializeJson(doc, output);
  //Ploteo    
      Serial.println(output); //Imprimimos en el plotter de Arduino
      myFile.println(output); //Imprimimos en el archivo creado en la SD
      myFile.close(); //cerramos los datos recibidos por este ciclo
      delay(1000); //Establecemos un retardo de 1 segundo para no saturar los puertos
      }
    client.stop(); // En caso de no tener el radio conectado detenemos comunicacion con el servidor
  }
  
