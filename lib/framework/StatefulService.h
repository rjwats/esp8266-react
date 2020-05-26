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

enum class UpdateOutcome { OK = 0, OK_NO_PROPAGATION, INVALID };

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

  UpdateOutcome updateWithoutPropagation(std::function<void(T&)> callback) {
    T newState;
    callback(newState);
    return applyUpdateInMutex(newState);
  }

  UpdateOutcome updateWithoutPropagation(JsonObject& jsonObject, JsonDeserializer<T> deserializer) {
    T newState;
    deserializer(jsonObject, newState);
    return applyUpdateInMutex(newState);
  }

  UpdateOutcome update(std::function<void(T&)> callback, const String& originId) {
    T newState;
    callback(newState);
    return applyUpdateInMutex(newState, [this, originId]() { callUpdateHandlers(originId); });
  }

  UpdateOutcome update(JsonObject& jsonObject, JsonDeserializer<T> deserializer, const String& originId) {
    T newState;
    deserializer(jsonObject, newState);
    return applyUpdateInMutex(newState, [this, originId]() { callUpdateHandlers(originId); });
  }

  T getState() {
    beginTransaction();
    T state(_state);
    endTransaction();
    return state;
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

  /**
   * Stateful services which want to control how updates are processed can do so by overriding this function:
   *
   * A false return value indicates the update was rejected due to the provided state being invalid in some way.
   * Propogation can be prevented by not calling the provided propagate update function.
   */
  virtual UpdateOutcome applyUpdate(T& newState) {
    _state = newState;
    return UpdateOutcome::OK;
  }

 private:
#ifdef ESP32
  SemaphoreHandle_t _accessMutex;
#endif
  std::list<StateUpdateHandlerInfo_t> _updateHandlers;

  UpdateOutcome applyUpdateInMutex(
      T& newState,
      std::function<void()> propagateUpdate = [] {}) {
    beginTransaction();
    UpdateOutcome outcome = applyUpdate(newState);
    endTransaction();
    if (outcome == UpdateOutcome::OK) {
      propagateUpdate();
    }
    return outcome;
  }

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
