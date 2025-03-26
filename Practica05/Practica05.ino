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
const int freq = 5000; //Frecuencia en Hz

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
    float seno = sin(i * M_PI / 180);
    float seno2 = (127*sin(i * M_PI / 180)+127);

    // Escalar y centrar la señal para PWM (0-255)
    int valorPWM = seno*125.4;

    // Escalar y centrar la señal para DAC (0-255)
    int valorDAC = seno2;
    
    ledcWrite(PinPWM, valorPWM);
    dacWrite(PinDAC, valorDAC);

    // Lectura de ADC y conversión a voltaje
    float adcDAC = analogRead(PinADCDAC);
    float adcPWM = analogRead(PinADCPWM);

    float voltagedac = analogRead(PinADCDAC);
    float voltagepwm = 3.3/4096 * analogRead(PinADCPWM);
    
    Serial.print("DAC: ");
    Serial.print(voltagedac);
    Serial.print(", PWM: ");
    Serial.println(voltagepwm);
    
    delayMicroseconds(5000);
  }
}
