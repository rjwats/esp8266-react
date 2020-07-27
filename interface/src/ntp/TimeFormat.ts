import { Dayjs } from 'dayjs';
import { dayjs } from '../api';

export const formatIgnoringOffset = (isoDateString: string) => dayjs(isoDateString.substr(0, 19)).format('ll @ HH:mm:ss');
export const formatLocalDateTime = (dayJs: Dayjs) => dayJs.format('YYYY-MM-DDTHH:mm');
