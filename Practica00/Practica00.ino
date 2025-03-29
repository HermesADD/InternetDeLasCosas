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
#include <DHT.h>
#include <DHT_U.h>

#include "DHT.h"
#include "BluetoothSerial.h"

#define DHTPIN 32
#define DHTTYPE DHT11 
DHT dht(DHTPIN, DHTTYPE); 

//Condición para habilitar el bluetooth
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

//Definir pines
#define TEMP 15 //Botón para temperatura
#define HUM 4 //Botón para humedad

BluetoothSerial BT; // Objeto Bluetooth

//Variables para el funcionamiento del botón
long timeCounter1 = 0;
long timeCounter2 = 0;
int timeThreshold = 250;
int leds[] = {14,27,26,25,33};

void setup() {
  Serial.begin(115200);
  BT.begin("ESP32HE");
  Serial.println("Bluetooth listo para emparejar!");

  dht.begin();
  for(int i = 0; i<5;i++){
    pinMode(leds[i], OUTPUT);
    digitalWrite(leds[i], LOW);
  }
  
  pinMode(TEMP, INPUT);
  pinMode(HUM, INPUT);
}

void loop() {
  if(digitalRead(TEMP) == 1 && millis()> timeCounter1 + timeThreshold){
    float t = dht.readTemperature();
    if(!isnan(t)){
      BT.println("Temperatura: " + String(t) + "ºC");
    }else{
      BT.println("Error al leer la temperatura. :(");
    }
    timeCounter1 = millis();
  }

  if(digitalRead(HUM) == 0 && millis()> timeCounter2 + timeThreshold){
    float h = dht.readHumidity();
    if(!isnan(h)){
      BT.println("Humedad: " + String(h) + "%");
    }else{
      BT.println("Error al leer la humedad. :(");
    }
    timeCounter2 = millis();
  }

  if(BT.available()){
    String entrada = BT.readStringUntil('\n');
    entrada.trim(); // Elimina espacios y \r

    if (entrada.length() == 1) {
      funcionLeds(entrada.charAt(0));
    } else {
      BT.println("Comando no identificado. Usa 1-5 para LEDS y 0 para apagar todos.");
    }
  }
  
}

void funcionLeds(char entrada){
  if (entrada == '\n' || entrada == '\r') return;
  if(entrada >= '1' && entrada <= '5' ){
    int led = entrada - '1';
    digitalWrite(leds[led],!digitalRead(leds[led]));
    BT.println(String("LED ") + (led + 1) + (digitalRead(leds[led]) ? " encendido" : " apagado"));
  }else if(entrada == '0' ){
    for(int i = 0; i<5; i++){
      digitalWrite(leds[i],LOW);
    } 
    BT.println("Todos los LEDS han sido apagados");
  }else{
    BT.println("Comando no identificado. Usa 1-5 para LEDS y 0 para apagar todos.");
  }
}
