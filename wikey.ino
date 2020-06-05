#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include <WebSocketsServer.h>
#include <WebSocketsClient.h>
#include <FS.h>
#include <ArduinoJson.h>
#include <EasyButton.h>

#define FLASH_BTN_PIN 0

const bool debugger = true;
const String ssid = "wikey";
const String password = "11111111";
const String remoteServerIP = "192.168.43.80";
const int remoteServerPort = 3000;
const int eepromSize = 4096;
const int maxNetwroksNum = 50;

String payloadStr;
String currentStationSsid;
bool isStationSet = false;
int StaConCntr = 0;
String uid;

ESP8266WebServer server(80);
WebSocketsServer webSocketServer = WebSocketsServer(81);
WebSocketsClient webSocketClient;
StaticJsonDocument<4096> doc;
EasyButton flashButton(FLASH_BTN_PIN, 40);

void setup() {
  WiFi.mode(WIFI_AP_STA);
  
  // debugger
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

  // Flash button
  setupFlashButton();
}

void loop() {
  // webServer
  loopWebServer();
  
  // WebSocket
  webSocketServer.loop();
  webSocketClient.loop();

  // Flash button
  loopFlashButton();
}
