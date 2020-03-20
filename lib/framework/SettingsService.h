#ifndef SettingsService_h
#define SettingsService_h

#include <list>
#include <functional>

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

  void update(std::function<void(T&)> callback, bool propogate = true, void* origin = nullptr) {
    callback(_settings);
    if (propogate) {
      callUpdateHandlers(origin);
    }
  }

  void callUpdateHandlers(void* origin = nullptr) {
    for (const SettingsUpdateHandlerInfo_t& handler : _settingsUpdateHandlers) {
      handler._cb(origin);
    }
  }

 protected:
  T _settings;

 private:
  std::list<SettingsUpdateHandlerInfo_t> _settingsUpdateHandlers;
};

#endif  // end SettingsService
