import React from 'react';

import FormLabel from '@material-ui/core/FormLabel';
import Slider from '@material-ui/core/Slider';
import { Box } from '@material-ui/core';

import { ConfettiMode } from '../types';
import { audioLightMode, AudioLightModeProps } from './AudioLightMode';

type AudioLightConfettiModeProps = AudioLightModeProps<ConfettiMode>;

class AudioLightConfettiMode extends React.Component<AudioLightConfettiModeProps> {

  render() {
    const { data, handleSliderChange } = this.props;

    return (
      <div>
        <Box my={2}>
          <FormLabel>Palette Changes (per cycle)</FormLabel>
          <Slider
            min={0}
            max={255}
            step={1}
            value={data.max_changes}
            onChange={handleSliderChange('max_changes')}
          />
          <FormLabel>Brightness</FormLabel>
          <Slider
            min={0}
            max={255}
            step={1}
            value={data.brightness}
            onChange={handleSliderChange('brightness')}
          />
          <FormLabel>Delay</FormLabel>
          <Slider
            min={0}
            max={255}
            step={1}
            value={data.delay}
            onChange={handleSliderChange('delay')}
          />
        </Box>
      </div>
    );
  }
}

export default audioLightMode(AudioLightConfettiMode);
