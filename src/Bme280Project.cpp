#include <Bme280Project.h>

Bme280Project::Bme280Project(AsyncWebServer *server) : SimpleService(server, BME280_SERVICE_PATH)
{
  _bme.begin(0x76);
}

Bme280Project::~Bme280Project() {}

void Bme280Project::loop()
{
  unsigned long currentMillis = millis();

  if (currentMillis - _previousMillis >= BME280_READ_INTERVAL)
  {
    _temperature = _bme.readTemperature();
    _humidity = _bme.readHumidity();
    _pressure = _bme.readPressure() / 100.0F;
    _altitude = _bme.readAltitude(BME280_SEALEVELPRESSURE_HPA);
  }
}

void Bme280Project::readFromJsonObject(JsonObject &root)
{
}

void Bme280Project::writeToJsonObject(JsonObject &root)
{
  // connection settings
  root["temperature"] = _temperature;
  root["humidity"] = _humidity;
  root["pressure"] = _pressure;
  root["altitude"] = _altitude;
}
