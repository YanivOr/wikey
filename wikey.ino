#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include <WebSocketsServer.h>
#include <WebSocketsClient.h>
#include <FS.h>
#include <ArduinoJson.h>

const String ssid = "wikey";
const String password = "11111111";
const String remoteServerIP = "192.168.1.9";
const int remoteServerPort = 3000;
const int eepromSize = 4096;

String payloadStr;
String currentStationSsid;
bool isStationSet = false;
int StaConCntr = 0;
String uid;

ESP8266WebServer server(80);
WebSocketsServer webSocketServer = WebSocketsServer(81);
WebSocketsClient webSocketClient;
StaticJsonDocument<4096> doc;

void setup() {   
  // Debugger
  setupDebugger();

  // Eeprom
  setupEeprom();

  // Unique ID
  if (!doc["uid"]) {
    setupUid();
  } else {
    uid = String((const char*)doc["uid"]);
  }
  
  // Access Point
  setupAccessPoint();

  // Web Server
  setupWebServer();

  // WebSocket
  setupWebSocketServer();
}

void loop() {
  // webServer
  loopWebServer();
  
  // WebSocket
  webSocketServer.loop();
  webSocketClient.loop();
}
