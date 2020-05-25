#ifndef WebSocketLogHandler_h
#define WebSocketLogHandler_h

#include <ESPAsyncWebServer.h>
#include <SecurityManager.h>
#include <Logger.h>
#include <ESPUtils.h>

#define WEB_SOCKET_LOG_PATH "/ws/log"
#define WEB_SOCKET_LOG_BUFFER 512

class WebSocketLogHandler {
 public:
  WebSocketLogHandler(AsyncWebServer* server, SecurityManager* securityManager) : _webSocket(WEB_SOCKET_LOG_PATH) {
    _webSocket.setFilter(securityManager->filterRequest(AuthenticationPredicates::IS_ADMIN));
    server->addHandler(&_webSocket);
    server->on(WEB_SOCKET_LOG_PATH, HTTP_GET, std::bind(&WebSocketLogHandler::forbidden, this, std::placeholders::_1));
  }

  void begin() {
    Logger::getInstance()->addEventHandler(std::bind(&WebSocketLogHandler::logEvent,
                                                     this,
                                                     std::placeholders::_1,
                                                     std::placeholders::_2,
                                                     std::placeholders::_3,
                                                     std::placeholders::_4,
                                                     std::placeholders::_5));
  }

 private:
  AsyncWebSocket _webSocket;

  void forbidden(AsyncWebServerRequest* request) {
    request->send(403);
  }

  void logEvent(const tm* time, LogLevel level, const String& file, const uint16_t line, const String& message) {
    // if there are no clients, don't bother doing anything
    if (!_webSocket.getClients().length()) {
      return;
    }

    // create JsonObject to hold log event
    DynamicJsonDocument jsonDocument = DynamicJsonDocument(WEB_SOCKET_LOG_BUFFER);
    JsonObject logEvent = jsonDocument.to<JsonObject>();
    logEvent["time"] = ESPUtils::toISOString(time, true);
    logEvent["level"] = level;
    logEvent["file"] = file;
    logEvent["line"] = line;
    logEvent["message"] = message;

    // transmit log event to all clients
    size_t len = measureJson(jsonDocument);
    AsyncWebSocketMessageBuffer* buffer = _webSocket.makeBuffer(len);
    if (buffer) {
      serializeJson(jsonDocument, (char*)buffer->get(), len + 1);
      _webSocket.textAll(buffer);
    }
  }
};

#endif
