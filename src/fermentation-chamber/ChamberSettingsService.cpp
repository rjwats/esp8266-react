#include <fermentation-chamber/ChamberSettingsService.h>

ChamberSettingsService::ChamberSettingsService(AsyncWebServer* server, FS* fs) :
  SettingsService(server, fs, CHAMBER_SETTINGS_SERVICE_PATH, CHAMBER_SETTINGS_FILE) {
    server->on(CHAMBER_STATUS_SERVICE_PATH, HTTP_GET, std::bind(&ChamberSettingsService::chamberStatus, this, std::placeholders::_1));
  }

ChamberSettingsService::~ChamberSettingsService() {}

void ChamberSettingsService::begin() {
  // load settings
  SettingsService::begin();

  Serial.print("Starting temp sensor bus...");
  _tempSensors.begin();
  _tempSensors.setWaitForConversion(false);
  Serial.println("finished!");

  Serial.print("Found ");
  Serial.print(_tempSensors.getDS18Count());
  Serial.print(" sensor(s).");

  // request temps
  prepareNextControllerLoop();
}

void ChamberSettingsService::prepareNextControllerLoop() {
  _tempSensors.requestTemperatures();
  _nextEvaluation = millis() + TEMP_SENSOR_INTERVAL;
}

void ChamberSettingsService::loop() {
  if (millis() < _nextEvaluation) {
    return;
  }

  switch (_chamberStatus) {
    case STATUS_HEATING:
     break;
    case STATUS_COOLING:
     break;
    case STATUS_IDLE:
    default:
     break;
  }

  prepareNextControllerLoop();
}

void ChamberSettingsService::onConfigUpdated() {

}

void ChamberSettingsService::configureController() {
  // set next evaluation to be immediately fired
  _nextEvaluation = millis();
}

void ChamberSettingsService::readFromJsonObject(JsonObject& root) {
   deviceAddressFromString(root["chamber_sensor_address"], _chamberSensorAddress);
   deviceAddressFromString(root["ambient_sensor_address"], _chamberSensorAddress);

  _targetTemp = root["target_temp"];
  _hysteresisHigh = root["hysteresis_high"];
  _hysteresisLow = root["hysteresis_low"];

  _minHeaterOnDuration = root["min_heater_on_duration"];
  _minHeaterOffDuration = root["min_heater_off_duration"];
  _minCoolerOnDuration = root["min_cooler_on_duration"];
  _minCoolerOffDuration = root["min_cooler_off_duration"];

  _enableHeater = root["enable_heater"];
  _enableCooler = root["enable_cooler"];
}

void ChamberSettingsService::writeToJsonObject(JsonObject& root) {
  root["chamber_sensor_address"] = deviceAddressAsString(_chamberSensorAddress);
  root["ambient_sensor_address"] = deviceAddressAsString(_ambientSensorAddress);

  root["target_temp"] = _targetTemp;
  root["hysteresis_high"] = _hysteresisHigh;
  root["hysteresis_low"] = _hysteresisLow;

  root["min_heater_on_duration"] = _minHeaterOnDuration;
  root["min_heater_off_duration"] = _minHeaterOffDuration;
  root["min_cooler_on_duration"] = _minCoolerOnDuration;
  root["min_cooler_off_duration"] = _minCoolerOffDuration;

  root["enable_heater"] = _enableHeater;
  root["enable_cooler"] = _enableCooler;
}

void ChamberSettingsService::chamberStatus(AsyncWebServerRequest *request) {
  AsyncJsonResponse * response = new AsyncJsonResponse();
  JsonObject& root = response->getRoot();
  DeviceAddress address;
  for (uint8_t i = 0; i < _tempSensors.getDS18Count(); i++) {
    if (_tempSensors.getAddress(address, i)) {
      JsonObject& sensorDetails = root.createNestedObject(deviceAddressAsString(address));
      sensorDetails["tempC"] =  _tempSensors.getTempC(address);
      sensorDetails["tempF"] =  _tempSensors.getTempF(address);
    }
  }
  response->setLength();
  request->send(response);
}
