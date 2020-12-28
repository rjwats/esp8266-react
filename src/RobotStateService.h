#ifndef RobotStateService_h
#define RobotStateService_h

#include <HttpEndpoint.h>
#include <WebSocketTxRx.h>
#include <Arduino.h>
#include <Servo.h>

#define LED_PIN 2

#define DEFAULT_CAM_Y 135
#define DEFAULT_CAM_X 90
#define DEFAULT_CLAW 90
#define DEFAULT_DRIVE_X 0
#define DEFAULT_DRIVE_Y 0

#define CAM_X_SERVO_PIN 13
#define CAM_X_SERVO_CHANNEL 10

#define CAM_Y_SERVO_PIN 15
#define CAM_Y_SERVO_CHANNEL 11

#define CLAW_SERVO_PIN 12
#define CLAW_SERVO_CHANNEL 12

#define LEFT_MOTOR_PWM_PIN 2
#define LEFT_MOTOR_DIRECTION_PIN 14
#define LEFT_MOTOR_PWM_CHANNEL 5

#define RIGHT_MOTOR_PWM_PIN 4
#define RIGHT_MOTOR_DIRECTION_PIN 16
#define RIGHT_MOTOR_PWM_CHANNEL 6

#define MOTOR_PWM_RESOLUTION 10
#define MOTOR_PWM_FREQUENCY 4096

#define DEAD_ZONE 20

#define ROBOT_SETTINGS_ENDPOINT_PATH "/rest/robotState"
#define ROBOT_SETTINGS_SOCKET_PATH "/ws/robotState"

struct MotorOutputs {
  int32_t left;
  int32_t right;
};

class RobotState {
 public:
  uint8_t camX;
  uint8_t camY;
  uint8_t claw;
  int32_t driveX;
  int32_t driveY;

  static void read(RobotState& settings, JsonObject& root) {
    root["cam_x"] = settings.camX;
    root["cam_y"] = settings.camY;
    root["claw"] = settings.claw;
    root["drive_x"] = settings.driveX;
    root["drive_y"] = settings.driveY;
  }

  static StateUpdateResult update(JsonObject& root, RobotState& robotState) {
    robotState.camX = readBounded(root, "cam_x", 0, 180, DEFAULT_CAM_X);
    robotState.camY = readBounded(root, "cam_y", 0, 180, DEFAULT_CAM_Y);
    robotState.claw = readBounded(root, "claw", 90, 180, DEFAULT_CLAW);
    robotState.driveX = readBounded(root, "drive_x", -1023, 1023, DEFAULT_DRIVE_X);
    robotState.driveY = readBounded(root, "drive_y", -1023, 1023, DEFAULT_DRIVE_Y);
    return StateUpdateResult::CHANGED;
  }

  template <typename N>
  static N readBounded(JsonObject& root, String prop, N minValue, N maxValue, N defaultValue) {
    N value = root[prop] | defaultValue;
    return max(min(value, maxValue), minValue);
  }
};

class RobotStateService : public StatefulService<RobotState> {
 public:
  RobotStateService(AsyncWebServer* server, SecurityManager* securityManager);
  void begin();

 private:
  HttpEndpoint<RobotState> _httpEndpoint;
  WebSocketTxRx<RobotState> _webSocket;
  Servo camXServo;
  Servo camYServo;
  Servo clawServo;

  void onConfigUpdated();
  struct MotorOutputs calculateMotorOutputs(int32_t nJoyX, int32_t nJoyY);
  void applyMotorOutput(int32_t output, uint8_t directionPin, uint8_t pwmChan);
};

#endif
