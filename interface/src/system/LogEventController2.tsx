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
  eventSource?: EventSource;
  reconnectTimeout?: NodeJS.Timeout;

  constructor(props: WithSnackbarProps) {
    super(props);
    this.state = {
      events: []
    };
  }

  componentDidMount() {
    this.configureEventSource();
  }

  componentWillUnmount() {
    if (this.eventSource) {
      this.eventSource.close();
    }
    if (this.reconnectTimeout) {
      clearTimeout(this.reconnectTimeout);
    }
  }

  configureEventSource = () => {
    this.eventSource = new EventSource(addAccessTokenParameter(LOG_EVENT_EVENT_SOURCE_URL));
    this.eventSource.onmessage = this.onMessage;
    this.eventSource.onerror = this.onError;
  }

  onError = () => {
    if (this.eventSource && this.reconnectTimeout) {
      this.eventSource.close();
      this.eventSource = undefined;
      this.reconnectTimeout = setTimeout(this.configureEventSource, 1000);
    }
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