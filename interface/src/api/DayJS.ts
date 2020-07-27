import dayjs from 'dayjs';
import localizedFormat from 'dayjs/plugin/localizedFormat';
import duration from 'dayjs/plugin/duration';
import relativeTime from 'dayjs/plugin/relativeTime';
import utc from 'dayjs/plugin/utc';

dayjs.extend(relativeTime)
dayjs.extend(duration);
dayjs.extend(localizedFormat);
dayjs.extend(utc);

export default dayjs;
