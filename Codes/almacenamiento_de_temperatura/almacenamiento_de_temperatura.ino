#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <BluetoothSerial.h>  // Biblioteca para la comunicación serie con Bluetooth
#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include "virtuabotixRTC.h"
// SCLK -> 6, I/O -> 7, CE -> 8
// CLK -> 6 , DAT -> 7, Reset -> 8
virtuabotixRTC myRTC(22, 21, 25);



BluetoothSerial SerialBT;

#define Datostemp 4
#define echoPin 18             // CHANGE PIN NUMBER HERE IF YOU WANT TO USE A DIFFERENT PIN
#define trigPin 5
#define valve 14
long duration, distance, distanciamm;
const int sleeptime = 60 * 1000000; //uS


OneWire oneWireObjeto(Datostemp);
DallasTemperature sensortemp(&oneWireObjeto);

String archivo = "/datos.txt";

void setup() {
  Serial.begin(9600);
  SerialBT.begin("EVAPORIMETRO");
  sensortemp.begin();

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(valve, OUTPUT);
  pinMode(13, OUTPUT);

  if (!SD.begin()) {
    Serial.println("Error en inicialización de la tarjeta SD");
  } else {
    Serial.println("Tarjeta SD inicializada correctamente");
  }

  // Abre el archivo en modo escritura
  File myFile = SD.open(archivo, FILE_WRITE);

  if (myFile) {
    myFile.println("Inicio de la captura de datos:");
    myFile.print("Fecha y hora;");
    myFile.print("Temperatura[°C];");
    myFile.print("Distance[cm];");
    myFile.println("Distance[mm];");
    myFile.close();
    Serial.println("Mensaje inicial grabado en la SD");
  } else {
    Serial.println("Error al abrir el archivo.");
  }
 
}

void loop() {

   myRTC.updateTime();
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  digitalWrite(13, HIGH);
  Serial.print("prueba hora");
  Serial.println(myRTC.hours);
  duration = pulseIn(echoPin, HIGH);
  distance = duration / 58.2;
  distanciamm = duration * 0.343 / 2.0;
  String disp = String(distance);
  String disp2 = String(distanciamm);

  Serial.println("Mandando comandos a los sensores");
  sensortemp.requestTemperatures();

  // Leemos y mostramos los datos del sensor de temperatura
  float temperatura = sensortemp.getTempCByIndex(0);
  Serial.print("Fecha y hora;");
  Serial.print("Temperatura[°C];");
  Serial.print("Distance[cm];");
  Serial.println("Distance[mm];");
  Serial.print(myRTC.dayofmonth);
  Serial.print("/");
  Serial.print(myRTC.month);
  Serial.print("/");
  Serial.print(myRTC.year);
  Serial.print(" ");
  Serial.print(myRTC.hours);
  Serial.print(":");
  Serial.print(myRTC.minutes);
  Serial.print(":");
  Serial.print(myRTC.seconds);
  Serial.print("; ");
  Serial.print(temperatura);
  Serial.print("; ");
  Serial.print(disp);
  Serial.print("; ");
  Serial.println(disp2);
  

  // Enviar datos por Bluetooth
  SerialBT.print("Temperatura: ");
  SerialBT.print(temperatura);
  SerialBT.println(" C");
  SerialBT.print("Distance: ");
  SerialBT.print(disp);
  SerialBT.println(" cm");
  SerialBT.print("Distance: ");
  SerialBT.print(disp2);
  SerialBT.println(" mm");

  // Abrir el archivo en modo escritura y agregar los datos
  File myFile = SD.open(archivo, FILE_APPEND);
    Serial.println("Grabando SD...");
//    myFile.print("Fecha y hora;");
//    myFile.print("Temperatura[°C];");
//    myFile.print("Distance[cm];");
//    myFile.println("Distance[mm];");
  if (myFile) {

    myFile.print(myRTC.dayofmonth);
    myFile.print("/");
    myFile.print(myRTC.month);
    myFile.print("/");
    myFile.print(myRTC.year);
    myFile.print(" ");
    myFile.print(myRTC.hours);
    myFile.print(":");
    myFile.print(myRTC.minutes);
    myFile.print(":");
    myFile.print(myRTC.seconds);
    myFile.print("; ");
    myFile.print(temperatura);
    myFile.print("; ");
    myFile.print(disp);
    myFile.print("; ");
    myFile.println(disp2);
    myFile.close();
  } else {
    Serial.println("Error al abrir el archivo.");
  }

  delay(10000);

  if (distanciamm < 100) {
    digitalWrite(valve, HIGH);
  }
  if (distanciamm > 480) {
    digitalWrite(valve, LOW);
  }

  digitalWrite(13, LOW);

}
