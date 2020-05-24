import React from 'react';
import { withSnackbar, WithSnackbarProps } from 'notistack';

import { LogEvent } from './types';
import { EVENT_SOURCE_ROOT } from '../api/Env';
import LogEventConsole from './LogEventConsole';
import { addAccessTokenParameter } from '../authentication';

const LOG_EVENT_EVENT_SOURCE_URL = EVENT_SOURCE_ROOT + "log";

interface LogEventController2State {
    eventSource?: EventSource;
    events: LogEvent[];
}

class LogEventController2 extends React.Component<WithSnackbarProps, LogEventController2State>  {
    eventSource: EventSource;

    constructor(props: WithSnackbarProps) {
        super(props);
        this.state = {
            events: []
        };
        this.eventSource = new EventSource(addAccessTokenParameter(LOG_EVENT_EVENT_SOURCE_URL));
        this.eventSource.onmessage = this.onMessage;
    }

    componentWillUnmount() {
        this.eventSource.close();
    }

    onMessage = (event: MessageEvent) => {
        const rawData = event.data;
        if (typeof rawData === 'string' || rawData instanceof String) {
            const event = JSON.parse(rawData as string) as LogEvent;
            this.setState(state => ({ events: [...state.events, event] }));
        }
    }

    render() {
        return (
            <LogEventConsole events={this.state.events} />
        );
    }

}

export default withSnackbar(LogEventController2);