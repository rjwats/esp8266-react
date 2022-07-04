#ifndef WebSocketLogHandler_h
#define WebSocketLogHandler_h

#include <ESPAsyncWebServer.h>
#include <SecurityManager.h>
//#include <Logger.h>
#include <ESPUtils.h>
#include <time.h>

#define WEB_SOCKET_LOG_PATH "/ws/log"
#define WEB_SOCKET_LOG_BUFFER 512

enum LogLevel { DEBUG = 0, INFO = 1, WARNING = 2, ERROR = 3 };

class LogEvent {
 public:
  uint32_t id;
  time_t time;
  LogLevel level;
  String file;
  uint16_t line;
  String message;

  static void serialize(LogEvent& logEvent, JsonObject& root) {
    root["time"] = logEvent.time;
    root["level"] = (uint8_t)logEvent.level;
    root["file"] = logEvent.file;
    root["line"] = logEvent.line;
    root["message"] = logEvent.message;
  }

  static void deserialize(JsonObject& root, LogEvent& logEvent) {
    logEvent.time = root["time"];
    logEvent.level = (LogLevel)root["level"].as<uint8_t>();
    logEvent.file = root["file"] | "";
    logEvent.line = root["line"];
    logEvent.message = root["message"] | "";
  }
};
class WebSocketLogHandler {
 public:
  WebSocketLogHandler(AsyncWebServer* server, SecurityManager* securityManager) : _webSocket(WEB_SOCKET_LOG_PATH) {
    _webSocket.setFilter(securityManager->filterRequest(AuthenticationPredicates::IS_ADMIN));
    server->addHandler(&_webSocket);
    server->on(WEB_SOCKET_LOG_PATH, HTTP_GET, std::bind(&WebSocketLogHandler::forbidden, this, std::placeholders::_1));
  }

  void begin() {
    //Logger::getInstance()->addEventHandler(std::bind(&WebSocketLogHandler::logEvent, this, std::placeholders::_1));
  }

  void loop() {
    unsigned long currentMillis = millis();
 
    if(currentMillis - previousMillis > 1000) {
    // save the last time you blinked the LED 
      previousMillis = currentMillis; 
      helloWorld();
    }
  }
 private:
  AsyncWebSocket _webSocket;
  long previousMillis;  

  void forbidden(AsyncWebServerRequest* request) {
    request->send(403);
  }

  void helloWorld() {
    LogEvent helloWorldEvent;
    helloWorldEvent.message = "Hello world";
    logEvent(helloWorldEvent);
  }

  boolean logEvent(LogEvent& logEvent) {
    // if there are no clients, don't bother doing anything
    if (!_webSocket.getClients().length()) {
      return true;
    }
    if (!_webSocket.availableForWriteAll()) {
      return false;
    }

    // create JsonObject to hold log event
    DynamicJsonDocument jsonDocument = DynamicJsonDocument(WEB_SOCKET_LOG_BUFFER);
    JsonObject jsonObject = jsonDocument.to<JsonObject>();
    jsonObject["time"] = ESPUtils::toISOString(localtime(&logEvent.time), true);
    jsonObject["level"] = logEvent.level;
    jsonObject["file"] = logEvent.file;
    jsonObject["line"] = logEvent.line;
    jsonObject["message"] = logEvent.message;

    // transmit log event to all clients
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
