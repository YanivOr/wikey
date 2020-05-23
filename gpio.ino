/*
 HIGH         1
 LOW          0
 LED_BUILTIN  2
*/

void onOffGpio(int pin, int val) {
  pinMode(pin, OUTPUT);
  digitalWrite(pin, val);
}

void pulseGpio(int pin) {
  pinMode(pin, OUTPUT);
  
  for (int i=0;i<5;i++) {
    digitalWrite(pin, LOW);
    delay(50);
    digitalWrite(pin, HIGH);
    delay(50);
  }
}
