void handleRoot() {
  handleFileRead("/");

  if (server.method() == HTTP_POST) {
    for (uint8_t i = 0; i < server.args(); i++) {
      if (server.argName(i) == "ssid-main") {
        String ssidMain = server.arg(i);
        Serial.println("ssidMain = " + ssidMain);
      }
      if (server.argName(i) == "password-main") {
        String passwordMain = server.arg(i);
        Serial.println("passwordMain = " + passwordMain);
      }
      if (server.argName(i) == "ssid-fallback") {
        String ssidFallback = server.arg(i);
        Serial.println("ssidFallback = " + ssidFallback);
      }
      if (server.argName(i) == "password-fallback") {
        String passwordFallback = server.arg(i);
        Serial.println("passwordFallback = " + passwordFallback);
      }
    }
  }

/*
  setupStation(String ssid, String password)
*/
}

void handleStyle() {
  handleFileRead("/style.css");
}

String getContentType(String filename) {
  if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js")) return "application/javascript";
  else if (filename.endsWith(".ico")) return "image/x-icon";
  return "text/plain";
}

bool handleFileRead(String path) {
  Serial.println("handleFileRead: " + path);
  if (path.endsWith("/")) path += "index.html";
  String contentType = getContentType(path);
  if (SPIFFS.exists(path)) {
    File file = SPIFFS.open(path, "r");
    size_t sent = server.streamFile(file, contentType);
    file.close();
    return true;
  }
  Serial.println("\tFile Not Found");
  return false;
}

void setupWebServer() {
  server.on("/", handleRoot);
  server.on("/style.css", handleStyle);
  server.begin();
  SPIFFS.begin();
  Serial.println("HTTP server started");
}

void loopWebServer() {
  server.handleClient();
}
