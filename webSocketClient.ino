void setupWebSocketClient() {
  webSocketClient.begin(remoteServerIP, remoteServerPort, "/");
  webSocketClient.onEvent(webSocketClientEvent);
  webSocketClient.setReconnectInterval(5000);
  
  // start heartbeat
  // ping server every 15000 ms
  // expect pong from server within 3000 ms
  // consider connection disconnected if pong is not received 2 times
  webSocketClient.enableHeartbeat(15000, 3000, 2);
}

void webSocketClientEvent(WStype_t type, uint8_t * payload, size_t length) {

  switch(type) {
    case WStype_DISCONNECTED:
      debugln("[WSclient] Disconnected!\n");
      ESP.restart();
      break;
    case WStype_CONNECTED: {
      payloadStr = (char*)payload;
      
      debug("[WSclient] Connected to url:");
      debugln(payloadStr);

      // send message to server when Connected
      webSocketClient.sendTXT("{\"id\":\"" + uid +  "\", \"type\": \"device\", \"command\": \"INIT\"}");
    }
      break;
    case WStype_TEXT:
      payloadStr = (char*)payload;
      
      debugln("[WSclient] got text");
      debugln(payloadStr);

      handleClientMessage(payloadStr);
      break;
    case WStype_BIN:
      debugln("[WSclient] got binary length:");
      debugln(String(length));
      //hexdump(payload, length);

      // send data to server
      // webSocketClient.sendBIN(payload, length);
      break;
    case WStype_PING:
      // pong will be send automatically
      debugln("[WSclient] got ping\n");
      break;
    case WStype_PONG:
      // answer to a ping we send
      debugln("[WSclient] got pong\n");
      break;
   }
}

void handleClientMessage(String payloadStr) {
  DeserializationError error = deserializeJson(doc, payloadStr);
  
  if (error) {
    debug(F("deserializeJson() failed: "));
    debugln(error.c_str());
    return;
  }

  const String command = doc["command"];
  
  if (command.equals("GPIO")) {
    handleCmdGpio(doc["data"]["pin"], doc["data"]["val"]);
  } else if (command=="PULSE") {
    handleCmdPulse(doc["data"]["pin"], doc["data"]["startAs"], doc["data"]["freq"], doc["data"]["amount"]);
  } else if (command=="STR") {
    handleCmdStr(doc["data"]["val"]);
  }
}

void handleCmdGpio(int pin, int val) {
  onOffGpio(pin, val);
}

void handleCmdPulse(int pin, int startAs, int freq, int amount) {
  setPulse(pin, startAs, freq, amount);
}

void handleCmdStr(String val) {
  debugln(val);
}
