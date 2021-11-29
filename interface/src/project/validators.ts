import Schema from "async-validator";

export const LIGHT_MQTT_SETTINGS_VALIDATOR = new Schema({
    unique_id: {
        required: true, message: "Please provide an id"
    },
    name: {
        required: true, message: "Please provide a name"
    },
    mqtt_path: {
        required: true, message: "Please provide an MQTT path"
    }
});
