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

enum class UpdateOutcome { CHANGED = 0, UNCHANGED, ERROR };

template <class T>
using UpdateFunction = UpdateOutcome (*)(T& settings);

template <class T>
using JsonUpdateFunction = UpdateOutcome (*)(JsonObject& state, T& settings);

typedef size_t update_handler_id_t;
typedef std::function<void(const String& originId)> StateUpdateCallback;

typedef struct StateUpdateHandlerInfo {
  static update_handler_id_t currentUpdatedHandlerId;
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

  UpdateOutcome update(UpdateFunction<T> updateFunction, const String& originId) {
    beginTransaction();
    UpdateOutcome outcome = updateFunction(_state);
    if (outcome == UpdateOutcome::CHANGED) {
      callUpdateHandlers(originId);
    }
    endTransaction();
    return outcome;
  }

  UpdateOutcome updateWithoutPropagation(UpdateFunction<T> updateFunction) {
    beginTransaction();
    UpdateOutcome outcome = updateFunction(_state);
    endTransaction();
    return outcome;
  }

  UpdateOutcome update(JsonObject& jsonObject, JsonUpdateFunction<T> jsonUpdateFunction, const String& originId) {
    beginTransaction();
    UpdateOutcome outcome = jsonUpdateFunction(jsonObject, _state);
    if (outcome == UpdateOutcome::CHANGED) {
      callUpdateHandlers(originId);
    }
    endTransaction();
    return outcome;
  }

  UpdateOutcome updateWithoutPropagation(JsonObject& jsonObject, JsonUpdateFunction<T> jsonUpdateFunction) {
    beginTransaction();
    UpdateOutcome outcome = jsonUpdateFunction(jsonObject, _state);
    endTransaction();
    return outcome;
  }

  void populate(JsonObject& jsonObject, JsonDeserializer<T> deserializer, const String& originId) {
    beginTransaction();
    deserializer(jsonObject, _state);
    callUpdateHandlers(originId);
    endTransaction();
  }

  void populateWithoutPropagation(JsonObject& jsonObject, JsonDeserializer<T> deserializer) {
    beginTransaction();
    deserializer(jsonObject, _state);
    endTransaction();
  }

  void read(std::function<void(T&)> callback) {
    beginTransaction();
    callback(_state);
    endTransaction();
  }

  void read(JsonObject& jsonObject, JsonSerializer<T> serializer) {
    beginTransaction();
    serializer(_state, jsonObject);
    endTransaction();
  }

  void callUpdateHandlers(const String& originId) {
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

  inline void beginTransaction() {
#ifdef ESP32
    xSemaphoreTakeRecursive(_accessMutex, portMAX_DELAY);
#endif
  }

  inline void endTransaction() {
#ifdef ESP32
    xSemaphoreGiveRecursive(_accessMutex);
#endif
  }
};

#endif  // end StatefulService_h
