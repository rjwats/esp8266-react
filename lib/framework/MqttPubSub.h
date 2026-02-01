#ifndef MqttPubSub_h
#define MqttPubSub_h

#include <StatefulService.h>
#include <espMqttClient.h>

#define MQTT_ORIGIN_ID "mqtt"

template <class T>
class MqttConnector {
 protected:
  StatefulService<T>* _statefulService;
  espMqttClient* _mqttClient;

  MqttConnector(StatefulService<T>* statefulService, espMqttClient* mqttClient) :
      _statefulService(statefulService), _mqttClient(mqttClient) {
    _mqttClient->onConnect(std::bind(&MqttConnector::onConnect, this));
  }

  virtual void onConnect() = 0;

 public:
  inline espMqttClient* getMqttClient() const {
    return _mqttClient;
  }
};

template <class T>
class MqttPub : virtual public MqttConnector<T> {
 public:
  MqttPub(JsonStateReader<T> stateReader,
          StatefulService<T>* statefulService,
          espMqttClient* mqttClient,
          const String& pubTopic = "",
          bool retain = false) :
      MqttConnector<T>(statefulService, mqttClient), _stateReader(stateReader), _pubTopic(pubTopic), _retain(retain) {
    MqttConnector<T>::_statefulService->addUpdateHandler([&](const String& originId) { publish(); }, false);
  }

  void setRetain(const bool retain) {
    _retain = retain;
    publish();
  }

  void setPubTopic(const String& pubTopic) {
    _pubTopic = pubTopic;
    publish();
  }

 protected:
  virtual void onConnect() {
    publish();
  }

 private:
  JsonStateReader<T> _stateReader;
  String _pubTopic;
  bool _retain;

  void publish() {
    if (_pubTopic.length() > 0 && MqttConnector<T>::_mqttClient->connected()) {
      // serialize to json doc
      JsonDocument json;
      JsonObject jsonObject = json.to<JsonObject>();
      MqttConnector<T>::_statefulService->read(jsonObject, _stateReader);

      // serialize to string
      String payload;
      serializeJson(json, payload);

      // publish the payload
      MqttConnector<T>::_mqttClient->publish(_pubTopic.c_str(), 0, _retain, payload.c_str());
    }
  }
};

template <class T>
class MqttSub : virtual public MqttConnector<T> {
 public:
  MqttSub(JsonStateUpdater<T> stateUpdater,
          StatefulService<T>* statefulService,
          espMqttClient* mqttClient,
          const String& subTopic = "") :
      MqttConnector<T>(statefulService, mqttClient), _stateUpdater(stateUpdater), _subTopic(subTopic) {
    MqttConnector<T>::_mqttClient->onMessage(
        [this](const espMqttClientTypes::MessageProperties& properties,
               const char* topic,
               const uint8_t* payload,
               size_t len,
               size_t index,
               size_t total) { this->onMqttMessage(properties, topic, payload, len, index, total); });
  }

  void setSubTopic(const String& subTopic) {
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
  JsonStateUpdater<T> _stateUpdater;
  String _subTopic;

  void subscribe() {
    if (_subTopic.length() > 0) {
      MqttConnector<T>::_mqttClient->subscribe(_subTopic.c_str(), 2);
    }
  }

  void onMqttMessage(const espMqttClientTypes::MessageProperties& properties,
                     const char* topic,
                     const uint8_t* payload,
                     size_t len,
                     size_t index,
                     size_t total) {
    // we only care about the topic we are watching in this class
    if (strcmp(_subTopic.c_str(), topic) != 0) {
      return;
    }

    // deserialize from
    JsonDocument json;
    DeserializationError error = deserializeJson(json, payload, len);
    if (!error && json.is<JsonObject>()) {
      JsonObject jsonObject = json.as<JsonObject>();
      MqttConnector<T>::_statefulService->update(jsonObject, _stateUpdater, MQTT_ORIGIN_ID);
    }
  }
};

template <class T>
class MqttPubSub : public MqttPub<T>, public MqttSub<T> {
 public:
  MqttPubSub(JsonStateReader<T> stateReader,
             JsonStateUpdater<T> stateUpdater,
             StatefulService<T>* statefulService,
             espMqttClient* mqttClient,
             const String& pubTopic = "",
             const String& subTopic = "",
             bool retain = false) :
      MqttConnector<T>(statefulService, mqttClient),
      MqttPub<T>(stateReader, statefulService, mqttClient, pubTopic, retain),
      MqttSub<T>(stateUpdater, statefulService, mqttClient, subTopic) {
  }

 public:
  void configureTopics(const String& pubTopic, const String& subTopic) {
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
