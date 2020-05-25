void setupEeprom() {
  EEPROM.begin(eepromSize);
  getJson();
}

void setEeprom(const String &val) {
  for(int i=0;i<eepromSize;i++) {
    EEPROM.write(0x0F+i, val[i]);
  }
  EEPROM.commit();
}

String getEeprom() {
  String val;
  for(int i=0;i<eepromSize;i++) {
    char chr = EEPROM.read(0x0F+i);
        
    if (chr==0x0F) {
      break;
    }
    
    val += chr;
  }
  return String(val);
}

void clearEeprom() {
  for(int i=0;i<eepromSize;i++) {
    EEPROM.write(0x0F+i, 0x0F);
  }
  EEPROM.commit();
}

void getJson() {
  String eepromData = getEeprom();
  DeserializationError error = deserializeJson(doc, eepromData);
}
