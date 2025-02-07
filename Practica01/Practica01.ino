/*
 *Universidad Nacional Autónoma de México
 *Facultad de Ciencias
 *Licenciatura en Ciencias de la Computación
 *Seminario de Ciencias de la Computación A: Introducción al Internet de las Cosas
 *
 *Escrito por: M. en I. Valente Vázquez Velázquez
 *Última modificación: 1-enero-2023
 *https://sites.google.com/ciencias.unam.mx/introduccion-iot-fc/inicio
*/

//Defino pines (macro)
#define LED1 14


void setup() {
  //Declaro los pines como salida (OUTPUT)
  pinMode(LED1, OUTPUT);
}

void loop() {
  //Enciendo dos leds durante 200 ms
  digitalWrite(LED1, 1);
  delay(12);

  //Apago dos leds durante 200 ms
  digitalWrite(LED1, LOW);
  delay(12);
}
