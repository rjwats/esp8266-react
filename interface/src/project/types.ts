
type SensorTemperatures = { [sensor_address: string]: number; }

export interface ChamberStatus {
  // configurable addresses for sensors
  chamber_sensor_address: string;
  ambient_sensor_address: string;

  // target temp
  target_temp: number;

  // flags for enabling/disabling the device
  enable_heater: boolean;
  enable_cooler: boolean;

  // threshold tempratures
  heater_on_temp: number;
  cooler_on_temp: number;
  heater_off_temp: number;
  cooler_off_temp: number;

  // status
  status: number;
  sensors: SensorTemperatures
}

export interface ChamberSettings {
  // configurable addresses for sensors
  chamber_sensor_address: string;
  ambient_sensor_address: string;

  // target temps and hysteresis thresholds
  target_temp: number;
  hysteresis_high: number;
  hysteresis_low: number;
  hysteresis_factor: number;

  // cycle limits
  min_heater_on_duration: number;
  min_heater_off_duration: number;
  min_cooler_on_duration: number;
  min_cooler_off_duration: number;

  // flags for enabling/disabling the device
  enable_heater: boolean;
  enable_cooler: boolean;
}
