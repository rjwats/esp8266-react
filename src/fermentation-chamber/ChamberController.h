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

    OneWire _ds = OneWire(TEMP_SENSOR_PIN);
    DallasTemperature _tempSensors = DallasTemperature(&_ds);

    unsigned long _nextEvaluation;

    void sensorStatus(AsyncWebServerRequest *request);
    void prepareNextControllerLoop();
    void configureController();

};

#endif // end ChamberController_h
