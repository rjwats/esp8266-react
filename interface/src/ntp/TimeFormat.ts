import { DateTime } from 'luxon';
import parseMilliseconds from 'parse-ms';

export const fromISOWithOffset = (isoDateString: string) => DateTime.fromISO(isoDateString, { setZone: true });
export const toISOWithOffset = (dateTime: DateTime) =>  dateTime.toFormat("yyyy-LL-dd'T'HH:mm:ssZZZ");
export const formatIsoDateTime = (isoDateString: string) => fromISOWithOffset(isoDateString).toFormat('DD @ HH:mm:ss ZZ');
export const formatLocalDateTime = (dateTime: DateTime) => dateTime.toFormat("yyyy-LL-dd'T'HH:mm");

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
