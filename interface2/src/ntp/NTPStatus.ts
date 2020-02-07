import { Theme } from "@material-ui/core";
import { NTPStatus } from "./types";

export const NTP_INACTIVE = 0;
export const NTP_ACTIVE = 1;

export const isNtpActive = ({ status }: NTPStatus) => status === NTP_ACTIVE;

export const ntpStatusHighlight = ({ status }: NTPStatus, theme: Theme) => {
  switch (status) {
    case NTP_INACTIVE:
      return theme.palette.info.main;
    case NTP_ACTIVE:
      return theme.palette.success.main;
    default:
      return theme.palette.error.main;
  }
}

export const ntpStatus = ({ status }: NTPStatus) => {
  switch (status) {
    case NTP_INACTIVE:
      return "Inactive";
    case NTP_ACTIVE:
      return "Active";
    default:
      return "Unknown";
  }
}
