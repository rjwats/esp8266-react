#ifndef SerialLogHandler_h
#define SerialLogHandler_h

#include <Arduino.h>
#include <Logger.h>
#include <ESPUtils.h>
#include <time.h>

#define COLOR_RESET "\x1B[0m"
#define COLOR_CYAN "\x1B[0;36m"
#define COLOR_BLUE "\x1B[0;34m"
#define COLOR_GREEN "\x1B[0;32m"
#define COLOR_YELLOW "\x1B[0;33m"
#define COLOR_RED "\x1B[0;31m"
#define COLOR_WHITE "\x1B[0;37m"
// #define COLOR_MAGENTA "\x1B[0;35m" - perhaps for trace?

class SerialLogHandler {
 public:
  static void logEvent(const time_t instant, LogLevel level, const String& file, uint16_t line, const String& message) {
    Serial.printf_P(PSTR("%s %s%7s %s%s[%d] %s%s\r\n"),
                    ESPUtils::toHrString(localtime(&instant)).c_str(),
                    levelColor(level),
                    levelString(level),
                    COLOR_CYAN,
                    file.c_str(),
                    line,
                    COLOR_RESET,
                    message.c_str());
  }

 private:
  static const char* levelString(LogLevel level) {
    switch (level) {
      case LogLevel::DEBUG:
        return "DEBUG";
      case LogLevel::INFO:
        return "INFO";
      case LogLevel::WARNING:
        return "WARNING";
      case LogLevel::ERROR:
        return "ERROR";
      default:
        return "UNKNOWN";
    }
  }
  static const char* levelColor(LogLevel level) {
    switch (level) {
      case LogLevel::DEBUG:
        return COLOR_BLUE;
      case LogLevel::INFO:
        return COLOR_GREEN;
      case LogLevel::WARNING:
        return COLOR_YELLOW;
      case LogLevel::ERROR:
        return COLOR_RED;
      default:
        return COLOR_WHITE;
    }
  }
};

#endif
