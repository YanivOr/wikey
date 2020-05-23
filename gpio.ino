void onOffGpio(int pin, int val) {
  pinMode(pin, OUTPUT);
  digitalWrite(pin, val);
}

void pulseGpio(int pin, int val) {
  pinMode(pin, OUTPUT);
  
  for (int i=0;i<5;i++) {
    digitalWrite(pin, val);
    delay(50);
    digitalWrite(pin, !val);
    delay(50);
  }
}
