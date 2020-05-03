#ifndef SettingsSetBroker_h
#define SettingsSetBroker_h

#include <SettingsService.h>
#include <SettingsDeserializer.h>
#include <AsyncMqttClient.h>

#define MAX_MESSAGE_SIZE 1024
#define SETTINGS_BROKER_ORIGIN_ID "broker"

template <class T>
class SettingsSetBroker {
 public:
  SettingsSetBroker(SettingsDeserializer<T>* settingsDeserializer,
                    SettingsService<T>* settingsService,
                    AsyncMqttClient* mqttClient,
                    String topic = "") :
      _settingsDeserializer(settingsDeserializer),
      _settingsService(settingsService),
      _mqttClient(mqttClient),
      _topic(topic) {
    _settingsService->addUpdateHandler([&](String originId) { publish(); }, false);
    _mqttClient->onConnect(std::bind(&SettingsBroker::configureMQTT, this));
    _mqttClient->onMessage(std::bind(&SettingsBroker::onMqttMessage,
                                     this,
                                     std::placeholders::_1,
                                     std::placeholders::_2,
                                     std::placeholders::_3,
                                     std::placeholders::_4,
                                     std::placeholders::_5,
                                     std::placeholders::_6));
  }

  void setTopic(String topic) {
    if (!_topic.equals(topic)) {
      // unsubscribe from the existing topic if one was set
      if (_topic.length() > 0) {
        _mqttClient->unsubscribe(_topic.c_str());
      }
      // set the new topic and re-configure the subscription
      _topic = topic;
      configureMQTT();
    }
  }

 protected:
  virtual void configureMQTT() {
    if (_topic.length() > 0) {
      _mqttClient->subscribe(_topic.c_str(), 2);
    }
  }

 private:
  SettingsDeserializer<T>* _settingsDeserializer;
  SettingsService<T>* _settingsService;
  AsyncMqttClient* _mqttClient;
  String _topic;

  void onMqttMessage(char* topic,
                     char* payload,
                     AsyncMqttClientMessageProperties properties,
                     size_t len,
                     size_t index,
                     size_t total) {
    // we only care about the topic we are watching in this class
    if (strcmp(_setTopic.c_str(), topic)) {
      return;
    }

    // deserialize from string
    DynamicJsonDocument json(MAX_MESSAGE_SIZE);
    DeserializationError error = deserializeJson(json, payload, len);
    if (!error && json.is<JsonObject>()) {
      _settingsService->update(
          [&](T& settings) { _settingsDeserializer->deserialize(settings, json.as<JsonObject>()); },
          SETTINGS_BROKER_ORIGIN_ID);
    }
  }
};

#endif  // end SettingsSetBroker_h
