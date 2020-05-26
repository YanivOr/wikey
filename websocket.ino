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
  } else if(event == WebsocketsEvent::ConnectionClosed) {
    Serial.println("Connnection Closed");
  } else if(event == WebsocketsEvent::GotPing) {
    Serial.println("Got a Ping!");
  }
}

void setupWebsocket() {
  // wsServer
  // wsServer.listen(81);

  // wsClient
  wsClient.onMessage(onMessageCallback);
  wsClient.onEvent(onEventsCallback);
}

void loopWebsocket() {
  // wsServer
  // auto remoteWsClient = wsServer.accept();

  /*
  if(remoteWsClient.available()) {
    auto msg = remoteWsClient.readBlocking();

    Serial.print("Got Message: ");
    Serial.println(msg.data());

    remoteWsClient.send("Echo: " + msg.data());
    remoteWsClient.close();
  }
  */
  
  // wsClient
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
