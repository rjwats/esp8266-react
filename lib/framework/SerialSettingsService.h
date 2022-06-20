#ifndef SerialSettingsService_h
#define SerialSettingsService_h

#include <HttpEndpoint.h>
#include <FSPersistence.h>
#include <memory>
#include <string>
#include <vector>
#include <Stream.h>

#ifdef ESP32
#include <HardwareSerial.h>
#define HARDWARE_SERIAL_NUMBER 2
#elif defined(ESP8266)
#include <SoftwareSerial.h>
#endif

#include <StreamServer.h>

#define SERIAL_SETTINGS_FILE "/config/serialSettings.json"
#define SERIAL_SETTINGS_SERVICE_PATH "/rest/serialSettings"

class SerialSettings    {
    public:
    bool enabled;
    uint8_t rxPin;
    uint8_t txPin;
    uint32_t baud;
    uint32_t config;
    bool invert;
    uint16_t tCPPort;
    
    static void read(SerialSettings& settings, JsonObject& root) {
        root["enabled"] = settings.enabled;
        root["rxpin"] = settings.rxPin;
        root["txpin"] = settings.txPin;
        root["baud"] = settings.baud;
        root["config"] = settings.config;
        root["invert"] = settings.invert;
        root["port"] = settings.tCPPort;
    }

    static StateUpdateResult update(JsonObject& root, SerialSettings& settings) {
        settings.enabled = root["enabled"] | FACTORY_SERIAL_ENABLED;
        settings.rxPin = root["rxpin"] | FACTORY_SERIAL_RXPIN;
        settings.txPin = root["txpin"] | FACTORY_SERIAL_TXPIN;
        settings.baud = root["baud"] | FACTORY_SERIAL_BAUD;
        settings.config = root["config"] | FACTORY_SERIAL_CONFIG;
        settings.invert = root["invert"] | FACTORY_SERIAL_INVERTED;
        settings.tCPPort = root["port"] | FACTORY_TCP_PORT;
        serializeJsonPretty(root, Serial);
        return StateUpdateResult::CHANGED;
    }
};
class SerialSettingsService : public StatefulService<SerialSettings> {
public:
    SerialSettingsService(AsyncWebServer* server, FS* fs, SecurityManager* securityManager);

    void setup();
    void begin();
    void loop();
    void end();
    void dump_config() ;
    bool isEnabled() { return _state.enabled; };
    uint32_t baud() { return _state.baud; };
    uint32_t returnConfig() { return _state.config; };
    void onConfigUpdate() {  configureSerial(); } ;

private:
    HttpEndpoint<SerialSettings> httpEndpoint;
    FSPersistence<SerialSettings> fsPersistence;

    #ifdef ESP32
    HardwareSerial _serial{HARDWARE_SERIAL_NUMBER};
    #elif defined(ESP8266)
    SoftwareSerial _serial;
    #endif

    StreamServer _tcpServer{nullptr};

    void configureSerial();
};

#endif // end SerialSettingsService_h