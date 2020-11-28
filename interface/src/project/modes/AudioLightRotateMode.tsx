import { Box, FormLabel, Slider } from '@material-ui/core';
import React from 'react';
import ModeTransferList from '../components/ModeTransferList';

import { RotateMode } from '../types';
import { audioLightMode, AudioLightModeProps } from './AudioLightMode';

type AudioLightRotateModeProps = AudioLightModeProps<RotateMode>;

const millisToMinutesAndSeconds = (millis: number) => {
  var minutes = Math.floor(millis / 60000);
  var seconds = Math.floor((millis % 60000) / 1000);
  return minutes + ":" + (seconds < 10 ? '0' : '') + seconds;
}

class AudioLightRotateMode extends React.Component<AudioLightRotateModeProps> {

  changeModes = this.props.handleChange("modes");

  render() {
    const { data, handleSliderChange } = this.props;
    return (
      <div>
        <Box my={2}>
          <FormLabel>Delay</FormLabel>
          <Slider
            min={10000}
            max={600000}
            step={10000}
            value={data.delay}
            marks={true}
            onChange={handleSliderChange('delay')}
            valueLabelDisplay="on"
            valueLabelFormat={millisToMinutesAndSeconds}
          />
        </Box>
        <Box my={2}>
          <ModeTransferList selected={data.modes} onSelectionChanged={this.changeModes} />
        </Box>
      </div>
    );
  }
}

export default audioLightMode(AudioLightRotateMode);
