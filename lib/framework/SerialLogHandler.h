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
  static boolean logEvent(LogEvent& logEvent) {
    // wait till the entire buffer is free before attempting to write to serial
    // this helps us avoid blocking the main loop unnecessarily
    if (Serial.availableForWrite() < UART_TX_FIFO_SIZE) {
      return false;
    }
    Serial.printf_P(PSTR("%s %s%7s %s%s[%d] %s%s\r\n"),
                    ESPUtils::toHrString(localtime(&logEvent.time)).c_str(),
                    levelColor(logEvent.level),
                    levelString(logEvent.level),
                    COLOR_CYAN,
                    logEvent.file.c_str(),
                    logEvent.line,
                    COLOR_RESET,
                    logEvent.message.c_str());
    return true;
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
