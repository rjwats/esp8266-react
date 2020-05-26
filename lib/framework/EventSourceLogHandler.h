#ifndef EventSourceLogHandler_h
#define EventSourceLogHandler_h

#include <ESPAsyncWebServer.h>
#include <SecurityManager.h>
#include <Logger.h>
#include <ESPUtils.h>
#include <time.h>

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
    Logger::getInstance()->addEventHandler(std::bind(&EventSourceLogHandler::logEvent, this, std::placeholders::_1));
  }

 private:
  AsyncEventSource _events;
  static uint32_t eventId;

  void forbidden(AsyncWebServerRequest* request) {
    request->send(403);
  }

  boolean logEvent(LogEvent& logEvent) {
    // if there are no clients:
    // don't bother doing anything, report it as a success
    if (!_events.count()) {
      return true;
    }

    // create JsonObject to hold log event
    DynamicJsonDocument jsonDocument = DynamicJsonDocument(EVENT_SOURCE_LOG_BUFFER);
    JsonObject jsonObject = jsonDocument.to<JsonObject>();
    jsonObject["id"] = logEvent.id;
    jsonObject["time"] = ESPUtils::toISOString(localtime(&logEvent.time), true);
    jsonObject["level"] = logEvent.level;
    jsonObject["file"] = logEvent.file;
    jsonObject["line"] = logEvent.line;
    jsonObject["message"] = logEvent.message;

    // transmit log event to all clients
    size_t len = measureJson(jsonDocument);

    char* buffer = new char[len + 1];
    if (buffer) {
      serializeJson(jsonDocument, buffer, len + 1);
      _events.send(buffer, "message", millis());
    }
    delete[] buffer;

    // impossible to tell if the buffer is too full.
    return true;
  }
};

#endif
