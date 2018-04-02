#include <fermentation-chamber/ChamberController.h>

ChamberController::ChamberController(AsyncWebServer* server, FS* fs) :
  SettingsService(server, fs, CHAMBER_CONTROLLER_SETTINGS_SERVICE_PATH, CHAMBER_CONTROLLER_SETTINGS_FILE) {
    server->on(SENSOR_STATUS_SERVICE_PATH, HTTP_GET, std::bind(&ChamberController::sensorStatus, this, std::placeholders::_1));
  }

ChamberController::~ChamberController() {}

void ChamberController::begin() {
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

void ChamberController::prepareNextControllerLoop() {
  _tempSensors.requestTemperatures();
  _nextEvaluation = millis() + TEMP_SENSOR_INTERVAL;
}

void ChamberController::loop() {
  if (_tempSensors.getDS18Count() == 0 || millis() < _nextEvaluation) {
    return;
  }
  prepareNextControllerLoop();
}

void ChamberController::sensorStatus(AsyncWebServerRequest *request) {
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

void ChamberController::onConfigUpdated() {

}

void ChamberController::configureController() {

}

void ChamberController::readFromJsonObject(JsonObject& root) {

}

void ChamberController::writeToJsonObject(JsonObject& root) {

}
