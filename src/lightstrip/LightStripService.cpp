#include <lightstrip/LightStripService.h>

LightStripService::LightStripService(AsyncWebServer* server) :
  SimpleService(server, LIGHT_STRIP_SETTINGS_SERVICE_PATH),
  SimpleSocket(server, LIGHT_STRIP_SETTINGS_WEBSOCKET_PATH) {
}

LightStripService::~LightStripService() {}

void LightStripService::readFromJsonObject(JsonObject& root, String originId) {
  // update the config
  unsigned long modeCode = root["mode_code"];
  if (isModeCode(modeCode)){
    changeMode(modeCode);
  }
  currentMode->updateConfig(root);

  // push the updates out to the WebSockets
  pushPayloadToWebSockets(originId);
}

void LightStripService::writeToJsonObject(JsonObject& root) {
  root["mode_code"] = (unsigned long) currentMode->getModeCode();
  currentMode->writeConfig(root);
}

bool LightStripService::isModeCode(uint64_t code) {
  switch (code) {
    case IR_OFF:
    case IR_ON:
    case IR_SMOOTH:
    case IR_FADE:
    case IR_STROBE:
    case IR_FLASH:
      return true;
    default:
      return false;
  }
}

Mode* LightStripService::getModeForCode(uint64_t code) {
  switch (code) {
    case IR_OFF:
      return &offMode;
    case IR_ON:
      return &colorMode;
    case IR_SMOOTH:
      return &smoothMode;
    case IR_FADE:
      return &fadeMode;
    case IR_STROBE:
      return &strobeMode;
    case IR_FLASH:
      return &flashMode;
    default:
      return currentMode;
  }
}

void LightStripService::changeMode(uint64_t modeCode) {
  Mode* newMode = getModeForCode(modeCode);
  if (newMode != currentMode) {
    currentMode = newMode;
    currentMode->enable();
  }
}

void LightStripService::handleIRCode(uint64_t irCode, boolean repeat) {
  repeatTimeout = millis() + REPEAT_TIMEOUT_DURATION;
  previousIRCode = irCode;
  if (isModeCode(irCode)) {
    if (!repeat) {
      changeMode(irCode);
      pushPayloadToWebSockets("remote");
    }
  } else {
    currentMode->handleIrCode(irCode, repeat);
    pushPayloadToWebSockets("remote");
  }
}

void LightStripService::handleIRSensor() {
    if (irrecv.decode(&results)) {
      long now = millis();
      if (results.value == IR_REPEAT) {
        if (repeatTimeout > now){
          handleIRCode(previousIRCode, true);
        }
      } else {
        handleIRCode(results.value, false);
      }
      irrecv.resume();
    }
}

void LightStripService::loop() {
  handleIRSensor();
  currentMode->tick();
}

void LightStripService::begin() {
  Serial.println("Starting lightstrip service");
  irrecv.enableIRIn();
}
