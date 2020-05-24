#ifndef SerialLogHandler_h
#define SerialLogHandler_h

#include <Arduino.h>
#include <Logger.h>
#include <ESPUtils.h>

class SerialLogHandler {
 public:
  static void logEvent(tm* time, LogLevel level, char const* file, int line, char const* message) {
    Serial.printf_P(PSTR("%s %s%7s %s%s[%d] %s%s\r\n"),
                    ESPUtils::toHrString(time).c_str(),
                    levelColor(level),
                    levelString(level),
                    COLOR_CYAN,
                    file,
                    line,
                    COLOR_RESET,
                    message);
  }

 private:
  static const char* levelString(LogLevel level) {
    switch (level) {
      case LogLevel::DEBUG:
        return PSTR("DEBUG");
      case LogLevel::INFO:
        return PSTR("INFO");
      case LogLevel::WARNING:
        return PSTR("WARNING");
      case LogLevel::ERROR:
        return PSTR("ERROR");
      default:
        return PSTR("UNKNOWN");
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
