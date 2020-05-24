void handleRoot() {
  if (isStationSet) {
    handleFileRead("/response.html");
    
  } else {
    handleFileRead("/");
  }
}

void handleStyle() {
  handleFileRead("/style.css");
}

void handleResponse() {
  if (server.method() == HTTP_POST) {
    String ssidMain, passwordMain, ssidFallback, passwordFallback;

    for (uint8_t i = 0; i < server.args(); i++) {
      if (server.argName(i) == "ssid-main") {
        ssidMain = server.arg(i);
      }
      if (server.argName(i) == "password-main") {
        passwordMain = server.arg(i);
      }
      if (server.argName(i) == "ssid-fallback") {
        ssidFallback = server.arg(i);
      }
      if (server.argName(i) == "password-fallback") {
        passwordFallback = server.arg(i);
      }
    }

    setupStation(ssidMain, passwordMain, true);

    handleFileRead("/response.html");
  }
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
  String eepromData = getEeprom();
  DeserializationError error = deserializeJson(doc, eepromData);

  const String ssid = doc["ssid"];
  const String password = doc["password"];
  
  if (!ssid.equals("null") && !password.equals("null")) {
    setupStation(ssid, password, false);
    isStationSet = true;
  }
  
  server.on("/", handleRoot);
  server.on("/style.css", handleStyle);
  server.on("/response.html", handleResponse);
  server.begin();
  SPIFFS.begin();
  Serial.println("HTTP server started");
}

void loopWebServer() {
  server.handleClient();
}
