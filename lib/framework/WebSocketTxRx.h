#ifndef WebSocketTxRx_h
#define WebSocketTxRx_h

#include <StatefulService.h>
#include <JsonSerializer.h>
#include <JsonDeserializer.h>
#include <ESPAsyncWebServer.h>

#define WEB_SOCKET_MSG_SIZE 1024
#define WEB_SOCKET_CLIENT_ID_MSG_SIZE 128

#define WEB_SOCKET_ORIGIN "websocket"
#define WEB_SOCKET_ORIGIN_CLIENT_ID_PREFIX "websocket:"

template <class T>
class WebSocketConnector {
 protected:
  StatefulService<T>* _statefulService;
  AsyncWebServer* _server;
  AsyncWebSocket _webSocket;

  WebSocketConnector(StatefulService<T>* statefulService,
                     AsyncWebServer* server,
                     char const* socketPath,
                     SecurityManager* securityManager,
                     AuthenticationPredicate authenticationPredicate = AuthenticationPredicates::IS_ADMIN) :
      _statefulService(statefulService), _server(server), _webSocket(socketPath) {
    _webSocket.setFilter(securityManager->filterRequest(authenticationPredicate));
    _webSocket.onEvent(std::bind(&WebSocketConnector::onWSEvent,
                                 this,
                                 std::placeholders::_1,
                                 std::placeholders::_2,
                                 std::placeholders::_3,
                                 std::placeholders::_4,
                                 std::placeholders::_5,
                                 std::placeholders::_6));
    _server->addHandler(&_webSocket);
    _server->on(socketPath, HTTP_GET, std::bind(&WebSocketConnector::forbidden, this, std::placeholders::_1));
  }

  WebSocketConnector(StatefulService<T>* statefulService, AsyncWebServer* server, char const* socketPath) :
      _statefulService(statefulService), _server(server), _webSocket(socketPath) {
    _webSocket.onEvent(std::bind(&WebSocketConnector::onWSEvent,
                                 this,
                                 std::placeholders::_1,
                                 std::placeholders::_2,
                                 std::placeholders::_3,
                                 std::placeholders::_4,
                                 std::placeholders::_5,
                                 std::placeholders::_6));
    _server->addHandler(&_webSocket);
  }

  virtual void onWSEvent(AsyncWebSocket* server,
                         AsyncWebSocketClient* client,
                         AwsEventType type,
                         void* arg,
                         uint8_t* data,
                         size_t len) = 0;

  String clientId(AsyncWebSocketClient* client) {
    return WEB_SOCKET_ORIGIN_CLIENT_ID_PREFIX + String(client->id());
  }

 private:
  void forbidden(AsyncWebServerRequest* request) {
    request->send(403);
  }
};

template <class T>
class WebSocketTx : virtual public WebSocketConnector<T> {
 public:
  WebSocketTx(JsonSerializer<T> jsonSerializer,
              StatefulService<T>* statefulService,
              AsyncWebServer* server,
              char const* socketPath,
              SecurityManager* securityManager,
              AuthenticationPredicate authenticationPredicate = AuthenticationPredicates::IS_ADMIN) :
      WebSocketConnector<T>(statefulService, server, socketPath, securityManager, authenticationPredicate),
      _jsonSerializer(jsonSerializer) {
    WebSocketConnector<T>::_statefulService->addUpdateHandler([&](String originId) { transmitData(nullptr, originId); },
                                                              false);
  }

  WebSocketTx(JsonSerializer<T> jsonSerializer,
              StatefulService<T>* statefulService,
              AsyncWebServer* server,
              char const* socketPath) :
      WebSocketConnector<T>(statefulService, server, socketPath), _jsonSerializer(jsonSerializer) {
    WebSocketConnector<T>::_statefulService->addUpdateHandler([&](String originId) { transmitData(nullptr, originId); },
                                                              false);
  }

 protected:
  virtual void onWSEvent(AsyncWebSocket* server,
                         AsyncWebSocketClient* client,
                         AwsEventType type,
                         void* arg,
                         uint8_t* data,
                         size_t len) {
    if (type == WS_EVT_CONNECT) {
      // when a client connects, we transmit it's id and the current payload
      transmitId(client);
      transmitData(client, WEB_SOCKET_ORIGIN);
    }
  }

 private:
  JsonSerializer<T> _jsonSerializer;

  void transmitId(AsyncWebSocketClient* client) {
    DynamicJsonDocument jsonDocument = DynamicJsonDocument(WEB_SOCKET_CLIENT_ID_MSG_SIZE);
    JsonObject root = jsonDocument.to<JsonObject>();
    root["type"] = "id";
    root["id"] = WebSocketConnector<T>::clientId(client);
    size_t len = measureJson(jsonDocument);
    AsyncWebSocketMessageBuffer* buffer = WebSocketConnector<T>::_webSocket.makeBuffer(len);
    if (buffer) {
      serializeJson(jsonDocument, (char*)buffer->get(), len + 1);
      client->text(buffer);
    }
  }

