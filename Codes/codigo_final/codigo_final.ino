#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <BluetoothSerial.h>
#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include "virtuabotixRTC.h"

virtuabotixRTC myRTC(22, 21, 25);

BluetoothSerial SerialBT;

#define Datostemp 4
#define echoPin 18
#define trigPin 5
#define valve 14
long duration, distance, distanciamm;
const int interval =  60 * 1000; // Intervalo de 1 minuto en milisegundos

OneWire oneWireObjeto(Datostemp);
DallasTemperature sensortemp(&oneWireObjeto);

String archivo = "/datos.txt";
unsigned long previousMillis = 0;

float datos[4] = {2, 2, 2, 2};
float suma = 0;
float promedio = 0;
float voltaje = 0;
float resolucion = 0;

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
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    // Guarda la última vez que se ejecutó el código
    previousMillis = currentMillis;

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
    String disp = String(resolucion);
    String disp2 = String(distanciamm);

    Serial.println("Mandando comandos a los sensores");
    sensortemp.requestTemperatures();

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
    Serial.print(resolucion);
    Serial.print("; ");
    Serial.println(disp2);

    SerialBT.print("Temperatura: ");
    SerialBT.print(temperatura);
    SerialBT.println(" C");
    SerialBT.print("Distance: ");
    SerialBT.print(resolucion);
    SerialBT.println(" cm");
    SerialBT.print("Distance: ");
    SerialBT.print(disp2);
    SerialBT.println(" mm");

    File myFile = SD.open(archivo, FILE_APPEND);
    Serial.println("Grabando SD...");
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
      myFile.print(resolucion);
      myFile.print("; ");
      myFile.println(disp2);
      myFile.close();
    } else {
      Serial.println("Error al abrir el archivo.");
    }

    if (distanciamm < 100) {
      digitalWrite(valve, HIGH);
    }
    if (distanciamm > 480) {
      digitalWrite(valve, LOW);
    }

    digitalWrite(13, LOW);
  }

  // Código para lectura del sensor de voltaje
  voltaje = 0.0048875855327468 * analogRead(15);
  resolucion = (analogRead(15) - 181) * 2.1983;
  suma = 0;
  promedio = 0;

  for (int i = 0; i < 4; i++) {
    datos[i] = resolucion;
    suma += datos[i];
  }
  promedio = suma / 4;

  Serial.println(analogRead(15));
  Serial.println(resolucion);
  delay(1000);
}
