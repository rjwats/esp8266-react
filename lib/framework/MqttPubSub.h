#ifndef MqttPubSub_h
#define MqttPubSub_h

#include <SettingsService.h>
#include <JsonSerializer.h>
#include <JsonDeserializer.h>
#include <AsyncMqttClient.h>

#define MAX_MESSAGE_SIZE 1024
#define MQTT_ORIGIN_ID "mqtt"

template <class T>
class MqttConnector {
 protected:
  SettingsService<T>* _settingsService;
  AsyncMqttClient* _mqttClient;

  MqttConnector(SettingsService<T>* settingsService, AsyncMqttClient* mqttClient) :
      _settingsService(settingsService), _mqttClient(mqttClient) {
    _mqttClient->onConnect(std::bind(&MqttConnector::onConnect, this));
  }

  virtual void onConnect() = 0;
};

template <class T>
class MqttPub : virtual public MqttConnector<T> {
 public:
  MqttPub(JsonSerializer<T> jsonSerializer,
          SettingsService<T>* settingsService,
          AsyncMqttClient* mqttClient,
          String pubTopic = "") :
      MqttConnector<T>(settingsService, mqttClient), _jsonSerializer(jsonSerializer), _pubTopic(pubTopic) {
    MqttConnector<T>::_settingsService->addUpdateHandler([&](String originId) { publish(); }, false);
  }

  void setPubTopic(String pubTopic) {
    _pubTopic = pubTopic;
    publish();
  }

 protected:
  virtual void onConnect() {
    publish();
  }

 private:
  JsonSerializer<T> _jsonSerializer;
  String _pubTopic;

  void publish() {
    if (_pubTopic.length() > 0 && MqttConnector<T>::_mqttClient->connected()) {
      // serialize to json doc
      DynamicJsonDocument json(MAX_MESSAGE_SIZE);
      JsonObject jsonObject = json.to<JsonObject>();
      MqttConnector<T>::_settingsService->read(jsonObject, _jsonSerializer);

      // serialize to string
      String payload;
      serializeJson(json, payload);

      // publish the payload
      MqttConnector<T>::_mqttClient->publish(_pubTopic.c_str(), 0, false, payload.c_str());
    }
  }
};

template <class T>
class MqttSub : virtual public MqttConnector<T> {
 public:
  MqttSub(JsonDeserializer<T> jsonDeserializer,
          SettingsService<T>* settingsService,
          AsyncMqttClient* mqttClient,
          String subTopic = "") :
      MqttConnector<T>(settingsService, mqttClient), _jsonDeserializer(jsonDeserializer), _subTopic(subTopic) {
    MqttConnector<T>::_mqttClient->onMessage(std::bind(&MqttSub::onMqttMessage,
                                                       this,
                                                       std::placeholders::_1,
                                                       std::placeholders::_2,
                                                       std::placeholders::_3,
                                                       std::placeholders::_4,
                                                       std::placeholders::_5,
                                                       std::placeholders::_6));
  }

  void setSubTopic(String subTopic) {
    if (!_subTopic.equals(subTopic)) {
      // unsubscribe from the existing topic if one was set
      if (_subTopic.length() > 0) {
        MqttConnector<T>::_mqttClient->unsubscribe(_subTopic.c_str());
      }
      // set the new topic and re-configure the subscription
      _subTopic = subTopic;
      subscribe();
    }
  }

 protected:
  virtual void onConnect() {
    subscribe();
  }

 private:
  JsonDeserializer<T> _jsonDeserializer;
  String _subTopic;

  void subscribe() {
    if (_subTopic.length() > 0) {
      MqttConnector<T>::_mqttClient->subscribe(_subTopic.c_str(), 2);
    }
  }

  void onMqttMessage(char* topic,
                     char* payload,
                     AsyncMqttClientMessageProperties properties,
                     size_t len,
                     size_t index,
                     size_t total) {
    // we only care about the topic we are watching in this class
    if (strcmp(_subTopic.c_str(), topic)) {
      return;
    }

    // deserialize from string
    DynamicJsonDocument json(MAX_MESSAGE_SIZE);
    DeserializationError error = deserializeJson(json, payload, len);
    if (!error && json.is<JsonObject>()) {
      JsonObject jsonObject = json.as<JsonObject>();
      MqttConnector<T>::_settingsService->update(jsonObject, _jsonDeserializer, MQTT_ORIGIN_ID);
    }
  }
};

template <class T>
class MqttPubSub : public MqttPub<T>, public MqttSub<T> {
 public:
  MqttPubSub(JsonSerializer<T> jsonSerializer,
             JsonDeserializer<T> jsonDeserializer,
             SettingsService<T>* settingsService,
             AsyncMqttClient* mqttClient,
             String pubTopic = "",
             String subTopic = "") :
      MqttConnector<T>(settingsService, mqttClient),
      MqttPub<T>(jsonSerializer, settingsService, mqttClient, pubTopic = ""),
      MqttSub<T>(jsonDeserializer, settingsService, mqttClient, subTopic = "") {
  }

 public:
  void configureTopics(String pubTopic, String subTopic) {
    MqttSub<T>::setSubTopic(subTopic);
    MqttPub<T>::setPubTopic(pubTopic);
  }

 protected:
  void onConnect() {
    MqttSub<T>::onConnect();
    MqttPub<T>::onConnect();
  }
};

#endif  // end MqttPubSub
