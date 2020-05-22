#ifndef Logger_h
#define Logger_h

#include <Arduino.h>
#include <list>
#include <functional>
#include <JsonSerializer.h>
#include <time.h>

enum LogLevel { DEBUG = 0, INFO = 1, WARNING = 2, ERROR = 3 };

#define LOGF_D(fmt, ...) Logger::log_P(LogLevel::DEBUG, __FILE__, __LINE__, PSTR(fmt), ##__VA_ARGS__)
#define LOGF_I(fmt, ...) Logger::log_P(LogLevel::INFO, __FILE__, __LINE__, PSTR(fmt), ##__VA_ARGS__)
#define LOGF_W(fmt, ...) Logger::log_P(LogLevel::WARNING, __FILE__, __LINE__, PSTR(fmt), ##__VA_ARGS__)
#define LOGF_E(fmt, ...) Logger::log_P(LogLevel::ERROR, __FILE__, __LINE__, PSTR(fmt), ##__VA_ARGS__)

#define LOG_D(msg) Logger::log(LogLevel::DEBUG, __FILE__, __LINE__, F(msg))
#define LOG_I(msg) Logger::log(LogLevel::INFO, __FILE__, __LINE__, F(msg))
#define LOG_W(msg) Logger::log(LogLevel::WARNING, __FILE__, __LINE__, F(msg))
#define LOG_E(msg) Logger::log(LogLevel::ERROR, __FILE__, __LINE__, F(msg))

typedef size_t log_event_handler_id_t;
typedef std::function<void(time_t time, LogLevel level, const String& file, const uint16_t line, const String& message)>
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

  static void log(LogLevel level, char const* file, int line, const String& message) {
    logEvent(time(nullptr), level, file, line, message);
  }

  static void log_P(LogLevel level, char const* file, int line, PGM_P format, ...) {
    va_list args;
    va_start(args, format);
    logEvent(time(nullptr), level, file, line, printf_P_toString(format, args));
    va_end(args);
  }

 private:
  static std::list<LogEventHandlerInfo> _eventHandlers;

  Logger() {
    // class is static-only, prevent instantiation
  }

  static String printf_P_toString(PGM_P format, va_list args) {
    // inital buffer, we can extend it if the formatted string doesn't fit
    char temp[64];
    char* buffer = temp;
    // try and read into the buffer, exit early if buffer was large enough
    size_t len = vsnprintf_P(temp, sizeof(temp), format, args);
    if (len < sizeof(temp)) {
      return buffer;
    }
    // create a new buffer of the correct length if possible
    buffer = new char[len + 1];
    if (buffer) {
      // format to the buffer
      vsnprintf_P(buffer, len + 1, format, args);
      // return the value as a string after releasing the buffer
      String value = buffer;
      delete[] buffer;
      return value;
    }
    return F("Error formatting log message");
  }

 private:
  static void logEvent(time_t time, LogLevel level, char const* file, int line, const String& message) {
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
