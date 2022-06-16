import React from 'react';
import Sockette from 'sockette';
import { withSnackbar, WithSnackbarProps } from 'notistack';

import { addAccessTokenParameter } from '../../api/authentication';
import { LogEvent } from '../../types';
import { WEB_SOCKET_ROOT } from '../../api/endpoints';
import LogEventConsole from './LogEventConsole';

const LOG_EVENT_WEB_SOCKET_URL = WEB_SOCKET_ROOT + "serial";

interface LogEventControllerState {
  ws: Sockette;
  events: LogEvent[];
}

class LogEventController extends React.Component<WithSnackbarProps, LogEventControllerState>  {

  constructor(props: WithSnackbarProps) {
    super(props);
    this.state = {
      ws: new Sockette(addAccessTokenParameter(LOG_EVENT_WEB_SOCKET_URL), {
        onmessage: this.onMessage
      }),
      events: []
    };
  }

  componentWillUnmount() {
    this.state.ws.close();
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

export default withSnackbar(LogEventController);