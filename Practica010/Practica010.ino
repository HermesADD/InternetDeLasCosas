/*
 *Universidad Nacional Autónoma de México
 *Facultad de Ciencias
 *Licenciatura en Ciencias de la Computación
 *Seminario de Ciencias de la Computación A: Introducción al Internet de las Cosas
 *
 *Escrito por: 
 *  Delgado Díaz Hermes Alberto
 *  López Montúfar José Eleazar
 *  
 *  Bibliotecas arduinojson 6.15.2
 *  Placa esp32 2.0
 *  
*/

#include "CTBot.h"

#define LDR 34
int leds[5] = {14, 27, 26, 25, 33};
CTBot myBot;

String ssid = "INFINITUM1822";
String pass = "Sp5Ea8Kj1g";
String token = "8101015803:AAErLXVaaOiHQ8GFwfeAPhvfY8pfWeHk8A8";

bool alarmaActivada = false;
int umbral = 3000;

unsigned long ultimaAlerta = 0;
bool estadoAlerta = false;
const unsigned long intervaloAlerta = 500;

void setup(){
  Serial.begin(115200);
  delay(1000);
  Serial.print("Comenzando HE-Bot ...  ");
  
  pinMode(LDR, INPUT);  

  for(int i = 0; i< 5; i++){
    pinMode(leds[i],OUTPUT);
    digitalWrite(leds[i], LOW);
  }

  myBot.wifiConnect(ssid, pass);
  
  Serial.print("Conectando al Wi-Fi...");
  while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  Serial.print(".");
  }
  Serial.println("\nConectado a la red Wi-Fi.");
  Serial.println(WiFi.localIP());  // Muestra la IP local

  myBot.setTelegramToken(token);

  delay(2000);
  
  if (myBot.testConnection()){
    Serial.println("Conectado con Bot");
  }else{
    Serial.println("Error en la conexión");
  }
}

void loop(){
  TBMessage msg;

  int valorLDR = analogRead(LDR);
  float voltajeLDR = (valorLDR * 3.3)/4095;
  Serial.print("LDR: ");
  Serial.println(valorLDR);

  if(alarmaActivada && valorLDR > umbral){
    encenderSirena();
  }else{
    apagarSirena();
  }

  if(myBot.getNewMessage(msg)){
    String comando = msg.text;
    comando.toLowerCase();
    //Comando /start
    if(comando == "/start"){
      Serial.print("Nueva interacción de: ");
      Serial.println(msg.sender.username);
      String menu = "Hola, soy *HE-Bot*.\n\n" \
                       "Comandos disponibles:\n" \
                       "/dato - Monitorear valor del LDR\n" \
                       "/activar - Activar la alarma\n" \
                       "/desactivar - Desactivar la alarma\n" \
                       "/umbral N - Cambiar umbral (ej. /umbral 2500)\n" \
                       "/estado - Ver estado actual de la alarma";
       myBot.sendMessage(msg.sender.id,menu);
    }
    //Comando /dato
    else if(comando == "/dato"){
      myBot.sendMessage(msg.sender.id, "Valor de LDR: " + String(voltajeLDR));
    }
    //Comando /activar
    else if(comando == "/activar"){
      alarmaActivada = true;
      myBot.sendMessage(msg.sender.id, "Alarma activada.");
    }
    //Comando /desactivar
    else if(comando == "/desactivar"){
      alarmaActivada = false;
      myBot.sendMessage(msg.sender.id, "Alarma desactivada.");
    }
    //Comando /umbral
    else if(comando.startsWith("/umbral ")){
      String valorStr = comando.substring(8);
      int nuevoUmbral = valorStr.toInt();
      if (nuevoUmbral > 0 && nuevoUmbral < 4096) {
        umbral = nuevoUmbral;
        myBot.sendMessage(msg.sender.id, "Umbral actualizado a: " + String(umbral));
      } else {
        myBot.sendMessage(msg.sender.id, "Valor de umbral inválido. Debe estar entre 0 y 4095.");
      }
    }
    //Comando /estado
    else if(comando == "/estado"){
      String estado = alarmaActivada ? "activada" : "desactivada";
       myBot.sendMessage(msg.sender.id, "Estado actual:\n- Alarma: " + estado + "\n- Umbral: " + String(umbral) + "\n- LDR: " + String(valorLDR));
    }
    // Cualquier otra cosa
    else {
      myBot.sendMessage(msg.sender.id, "Mensaje no válido, intenta de nuevo con: /start");
    }
  }

  delay(50);
  
}

void encenderSirena(){
  unsigned long tiempoActual = millis();
  if(tiempoActual - ultimaAlerta >= intervaloAlerta){
    ultimaAlerta = tiempoActual;
    estadoAlerta = !estadoAlerta;
    for(int i = 0; i < 5; i++){
      digitalWrite(leds[i],estadoAlerta ? HIGH : LOW);
    }
  }
}

void apagarSirena(){
  for (int i = 0; i < 5; i++) {
    digitalWrite(leds[i], LOW);
  }
  estadoAlerta = false;
}
