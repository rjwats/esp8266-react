#ifndef ChamberSettingsService_h
#define ChamberSettingsService_h

#include <SettingsService.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <fermentation-chamber/JsonUtil.h>

#define TEMP_SENSOR_PIN 14
#define TEMP_SENSOR_INTERVAL 5000

#define CHAMBER_SETTINGS_FILE "/config/chamberSettings.json"
#define CHAMBER_SETTINGS_SERVICE_PATH "/rest/chamberSettings"

#define CHAMBER_STATUS_SERVICE_PATH "/rest/chamberStatus"

#define STATUS_IDLE 0
#define STATUS_HEATING 1
#define STATUS_COOLING 2

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

    // configurable addresses for sensors
    DeviceAddress _chamberSensorAddress;
    DeviceAddress _ambientSensorAddress;

    // target temps and hysteresis thresholds
    float _targetTemp;
    float _hysteresisHigh;
    float _hysteresisLow;

    // cycle limits
    unsigned long _minHeaterOnDuration;
    unsigned long _minHeaterOffDuration;

    unsigned long _minCoolerOnDuration;
    unsigned long _minCoolerOffDuration;

    // flags for enabling/disabling the device
    bool _enableHeater;
    bool _enableCooler;

    // status variables
    uint8_t _chamberStatus = STATUS_IDLE;
    unsigned long _heaterToggledAt;
    unsigned long _coolerToggledAt;
    unsigned long _nextEvaluation;

    void chamberStatus(AsyncWebServerRequest *request);
    void prepareNextControllerLoop();
    void changeStatus(uint8_t newStatus, unsigned long *previousToggle, unsigned long *toggleLimitDuration);
    void configureController();

};

#endif // end ChamberSettingsService_h
