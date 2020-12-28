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
  camXServo.attach(CAM_X_SERVO_PIN, CAM_X_SERVO_CHANNEL);
  camYServo.attach(CAM_Y_SERVO_PIN, CAM_Y_SERVO_CHANNEL);
  clawServo.attach(CLAW_SERVO_PIN, CLAW_SERVO_CHANNEL);

  // configure the default state
  _state.camX = DEFAULT_CAM_X;
  _state.camY = DEFAULT_CAM_Y;
  _state.claw = DEFAULT_CLAW;
  _state.driveX = DEFAULT_DRIVE_X;
  _state.driveY = DEFAULT_DRIVE_Y;

  // configure the PWM pins for both motors
  ledcSetup(LEFT_MOTOR_PWM_CHANNEL, MOTOR_PWM_FREQUENCY, MOTOR_PWM_RESOLUTION);
  ledcAttachPin(LEFT_MOTOR_PWM_PIN, LEFT_MOTOR_PWM_CHANNEL);
  ledcSetup(RIGHT_MOTOR_PWM_CHANNEL, MOTOR_PWM_FREQUENCY, MOTOR_PWM_RESOLUTION);
  ledcAttachPin(RIGHT_MOTOR_PWM_PIN, RIGHT_MOTOR_PWM_CHANNEL);

  // configure direction pins for both motors
  pinMode(LEFT_MOTOR_DIRECTION_PIN, OUTPUT);
  pinMode(RIGHT_MOTOR_DIRECTION_PIN, OUTPUT);

  // apply the default state
  onConfigUpdated();
}

void RobotStateService::onConfigUpdated() {
  camXServo.write(_state.camX);
  camYServo.write(_state.camY);
  clawServo.write(_state.claw);

  // calculate and convert motor outputs to PWM outputs for h-bridge
  struct MotorOutputs outputs = calculateMotorOutputs(_state.driveX, _state.driveY);
  applyMotorOutput(outputs.left, LEFT_MOTOR_DIRECTION_PIN, LEFT_MOTOR_PWM_CHANNEL);
  applyMotorOutput(outputs.right, RIGHT_MOTOR_DIRECTION_PIN, RIGHT_MOTOR_PWM_CHANNEL);
}

void RobotStateService::applyMotorOutput(int32_t output, uint8_t directionPin, uint8_t pwmChan) {
  bool forward = output >= 0;
  output = abs(output);
  output = output > DEAD_ZONE ? output : 0;
  if (output > 0) {
    output = map(output, 0, 1023, 600, 1023);
  }
  if (forward) {
    digitalWrite(directionPin, HIGH);
    ledcWrite(pwmChan, 1023 - output);
  } else {
    digitalWrite(directionPin, LOW);
    ledcWrite(pwmChan, output);
  }
}

// Wonderful code below borrowed from https://www.impulseadventure.com/elec/robot-differential-steering.html
struct MotorOutputs RobotStateService::calculateMotorOutputs(int32_t nJoyX, int32_t nJoyY) {
  // OUTPUTS
  int32_t nMotMixL;  // Motor (left)  mixed output           (-1023..+1023)
  int32_t nMotMixR;  // Motor (right) mixed output           (-1023..+1023)

  // CONFIG
  // - fPivYLimt  : The threshold at which the pivot action starts
  //                This threshold is measured in units on the Y-axis
  //                away from the X-axis (Y=0). A greater value will assign
  //                more of the joystick's range to pivot actions.
  //                Allowable range: (0..+1023)
  float fPivYLimit = 255.0;

  // TEMP VARIABLES
  float nMotPremixL;  // Motor (left)  premixed output        (-1023..+1023)
  float nMotPremixR;  // Motor (right) premixed output        (-1023..+1023)
  int32_t nPivSpeed;  // Pivot Speed                          (-1023..+1023)
  float fPivScale;    // Balance scale b/w drive and pivot    (   0..1   )

  // Calculate Drive Turn output due to Joystick X input
  if (nJoyY >= 0) {
    // Forward
    nMotPremixL = (nJoyX >= 0) ? 1023.0 : (1023.0 + nJoyX);
    nMotPremixR = (nJoyX >= 0) ? (1023.0 - nJoyX) : 1023.0;
  } else {
    // Reverse
    nMotPremixL = (nJoyX >= 0) ? (1023.0 - nJoyX) : 1023.0;
    nMotPremixR = (nJoyX >= 0) ? 1023.0 : (1023.0 + nJoyX);
  }

  // Scale Drive output due to Joystick Y input (throttle)
  nMotPremixL = nMotPremixL * nJoyY / 1023.0;
  nMotPremixR = nMotPremixR * nJoyY / 1023.0;

  // Now calculate pivot amount
  // - Strength of pivot (nPivSpeed) based on Joystick X input
  // - Blending of pivot vs drive (fPivScale) based on Joystick Y input
  nPivSpeed = nJoyX;
  fPivScale = (abs(nJoyY) > fPivYLimit) ? 0.0 : (1.0 - abs(nJoyY) / fPivYLimit);

  // Calculate final mix of Drive and Pivot
  nMotMixL = (1.0 - fPivScale) * nMotPremixL + fPivScale * (nPivSpeed);
  nMotMixR = (1.0 - fPivScale) * nMotPremixR + fPivScale * (-nPivSpeed);

  // return as a struct
  struct MotorOutputs outputs {
    nMotMixL, nMotMixR
  };
  return outputs;
}