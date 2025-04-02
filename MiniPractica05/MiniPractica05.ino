/*
  Universidad Nacional Autónoma de México
  Facultad de Ciencias
  Licenciatura en Ciencias de la Computación
  Seminario de Ciencias de la Computación A: Introducción al Internet de las Cosas

  Escrito por:
    Delgado Díaz Hermes Alberto
    López Montúfar José Eleazar
*/
// LED 3
#define PinPWM 26
// LED 4
#define PinDAC 25
#define PinADCPWM 13
#define PinADCDAC 12

//Variables para definir el PWM
const int freq = 10000; //Frecuencia en Hz


void setup() {
  Serial.begin(115200);

  // Configuración de pines
  pinMode(PinPWM, OUTPUT);
  pinMode(PinDAC, OUTPUT);
  pinMode(PinADCPWM, INPUT);
  pinMode(PinADCDAC, INPUT);
  const int ChanelPWM = 0; //Canal 0 - 15
  const int resolution = 8; //Bits de resolución, hasta 8

  ledcAttachChannel(PinPWM, freq, resolution, ChanelPWM);
}

void loop() {
  for (int i = 0; i < 360; i++) {
    // Usar la misma función para ambos LEDs
    float seno2 = (127*sin(i * M_PI / 180)+127);
    float umbral = 2.1 /3.3*255;
    float senodac = seno2*((255 - umbral)/ 255)+umbral;

    
    ledcWrite(PinPWM, seno2);
    dacWrite(PinDAC, senodac);

    
    Serial.print("DAC: ");
    Serial.print(analogRead(PinADCDAC));
    Serial.print(", PWM: ");
    Serial.println(analogRead(PinADCPWM));
    
    delayMicroseconds(10000);
  }
}
