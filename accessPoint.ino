void handleRoot() {
  server.send(200, "text/html", "<h1>You are connected</h1><br /><input type='text' placeholder='ssid' /><input type='password' /><input type='submit' />");
}

void setupAccessPoint() {
  delay(1000);
  Serial.begin(115200);
  Serial.println();
  Serial.print("Configuring access point...");
  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.on("/", handleRoot);
  server.begin();
  Serial.println("HTTP server started");
}


void loopAccessPoint() {
  server.handleClient();
}
