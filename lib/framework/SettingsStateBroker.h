#ifndef SettingsStateBroker_h
#define SettingsStateBroker_h

#include <SettingsService.h>
#include <SettingsSerializer.h>
#include <AsyncMqttClient.h>

#define MAX_MESSAGE_SIZE 1024

template <class T>
class SettingsStateBroker {
 public:
  SettingsStateBroker(SettingsSerializer<T>* settingsSerializer,
                      SettingsService<T>* settingsService,
                      AsyncMqttClient* mqttClient,
                      String topic = "") :
      _settingsSerializer(settingsSerializer),
      _settingsService(settingsService),
      _mqttClient(mqttClient),
      _topic(topic) {
    _settingsService->addUpdateHandler([&](String originId) { publish(); }, false);
    _mqttClient->onConnect(std::bind(&SettingsStateBroker::publish, this));
  }

  void setTopic(String topic) {
    _topic = topic;
    publish();
  }

 private:
  SettingsSerializer<T>* _settingsSerializer;
  SettingsService<T>* _settingsService;
  AsyncMqttClient* _mqttClient;
  String _topic;

  void publish() {
    if (_topic.length() > 0 && _mqttClient->connected()) {
      // serialize to json doc
      DynamicJsonDocument json(MAX_MESSAGE_SIZE);
      _settingsService->read([&](T& settings) { _settingsSerializer->serialize(settings, json.to<JsonObject>()); });

      // serialize to string
      String payload;
      serializeJson(json, payload);

      // publish the payload
      _mqttClient->publish(_topic.c_str(), 0, false, payload.c_str());
    }
  }
};

#endif  // end SettingsStateBroker_h
