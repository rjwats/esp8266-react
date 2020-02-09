import { APSettings } from "./types";

export const AP_MODE_ALWAYS = 0;
export const AP_MODE_DISCONNECTED = 1;
export const AP_NEVER = 2;

export const isAPEnabled = ({ provision_mode }: APSettings) => provision_mode === AP_MODE_ALWAYS || provision_mode === AP_MODE_DISCONNECTED;
