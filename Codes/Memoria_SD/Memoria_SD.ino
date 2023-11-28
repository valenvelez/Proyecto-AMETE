#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include <SPI.h>
File myFile;

String archivo = "/lol.txt";
String mensaje = "Datos grabados";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
   if(!SD.begin())
  {
    Serial.println("Error en inicialización");
  }
  else
  {
    Serial.println("OK");
  }
  myFile = SD.open(archivo, FILE_WRITE);

  if(myFile)
  {
    myFile.println(mensaje);
    myFile.println(mensaje);
    myFile.close();
    Serial.println("mensaje grabado en la SD");
  }
  else
  {
    Serial.println("error en grabado de los datos");
  }
  myFile.flush();
  
}

void loop() {
  // put your main code here, to run repeatedly:
 
  
}
