void handleRoot() {
  handleFileRead("/");
}

void handleAppJs() {
  handleFileRead("/app.js");
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
  debugln("handleFileRead: " + path);
  if (path.endsWith("/")) path += "index.html";
  String contentType = getContentType(path);
  if (SPIFFS.exists(path)) {
    File file = SPIFFS.open(path, "r");
    size_t sent = server.streamFile(file, contentType);
    file.close();
    return true;
  }
  debugln("\tFile Not Found");
  return false;
}

void setupWebServer() {
  server.on("/", handleRoot);
  server.on("/app.js", handleAppJs);
  server.on("/style.css", handleStyle);
  server.begin();
  SPIFFS.begin();
  debugln("HTTP server started");
}

void loopWebServer() {
  if (webServerEnabled) {
    server.handleClient();
  }
}
