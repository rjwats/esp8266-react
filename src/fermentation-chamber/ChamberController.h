#ifndef ChamberController_h
#define ChamberController_h

#include <SettingsService.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <fermentation-chamber/JsonUtil.h>

#define TEMP_SENSOR_PIN 14
#define TEMP_SENSOR_INTERVAL 5000

#define CHAMBER_CONTROLLER_SETTINGS_FILE "/config/chamberController.json"
#define CHAMBER_CONTROLLER_SETTINGS_SERVICE_PATH "/rest/chamberController"

#define SENSOR_STATUS_SERVICE_PATH "/rest/sensorStatus"

#define STATUS_IDLE 0
#define STATUS_HEATING 1
#define STATUS_COOLING 2

class ChamberController : public SettingsService {

  public:

    ChamberController(AsyncWebServer* server, FS* fs);
    ~ChamberController();

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
    unsigned long _heaterToggleLimit;
    unsigned long _coolerToggleLimit;
    unsigned long _nextEvaluation;

    void sensorStatus(AsyncWebServerRequest *request);
    void chamberStatus(AsyncWebServerRequest *request);
    void prepareNextControllerLoop();
    void configureController();

};

#endif // end ChamberController_h
