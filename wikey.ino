#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include <ArduinoWebsockets.h>
#include <FS.h>
#include <ArduinoJson.h>

using namespace websockets;

const char *ssid = "wikey";
const char *password = "11111111";
const char *remoteServer = "ws://192.168.1.9:3000";
const int eepromSize = 4096;

bool isStationSet = false;
int StaConCntr = 0;
String uid;

ESP8266WebServer server(80);
// WebsocketsServer wsServer;
// WebsocketsClient remoteWsClient;
WebsocketsClient wsClient;
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
  setupWebsocket();
}

void loop() {
  // webServer
  loopWebServer();
  
  // WebSocket
  loopWebsocket();
}
