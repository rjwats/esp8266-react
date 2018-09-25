#ifndef SimpleSocket_h
#define SimpleSocket_h

#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncJson.h>
#include <ArduinoJson.h>
#include <AsyncJsonRequestWebHandler.h>
#include <AsyncJsonCallbackResponse.h>

class SimpleSocket {

private:

  AsyncWebServer* _server;
  AsyncWebSocket _webSocket;

  private:

    void onWSEvent(AsyncWebSocket * server,
      AsyncWebSocketClient * client, AwsEventType type, void * arg,
      uint8_t *data, size_t len) {

      if (type == WS_EVT_CONNECT) {
        // when a client connects, we transmit it's id and the current payload
        transmitClientId(client);
        transmitPayload(client, "device");
      } else if(type == WS_EVT_DATA) {
        AwsFrameInfo * info = (AwsFrameInfo*)arg;
        if(info->final && info->index == 0 && info->len == len) {
          if(info->opcode == WS_TEXT) {
            DynamicJsonBuffer jsonBuffer;
            JsonVariant json = jsonBuffer.parse((char*)data);
            if (json && json.is<JsonObject>()){
              JsonObject& newConfig = json.as<JsonObject>();
              readFromJsonObject(newConfig, clientId(client));
            }
          }
        }
      }
    }

    void transmitClientId(AsyncWebSocketClient * client){
      String message = "{\"client_id\":\"";
      message += clientId(client);
      message += "\"}";
      client->text(message);
    }

    String clientId(AsyncWebSocketClient * client){
      String clientId = String(client->id());
      return "ws:" + clientId;
    }

    void transmitPayload(AsyncWebSocketClient * client, String originId) {
      DynamicJsonBuffer jsonBuffer;
      JsonObject& root = jsonBuffer.createObject();
      root["origin_id"] = originId;
      JsonObject& payload = root.createNestedObject("payload");

      // write to payload object
      writeToJsonObject(payload);

      size_t len = root.measureLength();
      AsyncWebSocketMessageBuffer * buffer = _webSocket.makeBuffer(len);
      if (buffer) {
          root.printTo((char *)buffer->get(), len + 1);
          if (client) {
              client->text(buffer);
          } else {
              _webSocket.textAll(buffer);
          }
      }
    }

  protected:

    virtual void readFromJsonObject(JsonObject& root, String originId){}
    virtual void writeToJsonObject(JsonObject& root) {}

    void pushPayloadToWebSockets(String originId){
      transmitPayload(NULL, originId);
    }

  public:

    SimpleSocket(AsyncWebServer* server, char const* socketPath):
    _server(server), _webSocket(socketPath)  {
      _webSocket.onEvent(std::bind(&SimpleSocket::onWSEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5, std::placeholders::_6));
      _server->addHandler(&_webSocket);
    }

    virtual ~SimpleSocket() {
      _server->removeHandler(&_webSocket);
    }

};

#endif // end SimpleSocket
