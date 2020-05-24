#ifndef Logger_h
#define Logger_h

#include <Arduino.h>
#include <list>
#include <functional>
#include <time.h>

#define FSH_P const __FlashStringHelper*

enum LogLevel { DEBUG = 0, INFO = 1, WARNING = 2, ERROR = 3 };

#define LOGF_D(fmt, ...) Logger::logf(LogLevel::DEBUG, F(__FILE__), __LINE__, PSTR(fmt), ##__VA_ARGS__)
#define LOGF_I(fmt, ...) Logger::logf(LogLevel::INFO, F(__FILE__), __LINE__, PSTR(fmt), ##__VA_ARGS__)
#define LOGF_W(fmt, ...) Logger::logf(LogLevel::WARNING, F(__FILE__), __LINE__, PSTR(fmt), ##__VA_ARGS__)
#define LOGF_E(fmt, ...) Logger::logf(LogLevel::ERROR, F(__FILE__), __LINE__, PSTR(fmt), ##__VA_ARGS__)

#define LOG_D(msg) Logger::log(LogLevel::DEBUG, F(__FILE__), __LINE__, F(msg))
#define LOG_I(msg) Logger::log(LogLevel::INFO, F(__FILE__), __LINE__, F(msg))
#define LOG_W(msg) Logger::log(LogLevel::WARNING, F(__FILE__), __LINE__, F(msg))
#define LOG_E(msg) Logger::log(LogLevel::ERROR, F(__FILE__), __LINE__, F(msg))

typedef size_t log_event_handler_id_t;
typedef std::function<void(tm* time, LogLevel level, String& file, int line, String& message)> LogEventHandler;

typedef struct LogEventHandlerInfo {
  static log_event_handler_id_t currentEventHandlerId;
  log_event_handler_id_t _id;
  LogEventHandler _cb;
  LogEventHandlerInfo(LogEventHandler cb) : _id(++currentEventHandlerId), _cb(cb){};
} LogEventHandlerInfo_t;

class Logger {
 public:
  static log_event_handler_id_t addEventHandler(LogEventHandler cb) {
    if (!cb) {
      return 0;
    }
    LogEventHandlerInfo eventHandler(cb);
    _eventHandlers.push_back(eventHandler);
    return eventHandler._id;
  }

  static void removeEventHandler(log_event_handler_id_t id) {
    for (auto i = _eventHandlers.begin(); i != _eventHandlers.end();) {
      if ((*i)._id == id) {
        i = _eventHandlers.erase(i);
      } else {
        ++i;
      }
    }
  }

  static void log(LogLevel level, FSH_P file, int line, FSH_P message) {
    logEvent(level, file, line, message);
  }

  static void logf(LogLevel level, FSH_P file, int line, PGM_P format, ...) {
    va_list args;

    // inital buffer, we can extend it if the formatted string doesn't fit
    char temp[64];
    va_start(args, format);
    size_t len = vsnprintf_P(temp, sizeof(temp), format, args);
    va_end(args);

    // buffer was large enough - log and exit early
    if (len < sizeof(temp)) {
      logEvent(level, file, line, temp);
      return;
    }

    // create a new buffer of the correct length if possible
    char* buffer = new char[len + 1];
    if (buffer) {
      vsnprintf_P(buffer, len + 1, format, args);
      logEvent(level, file, line, buffer);
      delete[] buffer;
      return;
    }

    // we failed to allocate
    logEvent(level, file, line, F("Error formatting log message"));
  }

 private:
  static std::list<LogEventHandlerInfo> _eventHandlers;

  Logger() {
    // class is static-only, prevent instantiation
  }

  static void logEvent(LogLevel level, String file, int line, String message) {
    time_t now = time(nullptr);
    tm* time = localtime(&now);
    for (const LogEventHandlerInfo& eventHandler : _eventHandlers) {
      eventHandler._cb(time, level, file, line, message);
    }
  }
};

#endif
