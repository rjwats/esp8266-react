import moment from 'moment';

export const formatIsoDateTime = isoDateString => moment.parseZone(isoDateString).format('ll @ HH:mm:ss');
