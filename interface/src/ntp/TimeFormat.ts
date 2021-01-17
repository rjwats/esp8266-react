import { parseISO, format } from 'date-fns';
import parseMilliseconds from 'parse-ms';

export const formatDateTime = (dateTime: string) => {
  return format(parseISO(dateTime.substr(0, 19)), 'PP @ HH:mm:ss');
}

export const formatLocalDateTimeNow = () => {
  return format(new Date(), "yyyy-MM-dd'T'HH:mm");
}

export const formatLocalDateTime = (dateTime: string) => {
  return format(parseISO(dateTime), "yyyy-MM-dd'T'HH:mm:ss");
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