  /**
   * Broadcasts the payload to the destination, if provided. Otherwise broadcasts to all clients except the origin, if
   * specified.
   *
   * Original implementation sent clients their own IDs so they could ignore updates they initiated. This approach
   * simplifies the client and the server implementation but may not be sufficent for all use-cases.
   */
  void transmitData(AsyncWebSocketClient* client, String originId) {
    DynamicJsonDocument jsonDocument = DynamicJsonDocument(WEB_SOCKET_MSG_SIZE);
    JsonObject root = jsonDocument.to<JsonObject>();
    root["type"] = "payload";
    root["origin_id"] = originId;
    JsonObject payload = root.createNestedObject("payload");
    WebSocketConnector<T>::_statefulService->read(payload, _jsonSerializer);

    size_t len = measureJson(jsonDocument);
    AsyncWebSocketMessageBuffer* buffer = WebSocketConnector<T>::_webSocket.makeBuffer(len);
    if (buffer) {
      serializeJson(jsonDocument, (char*)buffer->get(), len + 1);
      if (client) {
        client->text(buffer);
      } else {
        WebSocketConnector<T>::_webSocket.textAll(buffer);
      }
    }
  }
};

template <class T>
class WebSocketRx : virtual public WebSocketConnector<T> {
 public:
  WebSocketRx(JsonDeserializer<T> jsonDeserializer,
              StatefulService<T>* statefulService,
              AsyncWebServer* server,
              char const* socketPath,
              SecurityManager* securityManager,
              AuthenticationPredicate authenticationPredicate = AuthenticationPredicates::IS_ADMIN) :
      WebSocketConnector<T>(statefulService, server, socketPath, securityManager, authenticationPredicate),
      _jsonDeserializer(jsonDeserializer) {
  }

  WebSocketRx(JsonDeserializer<T> jsonDeserializer,
              StatefulService<T>* statefulService,
              AsyncWebServer* server,
              char const* socketPath) :
      WebSocketConnector<T>(statefulService, server, socketPath), _jsonDeserializer(jsonDeserializer) {
  }

 protected:
  virtual void onWSEvent(AsyncWebSocket* server,
                         AsyncWebSocketClient* client,
                         AwsEventType type,
                         void* arg,
                         uint8_t* data,
                         size_t len) {
    if (type == WS_EVT_DATA) {
      AwsFrameInfo* info = (AwsFrameInfo*)arg;
      if (info->final && info->index == 0 && info->len == len) {
        if (info->opcode == WS_TEXT) {
          DynamicJsonDocument jsonDocument = DynamicJsonDocument(WEB_SOCKET_MSG_SIZE);
          DeserializationError error = deserializeJson(jsonDocument, (char*)data);
          if (!error && jsonDocument.is<JsonObject>()) {
            JsonObject jsonObject = jsonDocument.as<JsonObject>();
            WebSocketConnector<T>::_statefulService->update(
                jsonObject, _jsonDeserializer, WebSocketConnector<T>::clientId(client));
          }
        }
      }
    }
  }

 private:
  JsonDeserializer<T> _jsonDeserializer;
};

template <class T>
class WebSocketTxRx : public WebSocketTx<T>, public WebSocketRx<T> {
 public:
  WebSocketTxRx(JsonSerializer<T> jsonSerializer,
                JsonDeserializer<T> jsonDeserializer,
                StatefulService<T>* statefulService,
                AsyncWebServer* server,
                char const* socketPath,
                SecurityManager* securityManager,
                AuthenticationPredicate authenticationPredicate = AuthenticationPredicates::IS_ADMIN) :
      WebSocketConnector<T>(statefulService, server, socketPath, securityManager, authenticationPredicate),
      WebSocketTx<T>(jsonSerializer, statefulService, server, socketPath, securityManager, authenticationPredicate),
      WebSocketRx<T>(jsonDeserializer, statefulService, server, socketPath, securityManager, authenticationPredicate) {
  }

  WebSocketTxRx(JsonSerializer<T> jsonSerializer,
                JsonDeserializer<T> jsonDeserializer,
                StatefulService<T>* statefulService,
                AsyncWebServer* server,
                char const* socketPath) :
      WebSocketConnector<T>(statefulService, server, socketPath),
      WebSocketTx<T>(jsonSerializer, statefulService, server, socketPath),
      WebSocketRx<T>(jsonDeserializer, statefulService, server, socketPath) {
  }

 protected:
  void onWSEvent(AsyncWebSocket* server,
                 AsyncWebSocketClient* client,
                 AwsEventType type,
                 void* arg,
                 uint8_t* data,
                 size_t len) {
    WebSocketRx<T>::onWSEvent(server, client, type, arg, data, len);
    WebSocketTx<T>::onWSEvent(server, client, type, arg, data, len);
  }
};

#endif
