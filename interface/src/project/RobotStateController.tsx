import React, { Component } from 'react';

import { WEB_SOCKET_ROOT } from '../api';
import { WebSocketControllerProps, WebSocketFormLoader, webSocketController } from '../components';
import { SectionContent } from '../components';
import RobotStateForm from './RobotStateForm';

import { RobotState } from './types';

export const ROBOT_STATE_WEBSOCKET = WEB_SOCKET_ROOT + "robotState";

type RobotStateControllerProps = WebSocketControllerProps<RobotState>;

class RobotStateController extends Component<RobotStateControllerProps> {

  render() {
    return (
      <SectionContent title='Robot Controller' titleGutter>
        <WebSocketFormLoader
          {...this.props}
          render={props => (
            <RobotStateForm {...props} />
          )}
        />
      </SectionContent>
    )
  }

}

export default webSocketController(ROBOT_STATE_WEBSOCKET, 50, RobotStateController);
