import * as Highlight from '../constants/Highlight';

export const NTP_INACTIVE = 0;
export const NTP_ACTIVE = 1;

export const isNtpActive = ntpStatus => ntpStatus && ntpStatus.status === NTP_ACTIVE;

export const ntpStatusHighlight = ntpStatus => {
  switch (ntpStatus.status) {
    case NTP_INACTIVE:
      return Highlight.IDLE;
    case NTP_ACTIVE:
      return Highlight.SUCCESS;
    default:
      return Highlight.ERROR;
  }
}

export const ntpStatus = ntpStatus => {
  switch (ntpStatus.status) {
    case NTP_INACTIVE:
      return "Inactive";
    case NTP_ACTIVE:
      return "Active";
    default:
      return "Unknown";
  }
}
