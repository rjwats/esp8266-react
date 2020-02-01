import moment from 'moment';

export const formatIsoDateTime = (isoDateString: string) => moment.parseZone(isoDateString).format('ll @ HH:mm:ss');
