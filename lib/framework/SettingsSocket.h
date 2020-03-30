#ifndef SettingsSocket_h
#define SettingsSocket_h

#include <SettingsService.h>
#include <SettingsSerializer.h>
#include <SettingsDeserializer.h>
#include <ESPAsyncWebServer.h>

#define MAX_SIMPLE_MSG_SIZE 1024

/**
 * SettingsSocket is designed to provide WebSocket based communication for making and observing updates to settings.
 *
 * TODO - Security via a parameter, optional on construction to start!
 */
template <class T>
class SettingsSocket {
 public:
  SettingsSocket(SettingsSerializer<T>* settingsSerializer,
                 SettingsDeserializer<T>* settingsDeserializer,
                 SettingsService<T>* settingsService,
                 AsyncWebServer* server,
                 char const* socketPath) :
      _settingsSerializer(settingsSerializer),
      _settingsDeserializer(settingsDeserializer),
      _settingsService(settingsService),
      _server(server),
      _webSocket(socketPath) {
    _webSocket.onEvent(std::bind(&SettingsSocket::onWSEvent,
                                 this,
                                 std::placeholders::_1,
                                 std::placeholders::_2,
                                 std::placeholders::_3,
                                 std::placeholders::_4,
                                 std::placeholders::_5,
                                 std::placeholders::_6));
    _settingsService->addUpdateHandler([&](void* origin) { broadcastPayload(nullptr, origin); }, false);
    _server->addHandler(&_webSocket);
  }

 private:
  SettingsSerializer<T>* _settingsSerializer;
  SettingsDeserializer<T>* _settingsDeserializer;
  SettingsService<T>* _settingsService;
  AsyncWebServer* _server;
  AsyncWebSocket _webSocket;

  /**
   * Responds to the WSEvent by sending the current payload to the clients when they connect and by applying the changes
   * sent to the socket directly to the settings service.
   */
  void onWSEvent(AsyncWebSocket* server,
                 AsyncWebSocketClient* client,
                 AwsEventType type,
                 void* arg,
                 uint8_t* data,
                 size_t len) {
    if (type == WS_EVT_CONNECT) {
      // send the payload to the client upon connect
      broadcastPayload(client);
    } else if (type == WS_EVT_DATA) {
      AwsFrameInfo* info = (AwsFrameInfo*)arg;
      // we only support single frame messages. That gives us 2930 bytes, should be plenty!
      if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
        DynamicJsonDocument json = DynamicJsonDocument(MAX_SIMPLE_MSG_SIZE);
        DeserializationError error = deserializeJson(json, (char*)data, len);
        if (!error && json.is<JsonObject>()) {
          _settingsService->update(
              [&](T& settings) { _settingsDeserializer->deserialize(settings, json.as<JsonObject>()); }, client);
        }
      }
    }
  }

  /**
   * Broadcasts the payload to the destination, if provided. Otherwise broadcasts to all clients except the origin, if
   * specified.
   *
   * Original implementation sent clients their own IDs so they could ignore updates they initiated. This approach
   * simplifies the client and the server implementation but may not be sufficent for all use-cases.
   */
  void broadcastPayload(AsyncWebSocketClient* destination = nullptr, void* origin = nullptr) {
    // write the payload to the json object
    DynamicJsonDocument json = DynamicJsonDocument(MAX_SIMPLE_MSG_SIZE);
    _settingsService->read([&](T& settings) { _settingsSerializer->serialize(settings, json.to<JsonObject>()); });

    // construct a WS buffer of the correct size (if possible)
    size_t len = measureJson(json);
    AsyncWebSocketMessageBuffer* buffer = _webSocket.makeBuffer(len);
    if (!buffer) {
      Serial.println("Failed to create WS buffer");
      return;
    }

    // serialize the payload to the buffer
    serializeJson(json, (char*)buffer->get(), len + 1);

    // broadcast the payload as required
    if (destination != nullptr) {
      if (destination->status() == WS_CONNECTED) {
        destination->text(buffer);
      }
    } else if (origin == nullptr) {
      _webSocket.textAll(buffer);
    } else {
      buffer->lock();
      for (const auto& client : _webSocket.getClients()) {
        if (client != origin && client->status() == WS_CONNECTED) {
          client->text(buffer);
        }
      }
      buffer->unlock();
    }
  }
};
#endif  // end SettingsSocket_h
