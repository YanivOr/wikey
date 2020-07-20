#include <WiFiClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
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
const int eepromSize = 4096;
const int maxNetwroksNum = 50;

String payloadStr;
String currentStationSsid;
bool isStationSet = false;
bool stationSetupEnabled = true;
bool webServerEnabled = true;
int StaConCntr = 0;
String uid;
String remoteServerAddress = "";
int remoteServerPort = 0;
unsigned long pulseFreqCounter = 0;
int pulseCounter = 0;
unsigned long pingIntervalCounter = 0;
int state = LOW;
bool pulseEnabled = false;
int plusePin = NULL;
int pulseState = NULL;
int pulseFreq = NULL;
int pulseAmount = NULL;

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
    remoteServerAddress = String((const char*)doc["serverAddress"]);
    remoteServerPort = String((const char*)doc["serverPort"]).toInt();
  }

  // Flash button
  setupFlashButton();
  
  // Access Point
  setupAccessPoint();

  // Station
  setupStation();

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

  // Flash button
  loopFlashButton();

  // GPIO
  loopPulseGpio();

  // PING
  loopPing();
}
