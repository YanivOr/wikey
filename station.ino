void setupStation(String ssid, String password, bool saveData) {
  WiFi.mode(WIFI_AP_STA);
  WiFi.begin(ssid, password);

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (saveData) {
    String stationData = "{\"ssid\":\"" + ssid + "\",\"password\":\"" + password + "\"}";
    setEeprom(stationData);
  }

  // Connect to server
  Serial.print("Connecting to remote server via websocket...");
  wsClient.connect(remoteServer);
  wsClient.send("{\"id\":\"123456\", \"type\": \"device\", \"command\": \"INIT\"}");
  Serial.println("connected");
}

void disconnectStation() {
  WiFi.disconnect();
}
