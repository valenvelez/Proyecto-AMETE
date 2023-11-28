#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <BluetoothSerial.h>  // Biblioteca para la comunicación serie con Bluetooth
BluetoothSerial SerialBT;

#define Datostemp 4
#define echoPin 18             // CHANGE PIN NUMBER HERE IF YOU WANT TO USE A DIFFERENT PIN
#define trigPin 5 
#define valve 14
long duration, distance, distanciamm;
const int sleeptime = 60*1000000; //uS

//Creacion de objetos
//Cuando una variable la pasamos con el símbolo &nombreVariable, estamos pasando la posición de memoria donde se almacenan los datos y no su valor. A esto se le llama paso por referencia.

OneWire oneWireObjeto(Datostemp);
DallasTemperature sensortemp(&oneWireObjeto);
 
void setup() {
    
    Serial.begin(9600);
//inicialización sensor
    SerialBT.begin("EVAPORIMETRO");
    sensortemp.begin(); 
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(valve,OUTPUT);
  pinMode (13,OUTPUT);
}
 
void loop() {
  esp_sleep_enable_timer_wakeup(sleeptime);
  
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  digitalWrite(13, HIGH);

  
  duration = pulseIn(echoPin, HIGH);
  distance = duration / 58.2;
  distanciamm = duration* 0.343 / 2.0;
  String disp = String(distance);
  String disp2 = String(distanciamm);
  
  
    // preparar el sensor de temp
    Serial.println("Mandando comandos a los sensores");
    sensortemp.requestTemperatures();
 
    // Leemos y mostramos los datos del sensor de temp
    Serial.print("Temperatura: ");
    Serial.print(sensortemp.getTempCByIndex(0));
    Serial.println(" C");
    Serial.print("Distance: ");
    Serial.print(disp);
    Serial.println(" cm");
    Serial.print("Distance: ");
    Serial.print(disp2);
    Serial.println(" mm");
      // Enviar datos por Bluetooth
    SerialBT.print("Temperatura: ");
    SerialBT.print(sensortemp.getTempCByIndex(0));
    SerialBT.println(" C");
    SerialBT.print("Distance: ");
    SerialBT.print(disp);
    SerialBT.println(" cm");
    SerialBT.print("Distance: ");
    SerialBT.print(disp2);
    SerialBT.println(" mm");

    delay(1000); 
    if(distanciamm<100)
    {
      digitalWrite(valve,HIGH);
    }
    if(distanciamm>480)
    {
      digitalWrite(valve,LOW);
    }

  digitalWrite(13, LOW);
esp_deep_sleep_start();
}
