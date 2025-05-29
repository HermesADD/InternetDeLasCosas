/*
 * Universidad Nacional Autónoma de México
 * Facultad de Ciencias
 * Licenciatura en Ciencias de la Computación
 * Seminario de Ciencias de la Computación A: Introducción al Internet de las Cosas
 *
 * Escrito por:
 *  Delgado Díaz Hermes Alberto
 *  López Montúfar José Eleazar
 * 
 * Version de bibliotecas: 
 * Arduinojson: 6.19.4
 * Placa 2.0.13
 * 
 * Descripción: Servidor web para control de 5 LEDs (3 digitales, 2 PWM) y monitoreo
 * de potenciómetro y fotoresistencia.
 */

// Bibliotecas ----------------------------------
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"
#include "ArduinoJson.h"

// Credenciales Red -----------------------------
const char* ssid = "INFINITUM1822";
const char* password = "Sp5Ea8Kj1g";

// Configuración de pines -----------------------
int leds[5] = {14, 27, 26, 25, 33}; // LEDs 1-5 (1-3 digital, 4-5 PWM)
const int potPin = 35;  // Potenciómetro
const int ldrPin = 34;  // Fotoresistencia

// Variables de estado --------------------------
String ledStates[] = {"off", "off", "off"}; // Estados de LEDs 1-3
String sliderValues[] = {"0", "0"}; // Valores PWM para LEDs 4-5
String sensorValues = "{}"; // JSON para valores de sensores

// PWM propiedades ------------------------------
const int freq = 1000;
const int ledChannels[] = {0, 1}; // Canales PWM para LEDs 4-5
const int resolution = 8;

// Parámetros HTTP ------------------------------
const char* PARAM_INPUT_STATE = "state";
const char* PARAM_INPUT_ID = "id";
const char* PARAM_INPUT_VALUE = "value";

// Objeto AsyncWebServer, puerto 80 ------------
AsyncWebServer server(80);

// Conexión WiFi -------------------------------
void setup_wifi() {
  WiFi.mode(WIFI_STA); // Modo estación
  WiFi.disconnect(); // Desconectar del modo AP (por si estaba conectado)
  delay(100);
  Serial.println("WiFi en modo estación establecido");
  
  delay(10);
  Serial.println();
  Serial.print("Conectando a: ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi conectado");
  Serial.println("Dirección IP: ");
  Serial.println(WiFi.localIP());
  delay(2000);
}

// Procesador de plantillas HTML ----------------
String processor(const String& var) {
  if (var == "LED1_STATE") return ledStates[0];
  if (var == "LED2_STATE") return ledStates[1];
  if (var == "LED3_STATE") return ledStates[2];
  if (var == "LED4_VALUE") return sliderValues[0];
  if (var == "LED5_VALUE") return sliderValues[1];
  if (var == "SENSOR_VALUES") return sensorValues;
  return String();
}

// Leer sensores y actualizar JSON --------------
void updateSensorData() {
  StaticJsonDocument<200> doc;
  doc["pot"] = analogRead(potPin);
  doc["ldr"] = analogRead(ldrPin);
  
  sensorValues = ""; // Limpiar antes de serializar
  serializeJson(doc, sensorValues);
}


void setup() {
  Serial.begin(115200);
  
  // Configurar pines
  for (int i = 0; i < 3; i++) {
    pinMode(leds[i], OUTPUT);
    digitalWrite(leds[i], LOW);
  }
  
  // Configurar PWM para LEDs 4 y 5
  for (int i = 0; i < 2; i++) {
    ledcSetup(ledChannels[i], freq, resolution);
    ledcAttachPin(leds[i+3], ledChannels[i]);
    ledcWrite(ledChannels[i], sliderValues[i].toInt());
  }
  
  // Configurar pines de sensores
  pinMode(potPin, INPUT);
  pinMode(ldrPin, INPUT);
  
  setup_wifi();
  
  // Inicializar SPIFFS
  if (!SPIFFS.begin(true)) {
    Serial.println("Error al leer SPIFFS");
    return;
  }

  // Rutas del servidor ------------------------
  
  // Raíz - Página principal
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  
  // Archivos estáticos
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/style.css", "text/css");
  });
  
  server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/script.js", "text/javascript");
  });
  
  // Control de LEDs digitales
  server.on("/led", HTTP_GET, [](AsyncWebServerRequest *request) {
    if (request->hasParam(PARAM_INPUT_ID) && request->hasParam(PARAM_INPUT_STATE)) {
      int id = request->getParam(PARAM_INPUT_ID)->value().toInt() - 1;
      String state = request->getParam(PARAM_INPUT_STATE)->value();
      
      if (id >= 0 && id < 3) {
        ledStates[id] = state;
        digitalWrite(leds[id], state == "on" ? HIGH : LOW);
      }
    }
    request->send(200, "text/plain", "OK");
  });
  
  // Control de LEDs PWM
  server.on("/slider", HTTP_GET, [](AsyncWebServerRequest *request) {
    if (request->hasParam(PARAM_INPUT_ID) && request->hasParam(PARAM_INPUT_VALUE)) {
      int id = request->getParam(PARAM_INPUT_ID)->value().toInt() - 4;
      String value = request->getParam(PARAM_INPUT_VALUE)->value();
      
      if (id >= 0 && id < 2) {
        sliderValues[id] = value;
        ledcWrite(ledChannels[id], value.toInt());
      }
    }
    request->send(200, "text/plain", "OK");
  });
  
  // Datos de sensores
  server.on("/sensors", HTTP_GET, [](AsyncWebServerRequest *request) {
    updateSensorData();
    request->send(200, "application/json", sensorValues);
  });
  
  // Iniciar servidor
  server.begin();
}

void loop() {
  // Actualizar datos de sensores periódicamente
  static unsigned long lastUpdate = 0;
  if (millis() - lastUpdate > 500) { // Actualizar cada 500ms
    updateSensorData();
    lastUpdate = millis();
  }
}
