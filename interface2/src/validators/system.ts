import Schema from "async-validator";

export const OTA_SETTINGS_VALIDATOR = new Schema({
  port: {
    required: true, type: "number", min: 1025, max: 65535, message: "Please number between 1025 and 65535"
  },
  password: {
    required: true, min: 1, max: 64, message: "Please provide a password of up to 64 characters"
  }
});
