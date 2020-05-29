void setupWebSocketServer() {
  webSocketServer.begin();
  webSocketServer.onEvent(webSocketServerEvent);
}

void webSocketServerEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  switch(type) {
    case WStype_DISCONNECTED:
      Serial.println("[WSserver] Disconnected!");
      break;
    case WStype_CONNECTED: {
      IPAddress ip = webSocketServer.remoteIP(num);
      Serial.print("[WSserver] Connected from: ");
      Serial.println(ip);
  
      // send message to client
      if (doc["uid"] || isStationSet) {
        webSocketServer.sendTXT(num, "{\"command\":\"CONNECT\",\"data\":{\"ssid\":\"" + currentStationSsid + "\",\"status\":\"connected\"}}");
      } else {
        webSocketServer.sendTXT(num, "{\"command\": \"INIT\", \"data\":\"" + uid +  "\"}");
      }
    }
      break;
    case WStype_TEXT:
      payloadStr = (char*)payload;
      handleServerMessage(payloadStr);
      break;
    case WStype_BIN:
      break;
  }
}

void handleServerMessage(String payloadStr) {
  DeserializationError error = deserializeJson(doc, payloadStr);
  
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    return;
  }

  const String command = doc["command"];

  if (command.equals("CONNECT")) {
    handleCmdConnect(doc["data"]["ssidMain"], doc["data"]["passwordMain"], doc["data"]["ssidFallback"], doc["data"]["passwordFallback"]);
  } else if (command.equals("RESET")) {
    handleCmdReset();
  }
}

void handleCmdConnect(String ssidMain, String passwordMain, String ssidFallback, String passwordFallback) {
  String stationData = "{";
  stationData += "\"uid\":\"" + uid + "\",";
  stationData += "\"ssidMain\":\"" + ssidMain + "\",";
  stationData += "\"passwordMain\":\"" + passwordMain + "\",";
  stationData += "\"ssidFallback\":\"" + ssidFallback + "\",";
  stationData += "\"passwordFallback\":\"" + passwordFallback + "\"";
  stationData += "}";
  setEeprom(stationData);

  isStationSet = setupStation(ssidMain, passwordMain);
  if (!isStationSet) {
    isStationSet = setupStation(ssidFallback, passwordFallback);
    if (!isStationSet) {
      resetStation();
    }
  }
}

void handleCmdReset() {
  resetStation();
}
