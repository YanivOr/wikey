void setupWebSocketServer() {
  webSocketServer.begin();
  webSocketServer.onEvent(webSocketServerEvent);
}

void webSocketServerEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  switch(type) {
    case WStype_DISCONNECTED:
      debugln("[WSserver] Disconnected!");
      break;
    case WStype_CONNECTED: {
      IPAddress ip = webSocketServer.remoteIP(num);
      debug("[WSserver] Connected from: ");
      debugln(ip.toString());
  
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
      handleServerMessage(payloadStr, num);
      break;
    case WStype_BIN:
      break;
  }
}

void handleServerMessage(String payloadStr, uint8_t num) {
  DeserializationError error = deserializeJson(doc, payloadStr);
  
  if (error) {
    debug(F("deserializeJson() failed: "));
    debugln(error.c_str());
    return;
  }

  const String command = doc["command"];

  if (command.equals("SCAN")) {
    handleCmdScan(num);
  }
  else if (command.equals("CONNECT")) {
    handleCmdConnect(doc["data"]["ssidMain"], doc["data"]["passwordMain"], doc["data"]["ssidFallback"], doc["data"]["passwordFallback"]);
  } else if (command.equals("RESET")) {
    handleCmdReset();
  }
}

void handleCmdScan(uint8_t num) {
  String networks [maxNetwroksNum];
  String networksList = "";
  
  scanWifiNetworks(networks);

  for (int i = 0; i < maxNetwroksNum; ++i) {
    if (!networks[i].equals("")) {
      networksList += "\"" + networks[i] + "\",";
    }
  }

  networksList.remove(networksList.length()-1);
  webSocketServer.sendTXT(num, "{\"command\": \"SCAN\", \"data\": [" + networksList +  "]}");
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
