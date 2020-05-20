#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <FS.h>

#ifndef APSSID
#define APSSID "wikey"
#define APPSK  "11111111"
#endif

const char *ssid = APSSID;
const char *password = APPSK;

ESP8266WebServer server(80);

void setup() {
  // Debugger
  setupDebugger();
  
  // Access Point
  setupAccessPoint();

  // Web Server
  setupWebServer();

  // GPIO
  setupGpio();
}

void loop() {
  // webServer
  loopWebServer();
}
