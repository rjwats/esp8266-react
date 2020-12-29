import { isEqual } from 'lodash';
import React from 'react';
import { WebSocketFormProps } from '../components';
import { RobotState } from './types';

type RobotStateFormProps = WebSocketFormProps<RobotState>;

class RobotStateForm extends React.Component<RobotStateFormProps> {

  intervalId?: NodeJS.Timeout;
  lastTimestamp: number = 0;

  poll = () => {
    const gamePad = navigator.getGamepads()[0];
    if (!gamePad) {
      return;
    }
    if (this.lastTimestamp !== gamePad.timestamp) {
      const { data, setData, saveData } = this.props;

      const nextState: RobotState = { ...data };
      nextState.claw = Math.round(90 + (gamePad.buttons[7].value * 90));
      nextState.cam_x = Math.round((((-gamePad.axes[2]) + 1) / 2) * 180);
      nextState.cam_y = 90 + Math.round(((gamePad.axes[3] + 1) / 2) * 100);
      nextState.drive_x = Math.round(gamePad.axes[0] * 1023);
      nextState.drive_y = Math.round((-gamePad.axes[1]) * 1023);

      // apply hacky deadzone
      nextState.drive_x = Math.abs(nextState.drive_x) < 100 ? 0 : nextState.drive_x;
      nextState.drive_y = Math.abs(nextState.drive_y) < 100 ? 0 : nextState.drive_y;

      if (!isEqual(nextState, data)) {
        setData(nextState, saveData);
      }
      this.lastTimestamp = gamePad.timestamp;
    }
  }

  componentDidMount = () => {
    this.intervalId = setInterval(this.poll, 200);
  };

  componentWillUnmount = () => {
    this.intervalId && clearInterval(this.intervalId);
  };

  render() {
    const { data } = this.props
    return (
      <div>
        <ul>
          <li>Claw:{data.claw}</li>
          <li>Cam X:{data.cam_x}</li>
          <li>Cam Y:{data.cam_y}</li>
          <li>Drive X:{data.drive_x}</li>
          <li>Drive Y:{data.drive_y}</li>
        </ul>
      </div>
    );
  }
}

export default RobotStateForm;
