#ifndef CircularLog_h
#define CircularLog_h

#include <FS.h>

#define INVALID_SLOT 1

template<typename T>
class CircularLog {

private:

    char const* _filePath;
    uint16_t _numSlots;
    uint32_t _fileSize;

    void createFileIfRequired() {
      if (!SPIFFS.exists(_filePath)){
        File logFile = SPIFFS.open(_filePath, "w");
        for (int i=0;i<_fileSize;i++){
          logFile.write(0);
        }
      }
    }

  public:

    CircularLog(char const* filePath, uint16_t _numSlots):
      _filePath(filePath),
      _numSlots(_numSlots),
      _fileSize(sizeof(T) * _numSlots) {}
    virtual ~CircularLog() {}

    void begin() {
      createFileIfRequired();
    }

    /**
    * Writes a single slot to the log
    **/
    uint8_t writeEntry(T *value, uint16_t slot) {
      if (slot >= _numSlots) {
        return INVALID_SLOT;
      }
      File logFile = SPIFFS.open(_filePath, "r+");
      logFile.seek(slot * sizeof(T));
      logFile.write((uint8_t *) value, sizeof(T));
      return 0;
    }

    /**
    * Reads all slots from the file, piping the results back to the callback as
    * pointers to an instance of T.
    *
    * Starts at the slot requested, looping round in circular fashion until all
    * slots have been deserialized and returned.
    **/
    template < typename Func >
    void readAllEntries(uint16_t startingSlot, Func consumer) {
      T value;
      File logFile = SPIFFS.open(_filePath, "r");
      logFile.seek(startingSlot * sizeof(T));
      uint16_t currentSlot = startingSlot;
      do {
        logFile.read((uint8_t *) &value, sizeof(T));
        consumer(&value);
        currentSlot++;
        if (currentSlot >= _numSlots){
          currentSlot = 0;
          logFile.seek(0);
        }
      } while (currentSlot != startingSlot);
    }

};

#endif // end CircularLog_h
