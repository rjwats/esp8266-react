#ifndef Logger_h
#define Logger_h

#include <Arduino.h>
#include <list>
#include <functional>
#include <JsonSerializer.h>
#include <time.h>

#define COLOR_RESET "\x1B[0m"
#define COLOR_BLACK "\x1B[0;30m"
#define COLOR_RED "\x1B[0;31m"
#define COLOR_GREEN "\x1B[0;32m"
#define COLOR_YELLOW "\x1B[0;33m"
#define COLOR_BLUE "\x1B[0;34m"
#define COLOR_MAGENTA "\x1B[0;35m"
#define COLOR_CYAN "\x1B[0;36m"
#define COLOR_WHITE "\x1B[0;37m"

enum LogLevel { DEBUG = 0, INFO = 1, WARNING = 2, ERROR = 3 };

#define LOGF_D(fmt, ...) Logger::logf(LogLevel::DEBUG, PSTR(__FILE__), __LINE__, PSTR(fmt), ##__VA_ARGS__)
#define LOGF_I(fmt, ...) Logger::logf(LogLevel::INFO, PSTR(__FILE__), __LINE__, PSTR(fmt), ##__VA_ARGS__)
#define LOGF_W(fmt, ...) Logger::logf(LogLevel::WARNING, PSTR(__FILE__), __LINE__, PSTR(fmt), ##__VA_ARGS__)
#define LOGF_E(fmt, ...) Logger::logf(LogLevel::ERROR, PSTR(__FILE__), __LINE__, PSTR(fmt), ##__VA_ARGS__)

#define LOG_D(msg) Logger::log(LogLevel::DEBUG, PSTR(__FILE__), __LINE__, PSTR(msg))
#define LOG_I(msg) Logger::log(LogLevel::INFO, PSTR(__FILE__), __LINE__, PSTR(msg))
#define LOG_W(msg) Logger::log(LogLevel::WARNING, PSTR(__FILE__), __LINE__, PSTR(msg))
#define LOG_E(msg) Logger::log(LogLevel::ERROR, PSTR(__FILE__), __LINE__, PSTR(msg))

typedef size_t log_event_handler_id_t;
typedef std::function<void(time_t time, LogLevel level, const char* file, const uint16_t line, const char* message)>
    LogEventHandler;

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

  static void log(LogLevel level, const char* file, int line, const char* message) {
    logEvent(time(nullptr), level, file, line, message);
  }

  static void logf(LogLevel level, const char* file, int line, const char* format, ...) {
    va_list args;

    // inital buffer, we can extend it if the formatted string doesn't fit
    char temp[64];
    va_start(args, format);
    size_t len = vsnprintf_P(temp, sizeof(temp), format, args);
    va_end(args);

    // buffer was large enough - log and exit early
    if (len < sizeof(temp)) {
      logEvent(time(nullptr), level, file, line, temp);
      return;
    }

    // create a new buffer of the correct length if possible
    char* buffer = new char[len + 1];
    if (buffer) {
      vsnprintf_P(buffer, len + 1, format, args);
      logEvent(time(nullptr), level, file, line, buffer);
      delete[] buffer;
      return;
    }

    // we failed to allocate
    logEvent(time(nullptr), level, file, line, PSTR("Error formatting log message"));
  }

 private:
  static std::list<LogEventHandlerInfo> _eventHandlers;

  Logger() {
    // class is static-only, prevent instantiation
  }

  static void logEvent(time_t time, LogLevel level, char const* file, int line, char const* message) {
    for (const LogEventHandlerInfo& eventHandler : _eventHandlers) {
      eventHandler._cb(time, level, file, line, message);
    }
  }
};

class SerialLogger {
 public:
  static void logEvent(time_t time, LogLevel level, char const* file, int line, char const* message) {
    Serial.print(time);
    Serial.print(" ");
    Serial.print(level);
    Serial.print(" ");
    Serial.print(file);
    Serial.print(" ");
    Serial.print(line);
    Serial.print(" ");
    Serial.print(message);
    Serial.println();
  }
};

#endif
