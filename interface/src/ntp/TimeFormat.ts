import { DateTime } from 'luxon';
import parseMilliseconds from 'parse-ms';

export const fromOffsetIso = (isoDateString: string) => {
  return DateTime.fromISO(isoDateString, { setZone: true });
}

export const toUtcIso = (dateTime: DateTime) => {
  return dateTime.set({ millisecond: 0 }).toUTC().toISO({
    suppressMilliseconds: true
  });
}

export const toLocalDateTime = (dateTime: DateTime) => {
  return dateTime.set({ second: 0, millisecond: 0 }).toISO({
    suppressMilliseconds: true, suppressSeconds: true, includeOffset: false
  });
}

export const toLocaleFormat = (isoDateString: string) => {
  return fromOffsetIso(isoDateString).toFormat('DD @ HH:mm:ssZZ');
}

export const formatDuration = (duration: number) => {
  const { days, hours, minutes, seconds } = parseMilliseconds(duration * 1000);
  var formatted = '';
  if (days) {
    formatted += days + ' days ';
  }
  if (formatted || hours) {
    formatted += hours + ' hours ';
  }
  if (formatted || minutes) {
    formatted += minutes + ' minutes ';
  }
  if (formatted || seconds) {
    formatted += seconds + ' seconds';
  }
  return formatted;
}
