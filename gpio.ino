void onOffGpio(int pin, int val) {
  pinMode(pin, OUTPUT);
  digitalWrite(pin, val);
}

void setPulse(int pin, int startAs, long interval, long duration) {
  pulseEnabled = true;
  plusePin = pin;
  pulseState = startAs;
  pulseInterval = interval;
  pulseDuration = duration;
}

void unsetPulse() {
  pulseEnabled = false;
  plusePin = NULL;
  pulseState = NULL;
  pulseInterval = NULL;
  pulseDuration = NULL;
}

void loopPulseGpio() {
  unsigned long currentMillis = millis();

  if (currentMillis - pulseDurationCounter >= pulseDuration) {
    pulseDurationCounter = currentMillis;
    pulseEnabled = false;
  }

  if (!pulseEnabled) {
    return;
  }
  
  pinMode(plusePin, OUTPUT);

  if (currentMillis - pulseIntervalCounter >= pulseInterval) {
    pulseIntervalCounter = currentMillis;

    if (pulseState == LOW) {
      pulseState = HIGH;
    } else {
      pulseState = LOW;
    }
    digitalWrite(plusePin, pulseState);
  }
}
