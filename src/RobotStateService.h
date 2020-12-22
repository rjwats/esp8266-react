#ifndef RobotStateService_h
#define RobotStateService_h

#include <HttpEndpoint.h>
#include <WebSocketTxRx.h>
#include <Arduino.h>
#include <Servo.h>

#define LED_PIN 2

#define DEFAULT_CAM_Y 90
#define DEFAULT_CAM_X 135
#define DEFAULT_CLAW 90

#define CAM_X_SERVO_PIN 2
#define CAM_Y_SERVO_PIN 16
#define CLAW_SERVO_PIN 4

#define ROBOT_SETTINGS_ENDPOINT_PATH "/rest/robotState"
#define ROBOT_SETTINGS_SOCKET_PATH "/ws/robotState"

class RobotState {
 public:
  uint8_t camX;
  uint8_t camY;
  uint8_t claw;

  static void read(RobotState& settings, JsonObject& root) {
    root["cam_x"] = settings.camX;
    root["cam_y"] = settings.camY;
    root["claw"] = settings.claw;
  }

  static StateUpdateResult update(JsonObject& root, RobotState& robotState) {
    robotState.camX = readBoundedByte(root, "cam_x", 0, 180, DEFAULT_CAM_X);
    robotState.camY = readBoundedByte(root, "cam_y", 0, 180, DEFAULT_CAM_Y);
    robotState.claw = readBoundedByte(root, "claw", 90, 180, DEFAULT_CLAW);
    return StateUpdateResult::CHANGED;
  }

  static uint8_t readBoundedByte(JsonObject& root,
                                 String prop,
                                 uint8_t minValue,
                                 uint8_t maxValue,
                                 uint8_t defaultValue) {
    uint8_t value = root[prop] | defaultValue;
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
};

#endif
