void setupUid() {
  for (int i=0;i<10;i++) {
    uid += String(getSingle());
  }
}

int getSingle() {
  return random(1, 10);
}
