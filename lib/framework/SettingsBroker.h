#ifndef SettingsBroker_h
#define SettingsBroker_h

#include <SettingsService.h>
#include <SettingsSerializer.h>
#include <SettingsDeserializer.h>
#include <AsyncMqttClient.h>

#define MAX_MESSAGE_SIZE 1024
#define SETTINGS_BROKER_ORIGIN_ID "broker"

/**
 * SettingsBroker is designed to operate with Home Assistant and takes care of observing state change requests over MQTT
 * and acting on them.
 *
 * The broker listens to changes on a "set" topic and publish it's state on a "state" topic. It also has
 * an optional config topic which can be used for home assistant's auto discovery feature if required.
 *
 * Settings are automatically published to the state topic when a connection to the broker is established or when
 * settings are updated.
 *
 * When a message is recieved on the set topic the settings are deserialized from the payload and applied. The state
 * topic is then updated as normal.
 */
template <class T>
class SettingsBroker {
 public:
  SettingsBroker(SettingsSerializer<T>* settingsSerializer,
                 SettingsDeserializer<T>* settingsDeserializer,
                 SettingsService<T>* settingsService,
                 AsyncMqttClient* mqttClient,
                 String setTopic = "",
                 String stateTopic = "") :
      _settingsSerializer(settingsSerializer),
      _settingsDeserializer(settingsDeserializer),
      _settingsService(settingsService),
      _mqttClient(mqttClient),
      _setTopic(setTopic),
      _stateTopic(stateTopic) {
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

  void configureBroker(String setTopic, String stateTopic) {
    _setTopic = setTopic;
    _stateTopic = stateTopic;
    configureMQTT();
  }

 protected:
  virtual void configureMQTT() {
    if (_setTopic.length() > 0) {
      _mqttClient->subscribe(_setTopic.c_str(), 2);
    }
    publish();
  }

 private:
  SettingsSerializer<T>* _settingsSerializer;
  SettingsDeserializer<T>* _settingsDeserializer;
  SettingsService<T>* _settingsService;
  AsyncMqttClient* _mqttClient;
  String _setTopic;
  String _stateTopic;

  void publish() {
    if (_stateTopic.length() > 0 && _mqttClient->connected()) {
      // serialize to json doc
      DynamicJsonDocument json(MAX_MESSAGE_SIZE);
      _settingsService->read([&](T& settings) { _settingsSerializer->serialize(settings, json.to<JsonObject>()); });

      // serialize to string
      String payload;
      serializeJson(json, payload);

      // publish the payload
      _mqttClient->publish(_stateTopic.c_str(), 0, false, payload.c_str());
    }
  }

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

#endif  // end SettingsBroker_h
