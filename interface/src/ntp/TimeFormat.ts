import moment from 'moment';

export const formatIsoDateTime = (isoDateString: string) => moment.parseZone(isoDateString).format('ll @ HH:mm:ss');
export const formatIsoDateTimeToHr = (isoDateString: string) => moment.parseZone(isoDateString).format('YYYY-MM-DD HH:mm:ssZZ');
