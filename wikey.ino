#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoWebsockets.h>
#include <FS.h>

using namespace websockets;

const char *ssid = "wikey";
const char *password = "11111111";
const char *remoteServer = "ws://192.168.1.9:3000";

ESP8266WebServer server(80);
WebsocketsClient wsClient;

void setup() {
  // Debugger
  setupDebugger();
  
  // Access Point
  setupAccessPoint();

  // Web Server
  setupWebServer();

  // WebSocket
  setupWebsocket();
}

void loop() {
  // webServer
  loopWebServer();
  
  // WebSocket
  loopWebsocket();
}
