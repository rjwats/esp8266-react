import { Theme } from "@material-ui/core";

import { ChamberStatus } from "./types";

export const CHAMBER_STATUS_IDLE = 0;
export const CHAMBER_STATUS_HEATING = 1;
export const CHAMBER_STATUS_COOLING = 2;

export const chamberEnabledHighlight = (enabled: boolean, theme: Theme) => {
  return enabled ? theme.palette.success.main : "";
}

export const chamberStatusHighlight = ({ status }: ChamberStatus, theme: Theme) => {
  switch (status) {
    case CHAMBER_STATUS_HEATING:
      return theme.palette.error.main;
    case CHAMBER_STATUS_COOLING:
      return theme.palette.info.main;
    case CHAMBER_STATUS_IDLE:
    default:
      return theme.palette.warning.main;
  }
}

export const chamberStatus = ({ status }: ChamberStatus) => {
  switch (status) {
    case CHAMBER_STATUS_IDLE:
      return "Idle";
    case CHAMBER_STATUS_HEATING:
      return "Heating";
    case CHAMBER_STATUS_COOLING:
      return "Cooling";
    default:
      return "Unknown"
  }
}
