import moment, { Moment } from 'moment';

export const formatIsoDateTime = (isoDateString: string) => moment.parseZone(isoDateString).format('ll @ HH:mm:ss');
export const formatLocalDateTime = (moment: Moment) => moment.format('YYYY-MM-DDTHH:mm');
export const formatIsoDateTimeToHr = (isoDateString: string) => moment.parseZone(isoDateString).format('YYYY-MM-DD HH:mm:ssZZ');
