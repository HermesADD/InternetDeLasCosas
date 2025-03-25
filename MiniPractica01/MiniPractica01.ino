/*
 *Universidad Nacional Autónoma de México
 *Facultad de Ciencias
 *Licenciatura en Ciencias de la Computación
 *Seminario de Ciencias de la Computación A: Introducción al Internet de las Cosas
 *
 *Escrito por: 
 *  Delgado Díaz Hermes Alberto
 *  López Montúfar José Eleazar
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
  delay(11);

  //Apago dos leds durante 200 ms
  digitalWrite(LED1, LOW);
  delay(11);
}
//Período total: 11ms + 11ms = 22ms = 0.022s
//Frecuencia: 1/0.022 = 45.45Hz
