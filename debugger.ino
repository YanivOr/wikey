void setupDebugger() {
  Serial.begin(115200);
}

void debug(String msg) {
  if (debugger) {
    Serial.print(msg);
  }
}

void debugln(String msg) {
  if (debugger) {
    Serial.println(msg);
  }
}
