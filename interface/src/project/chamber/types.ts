import { ENDPOINT_ROOT } from "../../api";

export const CHAMBER_SETTINGS_ENDPOINT = ENDPOINT_ROOT + "chamberSettings";
export const CHAMBER_STATUS_ENDPOINT = ENDPOINT_ROOT + "chamberStatus";
export const LOG_DATA_ENDPOINT = ENDPOINT_ROOT + "logData";

export interface SensorTemperature {
  temp_c: number;
}

export type SensorTemperatures = { [sensor_address: string]: SensorTemperature; }

export interface ChamberDataSet {
  data: ChamberData[];
}

// time, status, chamber_temp, ambient_temp, target_temp
export type ChamberData = [number, number, number, number, number]; 

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
