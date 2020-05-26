#ifndef WebSocketLogHandler_h
#define WebSocketLogHandler_h

#include <ESPAsyncWebServer.h>
#include <SecurityManager.h>
#include <Logger.h>
#include <ESPUtils.h>
#include <time.h>

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
    Logger::getInstance()->addEventHandler(std::bind(&WebSocketLogHandler::logEvent, this, std::placeholders::_1));
  }

 private:
  AsyncWebSocket _webSocket;

  void forbidden(AsyncWebServerRequest* request) {
    request->send(403);
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
