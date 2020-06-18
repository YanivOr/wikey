void onOffGpio(int pin, int val) {
  pinMode(pin, OUTPUT);
  digitalWrite(pin, val);
}

void setPulse(int pin, int startAs, int freq, int amount) {
  pulseEnabled = true;
  plusePin = pin;
  pulseState = startAs;
  pulseFreq = freq > 500 ? 500 : freq;
  pulseAmount = amount;
  pulseCounter = 0;
}

void loopPulseGpio() {
  if (!pulseEnabled) {
    return;
  }

  if (pulseCounter > pulseAmount * 2) {
    pulseEnabled = false;
  }
  
  unsigned long currentMillis = millis();
  pinMode(plusePin, OUTPUT);

  if (currentMillis - pulseFreqCounter >= 500 / pulseFreq) {
    pulseFreqCounter = currentMillis;
    pulseCounter++;

    if (pulseState == LOW) {
      pulseState = HIGH;
    } else {
      pulseState = LOW;
    }
    digitalWrite(plusePin, pulseState);
  }
}
