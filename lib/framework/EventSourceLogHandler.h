#ifndef EventSourceLogHandler_h
#define EventSourceLogHandler_h

#include <ESPAsyncWebServer.h>
#include <SecurityManager.h>
#include <Logger.h>
#include <ESPUtils.h>

#define EVENT_SOURCE_LOG_PATH "/es/log"
#define EVENT_SOURCE_LOG_BUFFER 512

class EventSourceLogHandler {
 public:
  EventSourceLogHandler(AsyncWebServer* server, SecurityManager* securityManager) : _events(EVENT_SOURCE_LOG_PATH) {
    _events.setFilter(securityManager->filterRequest(AuthenticationPredicates::IS_ADMIN));
    server->addHandler(&_events);
    server->on(
        EVENT_SOURCE_LOG_PATH, HTTP_GET, std::bind(&EventSourceLogHandler::forbidden, this, std::placeholders::_1));
  }

  void begin() {
    Logger::getInstance()->addEventHandler(std::bind(&EventSourceLogHandler::logEvent,
                                                     this,
                                                     std::placeholders::_1,
                                                     std::placeholders::_2,
                                                     std::placeholders::_3,
                                                     std::placeholders::_4,
                                                     std::placeholders::_5));
  }

 private:
  AsyncEventSource _events;
  static uint32_t eventId;

  void forbidden(AsyncWebServerRequest* request) {
    request->send(403);
  }

  void logEvent(const tm* time, LogLevel level, const String& file, const uint16_t line, const String& message) {
    // if there are no clients, don't bother doing anything
    if (!_events.count()) {
      return;
    }

    // create JsonObject to hold log event
    DynamicJsonDocument jsonDocument = DynamicJsonDocument(EVENT_SOURCE_LOG_BUFFER);
    JsonObject logEvent = jsonDocument.to<JsonObject>();
    logEvent["time"] = ESPUtils::toISOString(time, true);
    logEvent["level"] = level;
    logEvent["file"] = file;
    logEvent["line"] = line;
    logEvent["message"] = message;

    // transmit log event to all clients
    size_t len = measureJson(jsonDocument);
    char* buffer = new char[len + 1];
    if (buffer) {
      serializeJson(jsonDocument, buffer, len + 1);
      _events.send(buffer, "message", millis());
    }
    delete[] buffer;
  }
};

#endif
