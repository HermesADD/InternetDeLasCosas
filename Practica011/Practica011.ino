/*
  Universidad Nacional Autónoma de México
  Facultad de Ciencias
  Licenciatura en Ciencias de la Computación
  Seminario de Ciencias de la Computación A: Introducción al Internet de las Cosas

  Escrito por: Eleazar y Hermes
  Última modificación: 26-mayo-2025
  https://sites.google.com/ciencias.unam.mx/introduccion-iot-fc/inicio
*/

// Bibliotecas
#include <EspMQTTClient.h>   // Cliente MQTT para ESP32
#include <DHT.h>             // Sensor DHT
#include <DHT_U.h>           // Librería unificada DHT

// Definición de pines
#define DHTPIN         32   // Pin del sensor DHT
#define DHTTYPE     DHT11   // Modelo de sensor DHT
#define LED_PINS_COUNT 5    // Cantidad de LEDs
int ledPins[LED_PINS_COUNT] = {14, 27, 26, 25, 33};

#define BUTTON_LDR     15   // Botón para lectura LDR
#define BUTTON_POT      4   // Botón para lectura POT
#define LDR_PIN        34   // Pin analógico LDR
#define POT_PIN        35   // Pin analógico POT

// Credenciales Wi-Fi y broker MQTT
const char* ssid       = "INFINITUM1822";
const char* password   = "Sp5Ea8Kj1g";
const char* broker     = "broker.hivemq.com";
const char* mqttUser   = "";                // Usuario MQTT (si aplica)
const char* mqttPass   = "";                // Contraseña MQTT (si aplica)
const char* clientID   = "esp32-eleazar";
const int   mqttPort   = 1883;

// Tópicos MQTT
String baseTopic = "ClaseIoT/HE-FORCE";
String T_LDR     = baseTopic + "/Sensores/LDR";
String T_POT     = baseTopic + "/Sensores/Pot";
String T_DHT_TEMP= baseTopic + "/Sensores/DHT/Temperatura";
String T_DHT_HUM = baseTopic + "/Sensores/DHT/Humedad";
String T_Equipo  = baseTopic + "/Equipo";
String nombreEquipo = "HE-FORCE";

// Objetos globales
DHT dht(DHTPIN, DHTTYPE);
EspMQTTClient client(ssid, password, broker, mqttUser, mqttPass, clientID, mqttPort);

unsigned long lastPublish = 0;

void setup() {
  Serial.begin(115200);
  dht.begin();

  if (client.isConnected()) {
  Serial.println("✅ Conectado al broker MQTT");
} else {
  Serial.println("❌ No se pudo conectar al broker");
}


  Serial.println("Inicializando pines...");

  // Configuración de pines de LED
  for (int i = 0; i < LED_PINS_COUNT; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW);
  }

  // Configuración de botones
  pinMode(BUTTON_LDR, INPUT_PULLUP);
  pinMode(BUTTON_POT, INPUT_PULLUP);

  // Espera conexión Wi-Fi y MQTT
  if (client.isConnected()) {
    Serial.println("Conectado al broker MQTT!");
  } else {
    Serial.println("No se pudo conectar al broker, revisa tu red.");
  }
  delay(2000);
}

// Se ejecuta cuando la conexión MQTT está establecida
void onConnectionEstablished() {
  Serial.println("Suscribiéndose a tópicos de Actuadores/Led...");

  // Suscribir al control de todos los LEDs a la vez
  client.subscribe(baseTopic + "/Actuadores/Led/0", [](const String &payload) {
    Serial.printf("Recibido Led/0 -> %s\n", payload.c_str());
    for (int i = 0; i < LED_PINS_COUNT; i++) {
      digitalWrite(ledPins[i], (payload == "1") ? HIGH : LOW);
    }
  });

  // Suscribir control individual de cada LED
  for (int i = 0; i < LED_PINS_COUNT; i++) {
    int idx = i + 1;
    String topic = baseTopic + "/Actuadores/Led/" + String(idx);
    client.subscribe(topic, i, topic {
      Serial.printf("Recibido %s -> %s\n", topic.c_str(), payload.c_str());
      digitalWrite(ledPins[i], (payload == "1") ? HIGH : LOW);
    });
  }
}

void loop() {
  client.loop();

  // Lectura y publicación con botón LDR
  if (digitalRead(BUTTON_LDR) == HIGH) {
    float voltageldr = analogRead(LDR_PIN) * (3.3 / 4095.0);
    client.publish(T_LDR, String(voltageldr));
    Serial.printf("LDR: %.2f V publicado en %s\n", voltageldr, T_LDR.c_str());
    delay(500);
  }

  // Lectura y publicación con botón POT
  if (digitalRead(BUTTON_POT) == LOW) {
    float voltagepot = analogRead(POT_PIN) * (3.3 / 4095.0);
    client.publish(T_POT, String(voltagepot));
    Serial.printf("Potenciómetro: %.2f V publicado en %s\n", voltagepot, T_POT.c_str());
    delay(500);
  }

  // Publicar temperatura y humedad cada 15 segundos
  if (millis() - lastPublish >= 15000) {
    float temp = dht.readTemperature();
    float hum  = dht.readHumidity();
    if (!isnan(temp) && !isnan(hum)) {
      client.publish(T_DHT_TEMP, String(temp));
      client.publish(T_DHT_HUM,  String(hum));
      client.publish(T_Equipo, nombreEquipo);
      Serial.printf("Temperatura: %.2f °C en %s\n", temp, T_DHT_TEMP.c_str());
      Serial.printf("Humedad: %.2f %% en %s\n",     hum,  T_DHT_HUM.c_str());
    } else {
      Serial.println("Error al leer sensor DHT");
    }
    lastPublish = millis();
  }
}
