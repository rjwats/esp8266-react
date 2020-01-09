import { NTPStatusData } from '../containers/NTPStatus';
import { Theme } from '@material-ui/core';

export const NTP_TIME_NOT_SET = 0;
export const NTP_TIME_NEEDS_SYNC = 1;
export const NTP_TIME_SET = 2;

export const isSynchronized = (ntpStatus: NTPStatusData) => ntpStatus.status === NTP_TIME_NEEDS_SYNC || ntpStatus.status === NTP_TIME_SET;

export const ntpStatusHighlight = (ntpStatus: NTPStatusData, theme: Theme) => {
  switch (ntpStatus.status) {
    case NTP_TIME_SET:
      return theme.palette.success.main;
    case NTP_TIME_NEEDS_SYNC:
      return theme.palette.warning.main;
    case NTP_TIME_NOT_SET:
    default:
      return theme.palette.error.main;
  }
}

export const ntpStatus = (ntpStatus: NTPStatusData) => {
  switch (ntpStatus.status) {
    case NTP_TIME_SET:
      return "Synchronized";
    case NTP_TIME_NEEDS_SYNC:
      return "Synchronization required";
    case NTP_TIME_NOT_SET:
      return "Time not set"
    default:
      return "Unknown";
  }
}
