#ifndef Logger_h
#define Logger_h

#include <Arduino.h>
#include <list>
#include <functional>
#include <time.h>
#include <string>
#include <FS.h>
#include <JsonSerializer.h>
#include <JsonDeserializer.h>

#define LOGGER_FS_BUFFER_PATH "/log/"
#define FSH_P const __FlashStringHelper*
#define LOG_ENTRY_SIZE 512

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
typedef std::function<void(time_t time, LogLevel level, String& file, uint16_t line, String& message)> LogEventHandler;

typedef struct LogEventHandlerInfo {
  static log_event_handler_id_t currentEventHandlerId;
  log_event_handler_id_t _id;
  LogEventHandler _cb;
  LogEventHandlerInfo(LogEventHandler cb) : _id(++currentEventHandlerId), _cb(cb){};
} LogEventHandlerInfo_t;

class LogEvent {
 public:
  time_t time;
  LogLevel level;
  String file;
  uint16_t line;
  String message;

  static void serialize(LogEvent& logEvent, JsonObject& root) {
    root["time"] = logEvent.time;
    root["level"] = (uint8_t)logEvent.level;
    root["file"] = logEvent.file;
    root["line"] = logEvent.line;
    root["message"] = logEvent.message;
  }

  static void deserialize(JsonObject& root, LogEvent& logEvent) {
    logEvent.time = root["time"];
    logEvent.level = (LogLevel)root["level"].as<uint8_t>();
    logEvent.file = root["file"] | "";
    logEvent.line = root["line"];
    logEvent.message = root["message"] | "";
  }
};

class Logger {
 public:
  static void log(LogLevel level, FSH_P file, uint16_t line, FSH_P message) {
    if (_instance) {
      _instance->logEvent(level, file, line, message);
    }
  }

  static void logf(LogLevel level, FSH_P file, uint16_t line, PGM_P format, ...) {
    va_list args;

    // exit early if logging has not been started
    if (!_instance) {
      return;
    }

    // inital buffer, we can extend it if the formatted string doesn't fit
    char temp[64];
    va_start(args, format);
    size_t len = vsnprintf_P(temp, sizeof(temp), format, args);
    va_end(args);

    // buffer was large enough - log and exit early
    if (len < sizeof(temp)) {
      _instance->logEvent(level, file, line, temp);
      return;
    }

    // create a new buffer of the correct length if possible
    char* buffer = new char[len + 1];
    if (buffer) {
      vsnprintf_P(buffer, len + 1, format, args);
      _instance->logEvent(level, file, line, buffer);
      delete[] buffer;
      return;
    }

    // we failed to allocate
    _instance->logEvent(level, file, line, F("Error formatting log message"));
  }

  static void begin(FS* fs) {
    if (!_instance) {
      _instance = new Logger(fs, LOGGER_FS_BUFFER_PATH);
      _instance->begin();
    }
  }

  static Logger* getInstance() {
    return _instance;
  }

  log_event_handler_id_t addEventHandler(LogEventHandler cb) {
    if (!cb) {
      return 0;
    }
    LogEventHandlerInfo eventHandler(cb);
    _eventHandlers.push_back(eventHandler);
    return eventHandler._id;
  }

  void removeEventHandler(log_event_handler_id_t id) {
    for (auto i = _eventHandlers.begin(); i != _eventHandlers.end();) {
      if ((*i)._id == id) {
        i = _eventHandlers.erase(i);
      } else {
        ++i;
      }
    }
  }

  void logEvent(LogLevel level, String file, uint16_t line, String message) {
    time_t now = time(nullptr);
    for (const LogEventHandlerInfo& eventHandler : _eventHandlers) {
      eventHandler._cb(now, level, file, line, message);
    }
  }

 private:
  FS* _fs;
  std::list<LogEventHandlerInfo> _eventHandlers;

  // the directory where the logger will keep its buffer
  char const* _bufferPath;

  // how many log entries to store per file, how many files to store in total
  uint16_t _entriesPerFile;
  uint16_t _maxFileCount;

  // an in-memory copy of recent log events
  std::list<LogEvent> _buffer;
  uint32_t _maxBufferSize;

  // the id of the latest log event in the buffer
  String _latestFileName;
  uint32_t _latestFileIndex;
  uint32_t _latestEventId;

  // the id of the latest log event we flushed to the filesystem
  uint32_t _flushedEventId;

  static Logger* _instance;

  Logger(FS* fs, char const* bufferPath) :
      _fs(fs),
      _eventHandlers(),
      _bufferPath(bufferPath),
      _entriesPerFile(100),
      _maxFileCount(10),
      _buffer(),
      _maxBufferSize(10),
      _latestFileName(),
      _latestFileIndex(0),
      _latestEventId(0),
      _flushedEventId(0) {
  }

  String formatEventId(uint32_t eventId) {
    char fileName[11] = {0};
    sprintf(fileName, "%010d", eventId);
    return fileName;
  }

  uint32_t parseEventId(String eventId) {
    return strtoul(eventId.c_str(), nullptr, 10);
  }

  void begin() {
    // find the "latest" file
    Dir dir = SPIFFS.openDir(_bufferPath);
    _latestFileIndex = 0;
    while (dir.next()) {
      uint32_t fileIndex = parseEventId(dir.fileName());
      if (fileIndex > _latestFileIndex) {
        _latestFileIndex = fileIndex;
      }
    }

    // calculate the latest file name
    // this will result in 0000000000 if no file is found
    // use _latestFileIndex as the initial latest event id too
    _latestFileName = _bufferPath + formatEventId(_latestFileIndex);
    _latestEventId = _latestFileIndex;
    _flushedEventId = _latestFileIndex;

    // TODO - this could be done more efficently
    // parse latest file in order to calculate _latestEventId and _flushedEventId
    if (SPIFFS.exists(_latestFileName)) {
      File file = _fs->open(_latestFileName, "r");
      DynamicJsonDocument jsonDocument = DynamicJsonDocument(LOG_ENTRY_SIZE);
      while (file.available()) {
        // deserialize next entry
        deserializeJson(jsonDocument, file.readStringUntil('\n'));

        // chuck the next even on the pack of the buffer
        LogEvent logEvent;
        JsonObject jsonObject = jsonDocument.as<JsonObject>();
        LogEvent::deserialize(jsonObject, logEvent);
        _buffer.push_back(logEvent);

        // increment latest event id
        _latestEventId++;

        // prune buffer as required
        if (_buffer.size() > _maxBufferSize) {
          _buffer.pop_front();
        }
      }
      _flushedEventId = _latestEventId;
    }
  }
};

#endif
