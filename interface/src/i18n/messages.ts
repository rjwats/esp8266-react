import { merge } from 'lodash';
import { signInMessages } from '../messages';
import { projectMessages } from '../project/messages';

export const messages: Record<string, Record<string, string>> = merge(
    signInMessages,
    projectMessages
);
