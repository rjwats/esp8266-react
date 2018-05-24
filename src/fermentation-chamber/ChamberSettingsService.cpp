#include <fermentation-chamber/ChamberSettingsService.h>

ChamberSettingsService::ChamberSettingsService(AsyncWebServer* server, FS* fs) :
  SettingsService(server, fs, CHAMBER_SETTINGS_SERVICE_PATH, CHAMBER_SETTINGS_FILE), _circularLog("/logs/hourly.dat", LOG_SLOTS) {
    server->on(CHAMBER_STATUS_SERVICE_PATH, HTTP_GET, std::bind(&ChamberSettingsService::chamberStatus, this, std::placeholders::_1));
    server->on(LOG_DATA_SERVICE_PATH, HTTP_GET, std::bind(&ChamberSettingsService::logData, this, std::placeholders::_1));
  }

ChamberSettingsService::~ChamberSettingsService() {}

void ChamberSettingsService::begin() {
  // load settings
  SettingsService::begin();

  // start the circular log, initializing the log file if required.
  _circularLog.begin();

  // set both pins to output
  pinMode(COOLER_PIN, OUTPUT);
  pinMode(HEATER_PIN, OUTPUT);

  // configure to idle mode
  transitionToStatus(STATUS_IDLE);

  Serial.print("Starting temp sensor bus...");
  _tempSensors.begin();
  _tempSensors.setResolution(12);
  _tempSensors.setWaitForConversion(false);
  Serial.println("finished!");

  Serial.print("Found ");
  Serial.print(_tempSensors.getDS18Count());
  Serial.println(" sensor(s).");

  // pre-calculate threshold temps
  onConfigUpdated();

  // request temps
  prepareNextControllerLoop();
}

void ChamberSettingsService::prepareNextControllerLoop() {
  _tempSensors.requestTemperatures();
  _evaluatedAt = millis();
}

void ChamberSettingsService::changeStatus(uint8_t newStatus, unsigned long *previousToggle, unsigned long *toggleLimitDuration) {
  unsigned long currentMillis = millis();
  unsigned long toggleElapsed = (unsigned long)(currentMillis - *previousToggle);
  if (toggleElapsed >= *toggleLimitDuration){
    *previousToggle = currentMillis;
    transitionToStatus(newStatus);
  }
}

void ChamberSettingsService::transitionToStatus(uint8_t newStatus) {
  _status = newStatus;
  switch (_status) {
    case STATUS_HEATING:
      digitalWrite(COOLER_PIN, LOW);
      digitalWrite(HEATER_PIN, HIGH);
      break;
    case STATUS_COOLING:
      digitalWrite(COOLER_PIN, HIGH);
      digitalWrite(HEATER_PIN, LOW);
      break;
    case STATUS_IDLE:
    default:
      digitalWrite(COOLER_PIN, LOW);
      digitalWrite(HEATER_PIN, LOW);
      break;
  }
}

void ChamberSettingsService::loop() {
  evaluateChamberStatus();
  performLogging();
}

void ChamberSettingsService::evaluateChamberStatus() {
  if ((unsigned long)(millis() - _evaluatedAt) < EVALUATION_INTERVAL){
    return;
  }

  float temp = _tempSensors.getTempC(_chamberSensorAddress);
  Serial.print("Temp is currently:");
  Serial.println(temp);
  Serial.print("Status is currently:");
  Serial.println(_status);

  switch (_status) {
    case STATUS_HEATING:
      if (!_enableHeater || temp == DEVICE_DISCONNECTED_C || temp >= _heaterOffTemp){
        changeStatus(STATUS_IDLE, &_heaterToggledAt, &_minHeaterOnDuration);
      }
      break;
    case STATUS_COOLING:
      if (!_enableCooler || temp == DEVICE_DISCONNECTED_C || temp <= _coolerOffTemp){
        changeStatus(STATUS_IDLE, &_coolerToggledAt, &_minCoolerOnDuration);
      }
      break;
    case STATUS_IDLE:
    default:
      if (temp != DEVICE_DISCONNECTED_C){
        if (_enableHeater && temp <= _heaterOnTemp){
          changeStatus(STATUS_HEATING, &_heaterToggledAt, &_minHeaterOffDuration);
        }
        if (_enableCooler && temp >= _coolerOnTemp){
          changeStatus(STATUS_COOLING, &_coolerToggledAt, &_minCoolerOffDuration);
        }
      }
  }
  prepareNextControllerLoop();
}

void ChamberSettingsService::onConfigUpdated() {
  _heaterOffTemp = _targetTemp - (_hysteresisLow * _hysteresisFactor);
  _coolerOffTemp = _targetTemp + (_hysteresisHigh * _hysteresisFactor);
  _heaterOnTemp = _targetTemp - _hysteresisLow;
  _coolerOnTemp = _targetTemp + _hysteresisHigh;
}

