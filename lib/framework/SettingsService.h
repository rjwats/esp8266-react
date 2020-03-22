#ifndef SettingsService_h
#define SettingsService_h

#include <list>
#include <functional>
#ifdef ESP32
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#endif

typedef size_t update_handler_id_t;
typedef std::function<void(void* origin)> SettingsUpdateCallback;
static update_handler_id_t currentUpdatedHandlerId;

typedef struct SettingsUpdateHandlerInfo {
  update_handler_id_t _id;
  SettingsUpdateCallback _cb;
  bool _allowRemove;
  SettingsUpdateHandlerInfo(SettingsUpdateCallback cb, bool allowRemove) :
      _id(++currentUpdatedHandlerId),
      _cb(cb),
      _allowRemove(allowRemove){};
} SettingsUpdateHandlerInfo_t;

template <class T>
class SettingsService {
 public:
#ifdef ESP32
  SettingsService() : _updateMutex(xSemaphoreCreateRecursiveMutex()) {
  }
#endif

  update_handler_id_t addUpdateHandler(SettingsUpdateCallback cb, bool allowRemove = true) {
    if (!cb) {
      return 0;
    }
    SettingsUpdateHandlerInfo_t updateHandler(cb, allowRemove);
    _settingsUpdateHandlers.push_back(updateHandler);
    return updateHandler._id;
  }

  void removeUpdateHandler(update_handler_id_t id) {
    for (auto i = _settingsUpdateHandlers.begin(); i != _settingsUpdateHandlers.end();) {
      if ((*i)._id == id) {
        i = _settingsUpdateHandlers.erase(i);
      } else {
        ++i;
      }
    }
  }

  void update(std::function<void(T&)> callback, bool propogate, void* origin = nullptr) {
#ifdef ESP32
    xSemaphoreTakeRecursive(_updateMutex, portMAX_DELAY);
#endif
    callback(_settings);
    if (propogate) {
      callUpdateHandlers(origin);
    }
#ifdef ESP32
    xSemaphoreGiveRecursive(_updateMutex);
#endif
  }

  void update(std::function<void(T&)> callback, void* origin = nullptr) {
    update(callback, true, origin);
  }

  void read(std::function<void(T&)> callback) {
    update(callback, false);
  }

  void callUpdateHandlers(void* origin = nullptr) {
    for (const SettingsUpdateHandlerInfo_t& handler : _settingsUpdateHandlers) {
      handler._cb(origin);
    }
  }

 protected:
  T _settings;
#ifdef ESP32
  SemaphoreHandle_t _updateMutex;
#endif
 private:
  std::list<SettingsUpdateHandlerInfo_t> _settingsUpdateHandlers;
};

#endif  // end SettingsService_h
