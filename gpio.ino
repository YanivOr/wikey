void onOffGpio(int pin, int val) {
  pinMode(pin, OUTPUT);
  digitalWrite(pin, val);
}

void setPulse(int pin, int startAs, long interval, int amount) {
  pulseEnabled = true;
  plusePin = pin;
  pulseState = startAs;
  pulseInterval = interval;
  pulseAmount = amount;
  pulseCounter = 0;
}

void unsetPulse() {
  pulseEnabled = false;
  plusePin = NULL;
  pulseState = NULL;
  pulseInterval = NULL;
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

  if (currentMillis - pulseIntervalCounter >= pulseInterval) {
    pulseIntervalCounter = currentMillis;
    pulseCounter++;

    if (pulseState == LOW) {
      pulseState = HIGH;
    } else {
      pulseState = LOW;
    }
    digitalWrite(plusePin, pulseState);
  }
}
