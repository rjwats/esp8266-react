#include <FermentationChamber.h>

FermentationChamber::FermentationChamber(AsyncWebServer* server, FS* fs) :
    SettingsService(server, fs, CHAMBER_SETTINGS_SERVICE_PATH, CHAMBER_SETTINGS_FILE),
    _circularLog("/logs/hourly.dat", LOG_SLOTS) {
  server->on(CHAMBER_STATUS_SERVICE_PATH,
             HTTP_GET,
             std::bind(&FermentationChamber::chamberStatus, this, std::placeholders::_1));
  server->on(LOG_DATA_SERVICE_PATH, HTTP_GET, std::bind(&FermentationChamber::logData, this, std::placeholders::_1));
}

FermentationChamber::~FermentationChamber() {
}

void FermentationChamber::begin() {
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

void FermentationChamber::prepareNextControllerLoop() {
  _tempSensors.requestTemperatures();
  _evaluatedAt = millis();
}

void FermentationChamber::changeStatus(uint8_t newStatus,
                                       unsigned long* previousToggle,
                                       unsigned long* toggleLimitDuration) {
  unsigned long currentMillis = millis();
  unsigned long toggleElapsed = (unsigned long)(currentMillis - *previousToggle);
  if (toggleElapsed >= *toggleLimitDuration) {
    *previousToggle = currentMillis;
    transitionToStatus(newStatus);
  }
}

void FermentationChamber::transitionToStatus(uint8_t newStatus) {
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

void FermentationChamber::loop() {
  evaluateChamberStatus();
  performLogging();
}

void FermentationChamber::evaluateChamberStatus() {
  if ((unsigned long)(millis() - _evaluatedAt) < EVALUATION_INTERVAL) {
    return;
  }

  float temp = _tempSensors.getTempC(_settings.chamberSensorAddress);
  Serial.print("Temp is currently:");
  Serial.println(temp);
  Serial.print("Status is currently:");
  Serial.println(_status);

  switch (_status) {
    case STATUS_HEATING:
      if (!_settings.enableHeater || temp == DEVICE_DISCONNECTED_C || temp >= _heaterOffTemp) {
        changeStatus(STATUS_IDLE, &_heaterToggledAt, &_settings.minHeaterOnDuration);
      }
      break;
    case STATUS_COOLING:
      if (!_settings.enableCooler || temp == DEVICE_DISCONNECTED_C || temp <= _coolerOffTemp) {
        changeStatus(STATUS_IDLE, &_coolerToggledAt, &_settings.minCoolerOnDuration);
      }
      break;
    case STATUS_IDLE:
    default:
      if (temp != DEVICE_DISCONNECTED_C) {
        if (_settings.enableHeater && temp <= _heaterOnTemp) {
          changeStatus(STATUS_HEATING, &_heaterToggledAt, &_settings.minHeaterOffDuration);
        }
        if (_settings.enableCooler && temp >= _coolerOnTemp) {
          changeStatus(STATUS_COOLING, &_coolerToggledAt, &_settings.minCoolerOffDuration);
        }
      }
  }
  prepareNextControllerLoop();
}

void FermentationChamber::onConfigUpdated() {
  _heaterOffTemp = _settings.targetTemp - (_settings.hysteresisLow * _settings.hysteresisFactor);
  _coolerOffTemp = _settings.targetTemp + (_settings.hysteresisHigh * _settings.hysteresisFactor);
  _heaterOnTemp = _settings.targetTemp - _settings.hysteresisLow;
  _coolerOnTemp = _settings.targetTemp + _settings.hysteresisHigh;
}

void FermentationChamber::readFromJsonObject(JsonObject& root) {
  deviceAddressFromString(root["chamber_sensor_address"], _settings.chamberSensorAddress);
  deviceAddressFromString(root["ambient_sensor_address"], _settings.ambientSensorAddress);

  _settings.targetTemp = root["target_temp"];
  _settings.hysteresisHigh = root["hysteresis_high"];
  _settings.hysteresisLow = root["hysteresis_low"];
  _settings.hysteresisFactor = root["hysteresis_factor"];

  _settings.minHeaterOnDuration = root["min_heater_on_duration"];
  _settings.minHeaterOffDuration = root["min_heater_off_duration"];
  _settings.minCoolerOnDuration = root["min_cooler_on_duration"];
  _settings.minCoolerOffDuration = root["min_cooler_off_duration"];

  _settings.enableHeater = root["enable_heater"];
  _settings.enableCooler = root["enable_cooler"];
}

void FermentationChamber::writeToJsonObject(JsonObject& root) {
  root["chamber_sensor_address"] = deviceAddressAsString(_settings.chamberSensorAddress);
  root["ambient_sensor_address"] = deviceAddressAsString(_settings.ambientSensorAddress);

  root["target_temp"] = _settings.targetTemp;
  root["hysteresis_high"] = _settings.hysteresisHigh;
  root["hysteresis_low"] = _settings.hysteresisLow;
  root["hysteresis_factor"] = _settings.hysteresisFactor;

  root["min_heater_on_duration"] = _settings.minHeaterOnDuration;
  root["min_heater_off_duration"] = _settings.minHeaterOffDuration;
  root["min_cooler_on_duration"] = _settings.minCoolerOnDuration;
  root["min_cooler_off_duration"] = _settings.minCoolerOffDuration;

  root["enable_heater"] = _settings.enableHeater;
  root["enable_cooler"] = _settings.enableCooler;
}

/**
 * Runs as part of the main loop, logging when required.
 *
 * We don't worry about missed logs, we just ignore out of date data when we
 * read the log data out.
 */
void FermentationChamber::performLogging() {
  // grab the current instant in unix seconds
  time_t loggingTime = time(nullptr);

  // Don't do any logging until we have synchronized with the time server
  if (loggingTime < YEAR_TWO_THOUSAND_UNIX_TIMESTAMP) {
    return;
  }

  // If we have never logged, or if enough time has elapsed since we last logged
  if (!_loggedAt || (unsigned long)(loggingTime - _loggedAt) >= LOG_PERIOD_SECONDS) {
    // round down to nearest LOG_PERIOD_SECONDS
    uint16_t loggingSlot = (loggingTime / LOG_PERIOD_SECONDS) % LOG_SLOTS;
    loggingTime = loggingTime - (loggingTime % LOG_PERIOD_SECONDS);

    ChamberLogEntry entry;
    entry.time = loggingTime;
    entry.status = _status;
    entry.ambientTemp = _tempSensors.getTempC(_settings.ambientSensorAddress);
    entry.chamberTemp = _tempSensors.getTempC(_settings.chamberSensorAddress);
    entry.targetTemp = _settings.targetTemp;
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
void FermentationChamber::logData(AsyncWebServerRequest* request) {
  uint16_t loggingSlot = ((_loggedAt + LOG_PERIOD_SECONDS) / LOG_PERIOD_SECONDS) % LOG_SLOTS;
  unsigned long oldestExpectedTime = _loggedAt - (LOG_PERIOD_SECONDS * (LOG_SLOTS - 1));
  AsyncJsonResponse* response = new AsyncJsonResponse(false);
  JsonObject root = response->getRoot();
  JsonArray data = root.createNestedArray("data");

  _circularLog.readAllEntries(loggingSlot, [&](ChamberLogEntry* entry) {
    if (entry->time >= oldestExpectedTime) {
      JsonObject entryData = data.createNestedObject();
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

void FermentationChamber::chamberStatus(AsyncWebServerRequest* request) {
  AsyncJsonResponse* response = new AsyncJsonResponse(false);
  JsonObject root = response->getRoot();

  // basic config info - for display on status page
  root["chamber_sensor_address"] = deviceAddressAsString(_settings.chamberSensorAddress);
  root["ambient_sensor_address"] = deviceAddressAsString(_settings.ambientSensorAddress);
  root["target_temp"] = _settings.targetTemp;
  root["enable_heater"] = _settings.enableHeater;
  root["enable_cooler"] = _settings.enableCooler;

  // temp thresholds
  root["heater_on_temp"] = _heaterOnTemp;
  root["cooler_on_temp"] = _coolerOnTemp;
  root["heater_off_temp"] = _heaterOffTemp;
  root["cooler_off_temp"] = _coolerOffTemp;

  // current status
  root["status"] = _status;

  // write out sensors and current readings
  JsonObject sensors = root.createNestedObject("sensors");
  DeviceAddress address;
  for (uint8_t i = 0; i < _tempSensors.getDS18Count(); i++) {
    if (_tempSensors.getAddress(address, i)) {
      JsonObject sensorDetails = sensors.createNestedObject(deviceAddressAsString(address));
      sensorDetails["temp_c"] = _tempSensors.getTempC(address);
    }
  }

  // send response
  response->setLength();
  request->send(response);
}
