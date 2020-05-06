#ifndef SettingsSocket_h
#define SettingsSocket_h

#include <SettingsService.h>
#include <SettingsSerializer.h>
#include <SettingsDeserializer.h>
#include <ESPAsyncWebServer.h>

#define MAX_SETTINGS_SOCKET_MSG_SIZE 1024

#define SETTINGS_SOCKET_CLIENT_ID_MSG_SIZE 128
#define SETTINGS_SOCKET_ORIGIN "socket"
#define SETTINGS_SOCKET_CLIENT_ORIGIN_ID_PREFIX "socket:"

/**
 * SettingsSocket is designed to provide WebSocket based communication for making and observing updates to settings.
 */
template <class T>
class SettingsSocket {
 public:
  SettingsSocket(SettingsSerializer<T> settingsSerializer,
                 SettingsDeserializer<T> settingsDeserializer,
                 SettingsService<T>* settingsService,
                 AsyncWebServer* server,
                 char const* socketPath,
                 SecurityManager* securityManager,
                 AuthenticationPredicate authenticationPredicate = AuthenticationPredicates::IS_ADMIN) :
      _settingsSerializer(settingsSerializer),
      _settingsDeserializer(settingsDeserializer),
      _settingsService(settingsService),
      _server(server),
      _webSocket(socketPath) {
    _settingsService->addUpdateHandler([&](String originId) { transmitData(nullptr, originId); }, false);
    _webSocket.setFilter(securityManager->filterRequest(authenticationPredicate));
    _webSocket.onEvent(std::bind(&SettingsSocket::onWSEvent,
                                 this,
                                 std::placeholders::_1,
                                 std::placeholders::_2,
                                 std::placeholders::_3,
                                 std::placeholders::_4,
                                 std::placeholders::_5,
                                 std::placeholders::_6));
    _server->addHandler(&_webSocket);
    _server->on(socketPath, HTTP_GET, std::bind(&SettingsSocket::forbidden, this, std::placeholders::_1));
  }

  SettingsSocket(SettingsSerializer<T> settingsSerializer,
                 SettingsDeserializer<T> settingsDeserializer,
                 SettingsService<T>* settingsService,
                 AsyncWebServer* server,
                 char const* socketPath) :
      _settingsSerializer(settingsSerializer),
      _settingsDeserializer(settingsDeserializer),
      _settingsService(settingsService),
      _server(server),
      _webSocket(socketPath) {
    _settingsService->addUpdateHandler([&](String originId) { transmitData(nullptr, originId); }, false);
    _webSocket.onEvent(std::bind(&SettingsSocket::onWSEvent,
                                 this,
                                 std::placeholders::_1,
                                 std::placeholders::_2,
                                 std::placeholders::_3,
                                 std::placeholders::_4,
                                 std::placeholders::_5,
                                 std::placeholders::_6));
    _server->addHandler(&_webSocket);
  }

 private:
  SettingsSerializer<T> _settingsSerializer;
  SettingsDeserializer<T> _settingsDeserializer;
  SettingsService<T>* _settingsService;
  AsyncWebServer* _server;
  AsyncWebSocket _webSocket;

  /**
   * Renders a forbidden respnose to the client if they fail to connect.
   */
  void forbidden(AsyncWebServerRequest* request) {
    request->send(403);
  }

  /**
   * Responds to the WSEvent by sending the current settings to the clients when they connect and by applying the
   * changes sent to the socket directly to the settings service.
   */
  void onWSEvent(AsyncWebSocket* server,
                 AsyncWebSocketClient* client,
                 AwsEventType type,
                 void* arg,
                 uint8_t* data,
                 size_t len) {
    if (type == WS_EVT_CONNECT) {
      // when a client connects, we transmit it's id and the current payload
      transmitId(client);
      transmitData(client, SETTINGS_SOCKET_ORIGIN);
    } else if (type == WS_EVT_DATA) {
      AwsFrameInfo* info = (AwsFrameInfo*)arg;
      if (info->final && info->index == 0 && info->len == len) {
        if (info->opcode == WS_TEXT) {
          DynamicJsonDocument jsonDocument = DynamicJsonDocument(MAX_SETTINGS_SOCKET_MSG_SIZE);
          DeserializationError error = deserializeJson(jsonDocument, (char*)data);
          if (!error && jsonDocument.is<JsonObject>()) {
            _settingsService->update(
                [&](T& settings) {
                  JsonObject jsonObject = jsonDocument.as<JsonObject>();
                  _settingsDeserializer(jsonObject, settings);
                },
                clientId(client));
          }
        }
      }
    }
  }

  void transmitId(AsyncWebSocketClient* client) {
    DynamicJsonDocument jsonDocument = DynamicJsonDocument(SETTINGS_SOCKET_CLIENT_ID_MSG_SIZE);
    JsonObject root = jsonDocument.to<JsonObject>();
    root["type"] = "id";
    root["id"] = clientId(client);
    size_t len = measureJson(jsonDocument);
    AsyncWebSocketMessageBuffer* buffer = _webSocket.makeBuffer(len);
    if (buffer) {
      serializeJson(jsonDocument, (char*)buffer->get(), len + 1);
      client->text(buffer);
    }
  }

  String clientId(AsyncWebSocketClient* client) {
    return SETTINGS_SOCKET_CLIENT_ORIGIN_ID_PREFIX + String(client->id());
  }

  /**
   * Broadcasts the payload to the destination, if provided. Otherwise broadcasts to all clients except the origin, if
   * specified.
   *
   * Original implementation sent clients their own IDs so they could ignore updates they initiated. This approach
   * simplifies the client and the server implementation but may not be sufficent for all use-cases.
   */
  void transmitData(AsyncWebSocketClient* client, String originId) {
    DynamicJsonDocument jsonDocument = DynamicJsonDocument(MAX_SETTINGS_SOCKET_MSG_SIZE);
    JsonObject root = jsonDocument.to<JsonObject>();
    root["type"] = "payload";
    root["origin_id"] = originId;
    JsonObject payload = root.createNestedObject("payload");
    _settingsService->read([&](T& settings) { _settingsSerializer(settings, payload); });

    size_t len = measureJson(jsonDocument);
    AsyncWebSocketMessageBuffer* buffer = _webSocket.makeBuffer(len);
    if (buffer) {
      serializeJson(jsonDocument, (char*)buffer->get(), len + 1);
      if (client) {
        client->text(buffer);
      } else {
        _webSocket.textAll(buffer);
      }
    }
  }
};
#endif  // end SettingsSocket_h
