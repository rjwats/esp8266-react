#ifndef SerialMonitor_h
#define SerialMonitor_h

#include <Arduino.h>
#include <time.h>
#include <ESPAsyncWebServer.h>
#include <SecurityManager.h>
#include <NTPStatus.h>
#include <SerialServer.h>

#define WEB_SOCKET_SERIAL_PATH "/ws/serial"
#define WEB_SOCKET_SERIAL_BUFFER 512

class WebSocketSerialHandler {
 public:
  WebSocketSerialHandler(AsyncWebServer* server, SecurityManager* securityManager) : _webSocket(WEB_SOCKET_SERIAL_PATH) {
    _webSocket.setFilter(securityManager->filterRequest(AuthenticationPredicates::IS_ADMIN));
    server->addHandler(&_webSocket);
    server->on(WEB_SOCKET_SERIAL_PATH, HTTP_GET, std::bind(&WebSocketSerialHandler::forbidden, this, std::placeholders::_1));
  }

  Stream* stream;
  void loop() {
    size_t len;
    if ((stream) && (len = this->stream->available() > 0)) {
      char buf[128];
      this->stream->readBytes(buf, std::min(len, size_t{128}));
      if (serialEvent(String(buf))){
        return;
      } else {
        Serial.println("Something went wrong");
      };
    }
    unsigned long currentMillis = millis();
 
    if(currentMillis - previousMillis > 1000) {
    // save the last time you blinked the LED 
      previousMillis = currentMillis; 
      serialEvent("Hello world");
    }
  };

 private:
  AsyncWebSocket _webSocket;
  long previousMillis;  

  void forbidden(AsyncWebServerRequest* request) {
    request->send(403);
  }

  boolean serialEvent(String message) {
    // if there are no clients, don't bother doing anything
    if (!_webSocket.getClients().length()) {
      return true;
    }
    if (!_webSocket.availableForWriteAll()) {
      return false;
    }

    // create JsonObject to hold serial event
    DynamicJsonDocument jsonDocument = DynamicJsonDocument(WEB_SOCKET_SERIAL_BUFFER);
    JsonObject jsonObject = jsonDocument.to<JsonObject>();
    auto tempTime = time(nullptr);
    jsonObject["time"] = toISOString(localtime(&tempTime), true);
    jsonObject["message"] = message;

    // transmit serial event to all clients
    size_t len = measureJson(jsonDocument);
    AsyncWebSocketMessageBuffer* buffer = _webSocket.makeBuffer(len);
    if (buffer) {
      serializeJson(jsonDocument, (char*)buffer->get(), len + 1);
      _webSocket.textAll(buffer);
    }
    return true;
  }
};

#endif