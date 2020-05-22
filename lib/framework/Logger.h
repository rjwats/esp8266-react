#ifndef Logger_h
#define Logger_h

#include <Arduino.h>
#include <list>
#include <functional>

enum LogLevel { DEBUG = 0, INFO = 1, WARNING = 2, ERROR = 3 };

#define LOG_D(fmt, ...) Logger::log_P(LogLevel::DEBUG, "PSTR(%s)" fmt, ##__VA_ARGS__)
#define LOG_I(fmt, ...) Logger::log_P(LogLevel::INFO, "PSTR(%s)" fmt, ##__VA_ARGS__)
#define LOG_W(fmt, ...) Logger::log_P(LogLevel::WARNING, "PSTR(%s)" fmt, ##__VA_ARGS__)
#define LOG_E(fmt, ...) Logger::log_P(LogLevel::ERROR, "PSTR(%s)" fmt, ##__VA_ARGS__)

/*
#define LOG_D(msg) Logger.log(LogLevel::DEBUG, "%s " msg, __func__, ##__VA_ARGS__)
#define LOG_I(msg) Logger.log(LogLevel::INFO, "%s " msg, __func__, ##__VA_ARGS__)
#define LOG_W(msg) Logger.log(LogLevel::WARNING, "%s " msg, __func__, ##__VA_ARGS__)
#define LOG_E(msg) Logger.log(LogLevel::ERROR, "%s " msg, __func__, ##__VA_ARGS__)
*/

typedef size_t log_event_handler_id_t;
typedef std::function<void(const String& name, LogLevel level, const String& message)> LogEventHandler;

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

  static void log(LogLevel level, const String& message) {
    Serial.print(level);
    Serial.print(message);
    Serial.println();
    /*
    for (auto i = _eventHandlers.begin(); i != _eventHandlers.end();) {
      (*i)._cb(name, level, message);
    }
    */
  }

  static void log_P(LogLevel level, PGM_P format, ...) {
    va_list args;

    va_start(args, format);
    log(level, printf_PToString(format, args));
    va_end(args);
  }

 private:
  static std::list<LogEventHandlerInfo> _eventHandlers;
  Logger() {
    // class is static-only, prevent instantiation
  }
  static String printf_PToString(PGM_P format, ...) {
    va_list arg;

    // inital buffer, we can extend it if the formatted string doesn't fit
    char temp[64];
    char* buffer = temp;

    // try and read into the buffer, exit early if buffer was large enough
    va_start(arg, format);
    size_t len = vsnprintf_P(temp, sizeof(temp), format, arg);
    va_end(arg);
    if (len < sizeof(temp)) {
      return buffer;
    }

    // create a new buffer of the correct length, if possible
    buffer = new char[len + 1];
    if (buffer) {
      // format to the buffer
      va_start(arg, format);
      vsnprintf_P(buffer, len + 1, format, arg);
      va_end(arg);

      // return the value as a string after releasing the buffer
      String value = buffer;
      delete[] buffer;
      return value;
    }

    return F("Error formatting log message");
  }
};
#endif
