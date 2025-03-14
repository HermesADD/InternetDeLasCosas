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

#define PinPWM 26
#define PinDAC 25
#define PinADCPWM 13
#define PinADCDAC 12

//Variables para definir el PWM
const int freq = 5000; //Frecuencia en Hz
const int ChanelPWM = 0; //Canal 0 - 15
const int resolution = 8; //Bits de resolución, hasta 8
const float dacEscala = 4.5;

void setup() {
  Serial.begin(115200);
  pinMode(PinPWM, OUTPUT);
  pinMode(PinDAC, OUTPUT);
  pinMode(PinADCPWM, INPUT);
  pinMode(PinADCDAC, INPUT);
  
  ledcAttachChannel(PinPWM, freq, resolution, ChanelPWM); 

}

void loop() {
  for(int i = 0; i<360; i++){
    float seno = (sin(i * M_PI / 180)+1) * 127.5;
    
    //round se usa para redondear un float de manera más precisa.
    int valor = round(seno);
    int valorDAC = round(seno * dacEscala);
    
    //constrain se usa para limitar un valor dentro de un rango específico.
    valorDAC = constrain(valorDAC,0,255);
    
    dacWrite(PinDAC, valorDAC);
    ledcWrite(PinPWM, valor);

    int adcDAC = analogRead(PinADCDAC);
    int adcPWM = analogRead(PinADCPWM);
    
    Serial.print("PWM:");
    Serial.print(adcPWM);
    Serial.print(", DAC:");
    Serial.println(adcDAC);

    delay(10);
    }
} 
