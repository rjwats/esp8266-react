#ifndef Bme280Project_h
#define Bme280Project_h

#include <SimpleService.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define BME280_SEALEVELPRESSURE_HPA (1013.25)
#define BME280_READ_INTERVAL (1000.0)
#define BME280_SERVICE_PATH "/rest/bme280"

class Bme280Project : public SimpleService {

  public:

    Bme280Project(AsyncWebServer* server);
    ~Bme280Project();
    
    void loop();

  private:
    Adafruit_BME280 _bme;
    float _temperature;
    float _humidity;
    float _pressure;
    float _altitude;
    unsigned long _previousMillis;

  protected:

    void readFromJsonObject(JsonObject& root);
    void writeToJsonObject(JsonObject& root);
  
};

#endif
