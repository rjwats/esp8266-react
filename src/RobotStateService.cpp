#include <RobotStateService.h>

RobotStateService::RobotStateService(AsyncWebServer* server, SecurityManager* securityManager) :
    _httpEndpoint(RobotState::read,
                  RobotState::update,
                  this,
                  server,
                  ROBOT_SETTINGS_ENDPOINT_PATH,
                  securityManager,
                  AuthenticationPredicates::IS_AUTHENTICATED),
    _webSocket(RobotState::read,
               RobotState::update,
               this,
               server,
               ROBOT_SETTINGS_SOCKET_PATH,
               securityManager,
               AuthenticationPredicates::IS_AUTHENTICATED) {
  addUpdateHandler([&](const String& originId) { onConfigUpdated(); }, false);
}

void RobotStateService::begin() {
  camXServo.attach(CAM_X_SERVO_PIN);
  camYServo.attach(CAM_Y_SERVO_PIN);
  clawServo.attach(CLAW_SERVO_PIN);

  // configure the default state
  _state.camX = DEFAULT_CAM_X;
  _state.camY = DEFAULT_CAM_Y;
  _state.claw = DEFAULT_CLAW;

  // apply the default state
  onConfigUpdated();
}

void RobotStateService::onConfigUpdated() {
  camXServo.write(_state.camX);
  camYServo.write(_state.camY);
  clawServo.write(_state.claw);
}
