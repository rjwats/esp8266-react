import { NTPStatusData } from "../containers/NTPStatus";
import { Theme } from "@material-ui/core";

export const NTP_INACTIVE = 0;
export const NTP_ACTIVE = 1;

export const isNtpActive = ({ status }: NTPStatusData) => status === NTP_ACTIVE;

export const ntpStatusHighlight = ({ status }: NTPStatusData, theme: Theme) => {
  switch (status) {
    case NTP_INACTIVE:
      return theme.palette.info.main;
    case NTP_ACTIVE:
      return theme.palette.success.main;
    default:
      return theme.palette.error.main;
  }
}

export const ntpStatus = ({ status }: NTPStatusData) => {
  switch (status) {
    case NTP_INACTIVE:
      return "Inactive";
    case NTP_ACTIVE:
      return "Active";
    default:
      return "Unknown";
  }
}
