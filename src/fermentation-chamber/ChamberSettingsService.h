#ifndef ChamberSettingsService_h
#define ChamberSettingsService_h

#include <SettingsService.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <TimeLib.h>
#include <NtpClientLib.h>
#include <fermentation-chamber/JsonUtil.h>
#include <fermentation-chamber/CircularLog.h>

#define COOLER_PIN 12
#define HEATER_PIN 13
#define TEMP_SENSOR_PIN 14

// We store 60 slots of 60 second intervals (one hour) worth of logging data
// in a circular buffer in SPIFFS. This results in very low wear on the flash chip
// only 24 writes per sector per day.
//
// We might extend this to store 24 hours worth of data if it is deemed useful.
// The interface would need to be able to page back/forth through the data if
// this were the case could by done by - (n hours) which would make a nice enough
// interface.
#define LOG_SLOTS 60
#define LOG_PERIOD_SECONDS 60

// We evaluate the sensors and state every 5 seconds
#define EVALUATION_INTERVAL 5000

#define CHAMBER_SETTINGS_FILE "/config/chamberSettings.json"
#define CHAMBER_SETTINGS_SERVICE_PATH "/rest/chamberSettings"

#define CHAMBER_STATUS_SERVICE_PATH "/rest/chamberStatus"
#define LOG_DATA_SERVICE_PATH "/rest/logData"

#define STATUS_IDLE 0
#define STATUS_HEATING 1
#define STATUS_COOLING 2

struct ChamberLogEntry {
    unsigned long time;
    uint8_t status;
    float chamberTemp;
    float ambientTemp;
    float targetTemp;
};

class ChamberSettingsService : public SettingsService {

  public:

    ChamberSettingsService(AsyncWebServer* server, FS* fs);
    ~ChamberSettingsService();

    void begin();
    void loop();

  protected:

      void onConfigUpdated();
      void readFromJsonObject(JsonObject& root);
      void writeToJsonObject(JsonObject& root);

  private:
    // sensor objects
    OneWire _ds = OneWire(TEMP_SENSOR_PIN);
    DallasTemperature _tempSensors = DallasTemperature(&_ds);
    CircularLog<ChamberLogEntry> _circularLog;

    // configurable addresses for sensors
    DeviceAddress _chamberSensorAddress;
    DeviceAddress _ambientSensorAddress;

    // target temps and hysteresis thresholds
    float _targetTemp;
    float _hysteresisHigh;
    float _hysteresisLow;
    float _hysteresisFactor;

    // calculated off/on temps
    float _heaterOffTemp;
    float _coolerOffTemp;
    float _heaterOnTemp;
    float _coolerOnTemp;

    // cycle limits
    unsigned long _minHeaterOnDuration;
    unsigned long _minHeaterOffDuration;

    unsigned long _minCoolerOnDuration;
    unsigned long _minCoolerOffDuration;

    // flags for enabling/disabling the device
    bool _enableHeater;
    bool _enableCooler;

    // status variables
    uint8_t _status = STATUS_IDLE;
    unsigned long _heaterToggledAt;
    unsigned long _coolerToggledAt;
    unsigned long _evaluatedAt;
    unsigned long _loggedAt;

    void performLogging();
    void evaluateChamberStatus();
    void chamberStatus(AsyncWebServerRequest *request);
    void logData(AsyncWebServerRequest *request);
    void prepareNextControllerLoop();
    void changeStatus(uint8_t newStatus, unsigned long *previousToggle, unsigned long *toggleLimitDuration);
    void transitionToStatus(uint8_t newStatus);

};

#endif // end ChamberSettingsService_h
