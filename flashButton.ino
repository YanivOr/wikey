void setupFlashButton() {
  flashButton.begin();
  flashButton.onPressedFor(2000, onLongPressed);
}

void loopFlashButton() {
  flashButton.read();
}

void onLongPressed() {
  setupAccessPoint();
  webServerEnabled = true;
  stationSetupEnabled = false;
}
