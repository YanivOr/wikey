void onMessageCallback(WebsocketsMessage message) {
    Serial.print("Got Message: ");
    Serial.println(message.data());
    pulseGpio(2);
}

void onEventsCallback(WebsocketsEvent event, String data) {
  if(event == WebsocketsEvent::ConnectionOpened) {
    Serial.println("Connnection Opened");
    wsClient.send("{\"id\":\"123456\", \"type\": \"device\", \"command\": \"init\"}");    
  } else if(event == WebsocketsEvent::ConnectionClosed) {
    Serial.println("Connnection Closed");
  } else if(event == WebsocketsEvent::GotPing) {
    Serial.println("Got a Ping!");
  }
}

void setupWebsocket() {
  // Setup Callbacks
  wsClient.onMessage(onMessageCallback);
  wsClient.onEvent(onEventsCallback);
}

void loopWebsocket() {
    wsClient.poll();
}
