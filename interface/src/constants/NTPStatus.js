import * as Highlight from '../constants/Highlight';

export const NTP_TIME_NOT_SET = 0;
export const NTP_TIME_NEEDS_SYNC = 1;
export const NTP_TIME_SET = 2;

export const isSynchronized = ntpStatus => ntpStatus && (ntpStatus.status === NTP_TIME_NEEDS_SYNC || ntpStatus.status === NTP_TIME_SET);

export const ntpStatusHighlight = ntpStatus => {
  switch (ntpStatus.status){
    case NTP_TIME_SET:
      return Highlight.SUCCESS;
    case NTP_TIME_NEEDS_SYNC:
      return Highlight.WARN;
    case NTP_TIME_NOT_SET:
    default:
      return Highlight.ERROR;
  }
}

export const ntpStatus = ntpStatus => {
  switch (ntpStatus.status){
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
