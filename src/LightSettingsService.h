#ifndef LightSettingsService_h
#define LightSettingsService_h

#include <LightBrokerSettingsService.h>
#include <SettingsEndpoint.h>
#include <SettingsBroker.h>
#include <SettingsSocket.h>
#include <ESP8266React.h>

#define BLINK_LED 2
#define PRINT_DELAY 5000

#define DEFAULT_LED_STATE false
#define OFF_STATE "OFF"
#define ON_STATE "ON"

// Note that the built-in LED is on when the pin is low on most NodeMCU boards.
// This is because the anode is tied to VCC and the cathode to the GPIO 4 (Arduino pin 2).
#ifdef ESP32
  #define LED_ON 0x1
  #define LED_OFF 0x0
#elif defined(ESP8266)
  #define LED_ON 0x0
  #define LED_OFF 0x1
#endif


#define LIGHT_SETTINGS_ENDPOINT_PATH "/rest/lightSettings"
#define LIGHT_SETTINGS_SOCKET_PATH "/ws/lightSettings"

class LightSettings {
 public:
  bool ledOn;
};

class LightSettingsSerializer : public SettingsSerializer<LightSettings> {
 public:
  void serialize(LightSettings& settings, JsonObject root) {
    root["led_on"] = settings.ledOn;
  }
};

class LightSettingsDeserializer : public SettingsDeserializer<LightSettings> {
 public:
  void deserialize(LightSettings& settings, JsonObject root) {
    settings.ledOn = root["led_on"] | DEFAULT_LED_STATE;
  }
};

class HomeAssistantSerializer : public SettingsSerializer<LightSettings> {
 public:
  void serialize(LightSettings& settings, JsonObject root) {
    root["state"] = settings.ledOn ? ON_STATE : OFF_STATE;
  }
};

class HomeAssistantDeserializer : public SettingsDeserializer<LightSettings> {
 public:
  void deserialize(LightSettings& settings, JsonObject root) {
    String state = root["state"];
    settings.ledOn = strcmp(ON_STATE, state.c_str()) ? false : true;
  }
};

class LightSettingsService : public SettingsService<LightSettings> {
 public:
  LightSettingsService(AsyncWebServer* server,
                       SecurityManager* securityManager,
                       AsyncMqttClient* mqttClient,
                       LightBrokerSettingsService* lightBrokerSettingsService);
  void begin();

 private:
  SettingsEndpoint<LightSettings> _settingsEndpoint;
  SettingsBroker<LightSettings> _settingsBroker;
  SettingsSocket<LightSettings> _settingsSocket;
  AsyncMqttClient* _mqttClient;
  LightBrokerSettingsService* _lightBrokerSettingsService;

  void registerConfig();
  void onConfigUpdated();
};

#endif
