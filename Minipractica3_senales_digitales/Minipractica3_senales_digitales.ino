// Defino los botones 
#define PinBotton_PullUp 4
#define PinBotton_PullDown 15

// Defino los LEDS
#define PinLed_1 14
#define PinLed_2 27
#define PinLed_3 26
#define PinLed_4 25
#define PinLed_5 33

//Defino contador para las pulsaciones
int count = 0;
// Defino un contador de tiempo de la ultima pulsacion
long timeCounter = 0;
// Defino tiempo de espera entre pulsaciones
const int timeThreshold = 1000;

void setup() {
  // Inicio el monitor serie
  Serial.begin(115200);
  // Configuro los dos botones como input
  pinMode(PinBotton_PullUp, INPUT);
  pinMode(PinBotton_PullDown, INPUT);

  //  Configuro los LEDS como salida
  pinMode(PinLed_1, OUTPUT);
  pinMode(PinLed_2, OUTPUT);
  pinMode(PinLed_3, OUTPUT);
  pinMode(PinLed_4, OUTPUT);
  pinMode(PinLed_5, OUTPUT);
}

void loop() {
  
  int in_up = digitalRead(PinBotton_PullUp);
  int in_down = digitalRead(PinBotton_PullDown);
  
  if (in_up == LOW) {
    if (millis() > timeCounter + timeThreshold) {
      count++;
      Serial.println(count);
      timeCounter = millis();

      if(count ==1){
        digitalWrite(PinLed_1,HIGH);
        }
      if(count ==2){
        digitalWrite(PinLed_2,HIGH);
        }
      if(count ==3){
        digitalWrite(PinLed_3,HIGH);
        }
      if(count ==4){
        digitalWrite(PinLed_4,HIGH);
        }
      if(count ==5){
        digitalWrite(PinLed_5,HIGH);
        }
      
    }
  }

  /* Si el boton pulldown es presionado resetea el contador 
   *  Y apaga todos los LEDS
   */
  if(in_down == HIGH | count>5){
    count = 0;
      digitalWrite(PinLed_1,LOW);
      digitalWrite(PinLed_2,LOW);
      digitalWrite(PinLed_3,LOW);
      digitalWrite(PinLed_4,LOW);
      digitalWrite(PinLed_5,LOW);
    }
  
}
