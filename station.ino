bool setupStation(String ssid, String password) {
  WiFi.mode(WIFI_AP_STA);
  WiFi.begin(ssid, password);
  currentStationSsid = ssid;

  // Wait for connection
  StaConCntr = 0;
  while (WiFi.status() != WL_CONNECTED && StaConCntr < 20) {
    delay(500);
    Serial.print(".");
    webSocketServer.broadcastTXT("{\"command\":\"CONNECT\",\"data\":{\"ssid\":\"" + ssid + "\",\"status\":\"connecting\"}}");
    StaConCntr++;
  }

  if (!WiFi.localIP()) {
    Serial.println("");
    Serial.print("Connection to ");
    Serial.print(ssid);
    Serial.println(" has failed");
    return false;
  }
  
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  webSocketServer.broadcastTXT("{\"command\":\"CONNECT\",\"data\":{\"ssid\":\"" + ssid + "\",\"status\":\"connected\"}}");
  connectToServer();
  return true;
}

void connectToServer() {
  Serial.print("Connecting to remote server via websocket...");
  setupWebSocketClient();
  Serial.println("connected");
}

void disconnectStation() {
  WiFi.disconnect();
}

void resetStation() {
  Serial.println("Resetting station...");
  clearEeprom();
  ESP.restart();
}
