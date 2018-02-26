import moment from 'moment';

export const TIME_AND_DATE = 'DD/MM/YYYY HH:mm:ss';
export const unixTimeToTimeAndDate = unixTime => moment.unix(unixTime).format(TIME_AND_DATE);
