void setupStation() {
  const String ssidMain = doc["ssidMain"];
  const String passwordMain = doc["passwordMain"];
  const String ssidFallback = doc["ssidFallback"];
  const String passwordFallback = doc["passwordFallback"];

  if (!ssidMain.equals("null") && !ssidMain.equals("") && !passwordMain.equals("null") && !passwordMain.equals("")) {
    while (!isStationSet) {
      isStationSet = setStation(ssidMain, passwordMain);
      if (!isStationSet && !ssidFallback.equals("null") && !ssidFallback.equals("") && !passwordFallback.equals("null") && !passwordFallback.equals("")) {
        isStationSet = setStation(ssidFallback, passwordFallback);
      }
    }
  }
}

bool setStation(String ssid, String password) {
  WiFi.begin(ssid, password);
  currentStationSsid = ssid;

  // Wait for connection
  StaConCntr = 0;
  while (WiFi.status() != WL_CONNECTED && StaConCntr < 20) {
    delay(500);
    debug(".");
    debug(".");
    webSocketServer.broadcastTXT("{\"command\":\"CONNECT\",\"data\":{\"ssid\":\"" + ssid + "\",\"status\":\"connecting\"}}");
    StaConCntr++;
  }

  if (!WiFi.localIP()) {
    debugln("");
    debug("Connection to ");
    debug(ssid);
    debugln(" has failed");
    return false;
  }
  
  debugln("");
  debug("Connected to ");
  debugln(ssid);
  debug("IP address: ");
  debugln(WiFi.localIP().toString());

  WiFi.softAPdisconnect(true);
  webServerEnabled = false;
  webSocketServer.broadcastTXT("{\"command\":\"CONNECT\",\"data\":{\"ssid\":\"" + ssid + "\",\"status\":\"connected\"}}");
  connectToServer();
  return true;
}

void scanWifiNetworks(String (& networks) [maxNetwroksNum]) {
  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();
  
  debugln("scan done");
  if (n == 0) {
    debugln("no networks found");
  } else {
    debug(String(n));
    debugln(" networks found");
    for (int i = 0; i < n; ++i) {
      // Print SSID and RSSI for each network found
      debug(String(i + 1));
      debug(": ");
      debug(WiFi.SSID(i));
      debug(" (");
      debug(String(WiFi.RSSI(i)));
      debug(")");
      debugln(String(WiFi.encryptionType(i)));

      networks[i] = "{\"ssid\": \"" + WiFi.SSID(i) + "\", \"rssi\":\"" + String(WiFi.RSSI(i)) + "\", \"encryption\": \"" + String(WiFi.encryptionType(i)) + "\"}";
    }
  }
  debugln("");
}

void connectToServer() {
  debug("Connecting to remote server via websocket...");
  setupWebSocketClient();
  debugln("connected");
}

void disconnectStation() {
  WiFi.disconnect();
}

void resetStation() {
  debugln("Resetting station...");
  clearEeprom();
  ESP.restart();
}
