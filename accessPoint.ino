void setupAccessPoint() {
  debugln("");
  debugln("Configuring access point...");
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  debug("AP IP address: ");
  debugln(myIP.toString());
}
