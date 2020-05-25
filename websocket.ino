void onMessageCallback(WebsocketsMessage message) {
  Serial.print("Got Message: ");
  Serial.println(message.data());
  
  DeserializationError error = deserializeJson(doc, message.data());

  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    return;
  }

  const String command = doc["command"];

  if (command.equals("GPIO")) {
    handleCmdGpio(doc["data"]["pin"], doc["data"]["val"]);
  } else if (command=="PULSE") {
    handleCmdPulse(doc["data"]["pin"], doc["data"]["val"]);
  } else if (command=="STR") {
    handleCmdStr(doc["data"]["val"]);
  }
}

void onEventsCallback(WebsocketsEvent event, String data) {
  if(event == WebsocketsEvent::ConnectionOpened) {
    Serial.println("Connnection Opened");
    // wsClient.send("{\"id\":\"" + String(uid) + "\", \"type\": \"device\", \"command\": \"INIT\"}");    
  } else if(event == WebsocketsEvent::ConnectionClosed) {
    Serial.println("Connnection Closed");
  } else if(event == WebsocketsEvent::GotPing) {
    Serial.println("Got a Ping!");
  }
}

void setupWebsocket() {
  wsClient.onMessage(onMessageCallback);
  wsClient.onEvent(onEventsCallback);
}

void loopWebsocket() {
    wsClient.poll();
}

void handleCmdGpio(int pin, int val) {
  onOffGpio(pin, val);
}

void handleCmdPulse(int pin, int val) {
  pulseGpio(pin, val);
}

void handleCmdStr(String val) {
  Serial.println(val);
}
