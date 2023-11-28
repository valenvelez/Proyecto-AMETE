#include "virtuabotixRTC.h"
// SCLK -> 6, I/O -> 7, CE -> 8
// CLK -> 6 , DAT -> 7, Reset -> 8
virtuabotixRTC myRTC(22, 21, 25);

void setup() {
 Serial.begin(9600);
 // Formato para la fecha:
 // segundos, minutos, horas, día de la semana, día del mes, mes, año
 myRTC.setDS1302Time(30, 46, 18, 6, 25, 11, 2023); //Configuración del tiempo actual 
 // Recuerda comentar la línea anterior, una vez configurado
}

void loop() {
// myRTC.updateTime();
//
// Serial.print("Fecha / Hora: ");
// Serial.print(myRTC.dayofmonth);
// Serial.print("/");
// Serial.print(myRTC.month);
// Serial.print("/");
// Serial.print(myRTC.year);
// Serial.print(" ");
// Serial.print(myRTC.hours);
// Serial.print(":");
// Serial.print(myRTC.minutes);
// Serial.print(":");
// Serial.println(myRTC.seconds);
//
// delay(1000);
}
