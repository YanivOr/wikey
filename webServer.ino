void handleRoot() {
  if (server.method() == HTTP_POST) {
    String resetStationRequest;

    for (uint8_t i = 0; i < server.args(); i++) {
      if (server.argName(i) == "reset") {
        resetStationRequest = server.arg(i);
      }
    }

    if (resetStationRequest.equals("1")) {
      resetStation();
    }
  } else if (isStationSet) {
    handleFileRead("/response.html");
    
  } else {
    handleFileRead("/");
  }
}

void handleDataJs() {
  String content = "const uid = " + uid + "";
  server.send(200, "application/javascript", content);
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

    handleFileRead("/response.html");

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
    }
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
  const String ssidMain = doc["ssidMain"];
  const String passwordMain = doc["passwordMain"];
  const String ssidFallback = doc["ssidFallback"];
  const String passwordFallback = doc["passwordFallback"];
  
  if (!ssidMain.equals("null") && !ssidMain.equals("") && !passwordMain.equals("null") && !passwordMain.equals("")) {
    isStationSet = setupStation(ssidMain, passwordMain);
    if (!isStationSet && !ssidFallback.equals("null") && !ssidFallback.equals("") && !passwordFallback.equals("null") && !passwordFallback.equals("")) {
      isStationSet = setupStation(ssidFallback, passwordFallback);
    }
  }
  server.on("/", handleRoot);
  server.on("/data.js", handleDataJs);
  server.on("/style.css", handleStyle);
  server.on("/response.html", handleResponse);
  server.begin();
  SPIFFS.begin();
  Serial.println("HTTP server started");
}

void loopWebServer() {
  server.handleClient();
}
