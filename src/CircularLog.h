#ifndef CircularLog_h
#define CircularLog_h

#ifdef ESP32
#include <SPIFFS.h>
#elif defined(ESP8266)
#include <FS.h>
#endif

#define INVALID_SLOT 1

template <typename T>
class CircularLog {
 private:
  char const* _filePath;
  uint16_t _numSlots;
  uint32_t _fileSize;

  bool shouldCreateFile() {
    if (SPIFFS.exists(_filePath)) {
      File f = SPIFFS.open(_filePath, "r");
      return f.size() != _fileSize;
    }
    return true;
  }

  void createFileIfRequired() {
    if (shouldCreateFile()) {
      File logFile = SPIFFS.open(_filePath, "w");
      for (int i = 0; i < _fileSize; i++) {
        logFile.write(0);
      }
    }
  }

 public:
  CircularLog(char const* filePath, uint16_t _numSlots) :
      _filePath(filePath),
      _numSlots(_numSlots),
      _fileSize(sizeof(T) * _numSlots) {
  }
  virtual ~CircularLog() {
  }

  void begin() {
    createFileIfRequired();
  }

  /**
   * Writes a single slot to the log
   **/
  uint8_t writeEntry(T* value, uint16_t slot) {
    if (slot >= _numSlots) {
      return INVALID_SLOT;
    }
    File logFile = SPIFFS.open(_filePath, "r+");
    logFile.seek(slot * sizeof(T));
    logFile.write((uint8_t*)value, sizeof(T));
    return 0;
  }

  template <typename Func>
  void readEntries(uint16_t startingSlot, uint16_t maxEntries, Func consume) {
    T value;
    File logFile = SPIFFS.open(_filePath, "r");
    logFile.seek(startingSlot * sizeof(T));
    uint16_t currentSlot = startingSlot;
    for (uint16_t i = 0; i < maxEntries; i++) {
      logFile.read((uint8_t*)&value, sizeof(T));
      consume(&value);
      currentSlot++;
      if (currentSlot >= _numSlots) {
        currentSlot = 0;
        logFile.seek(0);
      }
    }
  }
};

#endif  // end CircularLog_h
