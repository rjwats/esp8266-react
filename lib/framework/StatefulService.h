#ifndef StatefulService_h
#define StatefulService_h

#include <Arduino.h>
#include <JsonDeserializer.h>
#include <JsonSerializer.h>

#include <list>
#include <functional>
#ifdef ESP32
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#endif

#ifndef DEFAULT_BUFFER_SIZE
#define DEFAULT_BUFFER_SIZE 1024
#endif

typedef size_t update_handler_id_t;
typedef std::function<void(String originId)> StateUpdateCallback;
static update_handler_id_t currentUpdatedHandlerId;

typedef struct StateUpdateHandlerInfo {
  update_handler_id_t _id;
  StateUpdateCallback _cb;
  bool _allowRemove;
  StateUpdateHandlerInfo(StateUpdateCallback cb, bool allowRemove) :
      _id(++currentUpdatedHandlerId), _cb(cb), _allowRemove(allowRemove){};
} StateUpdateHandlerInfo_t;

template <class T>
class StatefulService {
 public:
  template <typename... Args>
#ifdef ESP32
  StatefulService(Args&&... args) :
      _state(std::forward<Args>(args)...), _accessMutex(xSemaphoreCreateRecursiveMutex()) {
  }
#else
  StatefulService(Args&&... args) : _state(std::forward<Args>(args)...) {
  }
#endif

  update_handler_id_t addUpdateHandler(StateUpdateCallback cb, bool allowRemove = true) {
    if (!cb) {
      return 0;
    }
    StateUpdateHandlerInfo_t updateHandler(cb, allowRemove);
    _updateHandlers.push_back(updateHandler);
    return updateHandler._id;
  }

  void removeUpdateHandler(update_handler_id_t id) {
    for (auto i = _updateHandlers.begin(); i != _updateHandlers.end();) {
      if ((*i)._allowRemove && (*i)._id == id) {
        i = _updateHandlers.erase(i);
      } else {
        ++i;
      }
    }
  }

  void updateWithoutPropagation(std::function<void(T&)> callback) {
#ifdef ESP32
    xSemaphoreTakeRecursive(_accessMutex, portMAX_DELAY);
#endif
    callback(_state);
#ifdef ESP32
    xSemaphoreGiveRecursive(_accessMutex);
#endif
  }

  void updateWithoutPropagation(JsonObject& jsonObject, JsonDeserializer<T> deserializer) {
#ifdef ESP32
    xSemaphoreTakeRecursive(_accessMutex, portMAX_DELAY);
#endif
    deserializer(jsonObject, _state);
#ifdef ESP32
    xSemaphoreGiveRecursive(_accessMutex);
#endif
  }

  void update(std::function<void(T&)> callback, String originId) {
#ifdef ESP32
    xSemaphoreTakeRecursive(_accessMutex, portMAX_DELAY);
#endif
    callback(_state);
    callUpdateHandlers(originId);
#ifdef ESP32
    xSemaphoreGiveRecursive(_accessMutex);
#endif
  }

  void update(JsonObject& jsonObject, JsonDeserializer<T> deserializer, String originId) {
#ifdef ESP32
    xSemaphoreTakeRecursive(_accessMutex, portMAX_DELAY);
#endif
    deserializer(jsonObject, _state);
    callUpdateHandlers(originId);
#ifdef ESP32
    xSemaphoreGiveRecursive(_accessMutex);
#endif
  }

  void read(std::function<void(T&)> callback) {
#ifdef ESP32
    xSemaphoreTakeRecursive(_accessMutex, portMAX_DELAY);
#endif
    callback(_state);
#ifdef ESP32
    xSemaphoreGiveRecursive(_accessMutex);
#endif
  }

  void read(JsonObject& jsonObject, JsonSerializer<T> serializer) {
#ifdef ESP32
    xSemaphoreTakeRecursive(_accessMutex, portMAX_DELAY);
#endif
    serializer(_state, jsonObject);
#ifdef ESP32
    xSemaphoreGiveRecursive(_accessMutex);
#endif
  }

  void callUpdateHandlers(String originId) {
    for (const StateUpdateHandlerInfo_t& updateHandler : _updateHandlers) {
      updateHandler._cb(originId);
    }
  }

 protected:
  T _state;

 private:
#ifdef ESP32
  SemaphoreHandle_t _accessMutex;
#endif
  std::list<StateUpdateHandlerInfo_t> _updateHandlers;
};

#endif  // end StatefulService_h
