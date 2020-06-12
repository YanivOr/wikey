void setupAccessPoint() {
  debugln("");
  debugln("Configuring access point...");
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  debug("AP IP address: ");
  debugln(myIP.toString());
  if (!MDNS.begin("esp8266")) {
    debugln("Error setting up MDNS responder!");
  }
  debugln("mDNS responder started");
}
