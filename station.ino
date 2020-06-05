bool setupStation(String ssid, String password) {
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
      debugln((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*");

      networks[i] = WiFi.SSID(i);
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
