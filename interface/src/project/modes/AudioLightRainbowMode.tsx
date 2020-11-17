import React from 'react';

import FormLabel from '@material-ui/core/FormLabel';
import Slider from '@material-ui/core/Slider';

import { RainbowMode } from '../types';
import { audioLightMode, AudioLightModeProps } from './AudioLightMode';
import { Box, Switch } from '@material-ui/core';

type AudioLightRainbowModeProps = AudioLightModeProps<RainbowMode>;

class AudioLightRainbowMode extends React.Component<AudioLightRainbowModeProps> {

  render() {
    const { data, handleValueChange, handleSliderChange } = this.props;

    return (
      <div>
        <Box my={2}>
          <FormLabel>Audio Enabled</FormLabel>
          <Switch
            checked={data.audio_enabled}
            onChange={handleValueChange('audio_enabled')}
            value="enabled"
            color="primary"
          />
        </Box>
        <Box my={2}>
          <FormLabel>Brightness</FormLabel>
          <Slider
            min={0}
            max={255}
            step={1}
            value={data.brightness}
            onChange={handleSliderChange('brightness')}
          />
          <FormLabel>Rotate Speed</FormLabel>
          <Slider
            min={0}
            max={255}
            step={1}
            value={data.rotate_speed}
            onChange={handleSliderChange('rotate_speed')}
          />
          <FormLabel>Hue Delta</FormLabel>
          <Slider
            min={0}
            max={255}
            step={1}
            value={data.hue_delta}
            onChange={handleSliderChange('hue_delta')}
          />
        </Box>
      </div>
    );
  }
}

export default audioLightMode(AudioLightRainbowMode);
