#ifndef SerialSettingsService_h
#define SerialSettingsService_h

#include <HttpEndpoint.h>
#include <FSPersistence.h>
#include <memory>
#include <string>
#include <vector>
#include <Stream.h>
#include <PipedStream.h>

#include <StreamServer.h>
#include <SerialServer.h>

#define SERIAL_SETTINGS_FILE "/config/serialSettings.json"
#define SERIAL_SETTINGS_SERVICE_PATH "/rest/serialSettings"

class SerialSettings    {
    public:
    bool enabled{false};
    uint8_t rxPin{15};
    uint8_t txPin{17};
    unsigned long baud{};
    uint32_t config{SERIAL_8N1};
    bool invert{true};
    
    static void read(SerialSettings& settings, JsonObject& root) {
        root["enabled"] = settings.enabled;
        root["rxpin"] = settings.rxPin;
        root["txpin"] = settings.txPin;
        root["baud"] = settings.baud;
        root["config"] = settings.config;
        root["invert"] = settings.invert;
    }

    static StateUpdateResult update(JsonObject& root, SerialSettings& settings) {
        settings.enabled = root["enabled"] | FACTORY_SERIAL_ENABLED;
        settings.rxPin = root["rxpin"] | FACTORY_SERIAL_RXPIN;
        settings.txPin = root["txpin"] | FACTORY_SERIAL_TXPIN;
        settings.baud = root["baud"] | FACTORY_SERIAL_BAUD;
        settings.config = root["config"] | FACTORY_SERIAL_CONFIG;
        settings.invert = root["invert"] | FACTORY_SERIAL_INVERTED;
        return StateUpdateResult::CHANGED;
    }
};
class SerialSettingsService : public StatefulService<SerialSettings> {
public:
    SerialSettingsService(AsyncWebServer* server, FS* fs, SecurityManager* securityManager);

    void setup();
    void begin();
    void loop();
    void dump_config() ;
    bool isEnabled() { return this->_state.enabled; };
    void onConfigUpdate() {  configureSerial(); } ;

private:
    HttpEndpoint<SerialSettings> httpEndpoint;
    FSPersistence<SerialSettings> fsPersistence;
    SerialServer serialServer;
    StreamServer tcpServer{nullptr};

#ifdef ESP32
    HardwareSerial serial{2};
#elif defined(ESP8266)
    SoftwareSerial serial;
#endif

    void configureSerial();
};

#endif // end SerialSettingsService_h