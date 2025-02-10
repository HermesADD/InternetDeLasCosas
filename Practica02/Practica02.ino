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

//Defino pines 
#define led1 14
#define led2 27
#define led3 26
#define led4 25
#define led5 33

//Defino los leds de uso en un arreglo
const int leds[5] = {led1, led2, led3, led4, led5};

void setup() {
  //Defino mis pines como salida dentro de un ciclo for
  for (int i = 0; i < 5 ; i++) {
    pinMode(leds[i], OUTPUT);
  }
}

void loop() {
  //Ciclo for para encender 5 leds y esperar 200 ms
  for (int i = 0; i < 5; i++) {
    digitalWrite(leds[i], HIGH);
    delay(20);
    digitalWrite(leds[i], LOW);
    delay(20);
  }
  delay(200);

  //Ciclo for para apagar 5 leds y esperar 200 ms
  for (int i = 4; i >= 0; i--) {
    digitalWrite(leds[i], HIGH);
    delay(20);
    digitalWrite(leds[i], LOW);
    delay(20);
  }
  delay(200);
}
