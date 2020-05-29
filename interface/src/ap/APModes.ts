import { APSettings, ApProvisionMode } from "./types";

export const isAPEnabled = ({ provision_mode }: APSettings) => {
    return provision_mode === ApProvisionMode.AP_MODE_ALWAYS || provision_mode === ApProvisionMode.AP_MODE_DISCONNECTED;
}
