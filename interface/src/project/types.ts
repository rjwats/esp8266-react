export interface LightSettings {
  led_on: boolean;
}

export interface LightBrokerSettings { 
  unique_id : string;
  name: string;
  mqtt_path : string;
}