void ChamberSettingsService::readFromJsonObject(JsonObject& root) {
   deviceAddressFromString(root["chamber_sensor_address"], _chamberSensorAddress);
   deviceAddressFromString(root["ambient_sensor_address"], _ambientSensorAddress);

  _targetTemp = root["target_temp"];
  _hysteresisHigh = root["hysteresis_high"];
  _hysteresisLow = root["hysteresis_low"];
  _hysteresisFactor = root["hysteresis_factor"];

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
  root["hysteresis_factor"] = _hysteresisFactor;

  root["min_heater_on_duration"] = _minHeaterOnDuration;
  root["min_heater_off_duration"] = _minHeaterOffDuration;
  root["min_cooler_on_duration"] = _minCoolerOnDuration;
  root["min_cooler_off_duration"] = _minCoolerOffDuration;

  root["enable_heater"] = _enableHeater;
  root["enable_cooler"] = _enableCooler;
}

/**
* Runs as part of the main loop, logging when required.
*
* We don't worry about missed logs, we just ignore out of date data when we
* read the log data out.
*/
void ChamberSettingsService::performLogging() {
  // Don't do any logging until we have synchronized with the time server
  if (!NTP.getLastNTPSync()){
    return;
  }

  // If we have never logged, or if enough time has elapsed since we last logged
  unsigned long loggingTime = now();
  if (!_loggedAt || (unsigned long)(loggingTime - _loggedAt) >= LOG_PERIOD_SECONDS) {
    // round down to nearest LOG_PERIOD_SECONDS
    uint16_t loggingSlot = (loggingTime / LOG_PERIOD_SECONDS) % LOG_SLOTS;
    loggingTime = loggingTime - (loggingTime % LOG_PERIOD_SECONDS);

    ChamberLogEntry entry;
    entry.time = loggingTime;
    entry.status = _status;
    entry.ambientTemp = _tempSensors.getTempC(_ambientSensorAddress);
    entry.chamberTemp = _tempSensors.getTempC(_chamberSensorAddress);
    entry.targetTemp = _targetTemp;
    _circularLog.writeEntry(&entry, loggingSlot);

    _loggedAt = loggingTime;
  }
}

/**
* Returns pages of logging data. Starting from now back in time. Supports the following parameters:
*
* "pageSize" - Number of hours of log data to return, one page is one hour. (1 min (default), 6 max)
* "" - Number of hours of log data to return, one page is one hour. (0 min (default), 24 max)
*/
void ChamberSettingsService::logData(AsyncWebServerRequest *request) {
  uint16_t loggingSlot = ((_loggedAt + LOG_PERIOD_SECONDS) / LOG_PERIOD_SECONDS) % LOG_SLOTS;
  unsigned long oldestExpectedTime = _loggedAt - (LOG_PERIOD_SECONDS * (LOG_SLOTS  - 1));
  AsyncJsonResponse * response = new AsyncJsonResponse();
  JsonObject& root = response->getRoot();
  JsonArray& data = root.createNestedArray("data");

  _circularLog.readAllEntries(loggingSlot, [&](ChamberLogEntry *entry) {
    if (entry->time >= oldestExpectedTime){
      JsonObject& entryData = data.createNestedObject();
      entryData["time"] = entry->time;
      entryData["status"] = entry->status;
      entryData["chamber_temp"] = entry->chamberTemp;
      entryData["ambient_temp"] = entry->ambientTemp;
      entryData["target_temp"] = entry->targetTemp;
    }
  });

  // send response
  response->setLength();
  request->send(response);
}


void ChamberSettingsService::chamberStatus(AsyncWebServerRequest *request) {
  AsyncJsonResponse * response = new AsyncJsonResponse();
  JsonObject& root = response->getRoot();

  // basic config info - for display on status page
  root["chamber_sensor_address"] = deviceAddressAsString(_chamberSensorAddress);
  root["ambient_sensor_address"] = deviceAddressAsString(_ambientSensorAddress);
  root["target_temp"] = _targetTemp;
  root["enable_heater"] = _enableHeater;
  root["enable_cooler"] = _enableCooler;

  // temp thresholds
  root["heater_on_temp"] = _heaterOnTemp;
  root["cooler_on_temp"] = _coolerOnTemp;
  root["heater_off_temp"] = _heaterOffTemp;
  root["cooler_off_temp"] = _coolerOffTemp;

  // current status
  root["status"] = _status;

  // write out sensors and current readings
  JsonObject& sensors = root.createNestedObject("sensors");
  DeviceAddress address;
  for (uint8_t i = 0; i < _tempSensors.getDS18Count(); i++) {
    if (_tempSensors.getAddress(address, i)) {
      JsonObject& sensorDetails = sensors.createNestedObject(deviceAddressAsString(address));
      sensorDetails["temp_c"] =  _tempSensors.getTempC(address);
    }
  }

  // send response
  response->setLength();
  request->send(response);
}
