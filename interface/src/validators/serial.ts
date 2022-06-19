import Schema from 'async-validator';

//TODO Determine what esp we are dealing with
import { ESP32_PIN_VALIDATOR, TCP_PORT_VALIDATOR } from './shared';

export const SERIAL_SETTINGS_VALIDATOR = new Schema({
  rxpin: [
    { required: true, message: "Rx pin is required" },
    ESP32_PIN_VALIDATOR
  ],
  txpin: [
    { required: true, message: "Tx pin is required" },
    ESP32_PIN_VALIDATOR
  ],
  baud: [
    {required: true, message: "Baud rate is required"},
    { type: "number", min: 0, max: 115200, message: "Baud rate must be between 1 and 115200 (0 for automatic)" }
  ],
  config : [
    {required: true, message: "Config is required"},
    { type: "number", min: 134217744, max: 134217791, message: "Config must be between 134217744 and 134217791" }
  ],
  port: [
    { required: true, message: "Port is required" },
    TCP_PORT_VALIDATOR
  ],
});
