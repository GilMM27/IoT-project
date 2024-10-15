#define PIN_TRIG D5
#define PIN_ECHO D6

void setup() {
  Serial.begin(9600);
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
}

float ultrasonicRead() {
  digitalWrite(PIN_TRIG, LOW);  //para generar un pulso limpio ponemos a LOW 4us
  delayMicroseconds(4);
  
  digitalWrite(PIN_TRIG, HIGH);  //generamos Trigger (disparo) de 10us
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);
  
  float tiempo = pulseIn(PIN_ECHO, HIGH);
  return tiempo/58.3;
}

void loop() {
  float distance = ultrasonicRead();
  if (distance < 25) {
    Serial.println("Movement detected!");
  }
  delay(500);
}