/*
 *Universidad Nacional Autónoma de México
 *Facultad de Ciencias
 *Licenciatura en Ciencias de la Computación
 *Seminario de Ciencias de la Computación A: Introducción al Internet de las Cosas
 *
 *  Escrito por: 
 *  Delgado Díaz Hermes Alberto
 *  López Montúfar José Eleazar
 *  
 *  Bibliotecas arduinojson 7.0.3
 *  Placa esp32 3.0.7
 *  
*/


#ifdef ENABLE_DEBUG
   #define DEBUG_ESP_PORT Serial
   #define NODEBUG_WEBSOCKETS
   #define NDEBUG
#endif 

#include <Arduino.h>
#if defined(ESP8266)
  #include <ESP8266WiFi.h>
#elif defined(ESP32) || defined(ARDUINO_ARCH_RP2040)
  #include <WiFi.h>
#endif

#include "SinricPro.h"
#include "SinricProSwitch.h"

#define WIFI_SSID         "INFINITUM1822"
#define WIFI_PASS         "Sp5Ea8Kj1g"
#define APP_KEY           "7e44c845-fa0c-44dc-bab6-5a061b04f7d8"
#define APP_SECRET        "6146ce8b-70ca-4a26-9c85-71498a186350-e619b24d-0301-4a6f-8112-3486fff900fb"

#define SWITCH_ID_1       "6837c9b4f64d827f967b87ae"
#define RELAYPIN_1        14

#define SWITCH_ID_2       "6837d89f6868ee0e88796cc9"
#define RELAYPIN_2        27

#define SWITCH_ID_3       "6837d8de6868ee0e88796cec"
#define RELAYPIN_3        26

#define BAUD_RATE         115200                // Change baudrate to your need

bool onPowerState1(const String &deviceId, bool &state) {
 Serial.printf("Device 1 turned %s", state?"on":"off");
 digitalWrite(RELAYPIN_1, state ? HIGH:LOW);
 return true; // request handled properly
}

bool onPowerState2(const String &deviceId, bool &state) {
 Serial.printf("Device 2 turned %s", state?"on":"off");
 digitalWrite(RELAYPIN_2, state ? HIGH:LOW);
 return true; // request handled properly
}

bool onPowerState3(const String &deviceId, bool &state) {
 Serial.printf("Device 3 turned %s", state?"on":"off");
 digitalWrite(RELAYPIN_3, state ? HIGH:LOW);
 return true; // request handled properly
}

// setup function for WiFi connection
void setupWiFi() {
  Serial.printf("\r\n[Wifi]: Connecting");

  #if defined(ESP8266)
    WiFi.setSleepMode(WIFI_NONE_SLEEP); 
    WiFi.setAutoReconnect(true);
  #elif defined(ESP32)
    WiFi.setSleep(false); 
    WiFi.setAutoReconnect(true);
  #endif

  WiFi.begin(WIFI_SSID, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.printf(".");
    delay(250);
  }

  Serial.printf("connected!\r\n[WiFi]: IP-Address is %s\r\n", WiFi.localIP().toString().c_str());
}

// setup function for SinricPro
void setupSinricPro() {
  // add devices and callbacks to SinricPro
  pinMode(RELAYPIN_1, OUTPUT);
  pinMode(RELAYPIN_2, OUTPUT);
  pinMode(RELAYPIN_3, OUTPUT);
    
  SinricProSwitch& mySwitch1 = SinricPro[SWITCH_ID_1];
  mySwitch1.onPowerState(onPowerState1);
  
  SinricProSwitch& mySwitch2 = SinricPro[SWITCH_ID_2];
  mySwitch2.onPowerState(onPowerState2);
  
  SinricProSwitch& mySwitch3 = SinricPro[SWITCH_ID_3];
  mySwitch3.onPowerState(onPowerState3);
  
  
  // setup SinricPro
  SinricPro.onConnected([](){ Serial.printf("Connected to SinricPro\r\n"); }); 
  SinricPro.onDisconnected([](){ Serial.printf("Disconnected from SinricPro\r\n"); });
   
  SinricPro.begin(APP_KEY, APP_SECRET);
}

// main setup function
void setup() {
  Serial.begin(BAUD_RATE); Serial.printf("\r\n\r\n");
  setupWiFi();
  setupSinricPro();
}

void loop() {
  SinricPro.handle();
}
