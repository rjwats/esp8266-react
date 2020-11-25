import React from 'react';

import FormLabel from '@material-ui/core/FormLabel';
import Slider from '@material-ui/core/Slider';

import { PrideMode } from '../types';
import { audioLightMode, AudioLightModeProps } from './AudioLightMode';
import { Box, Switch } from '@material-ui/core';

type AudioLightPrideModeProps = AudioLightModeProps<PrideMode>;

class AudioLightPrideMode extends React.Component<AudioLightPrideModeProps> {
  render() {
    const { data, handleSliderChange } = this.props;

    return (
      <div>
        <Box my={2}>
          <FormLabel>Brightness BPM</FormLabel>
          <Slider
            min={0}
            max={255}
            step={1}
            value={data.brightness_bpm}
            onChange={handleSliderChange('brightness_bpm')}
          />
          <FormLabel>Brightness Freq Min</FormLabel>
          <Slider
            min={0}
            max={255}
            step={1}
            value={data.brightness_freq_min}
            onChange={handleSliderChange('brightness_freq_min')}
          />
          <FormLabel>Brightness Freq Max</FormLabel>
          <Slider
            min={0}
            max={255}
            step={1}
            value={data.brightness_freq_max}
            onChange={handleSliderChange('brightness_freq_max')}
          />
          <FormLabel>Hue BPM</FormLabel>
          <Slider
            min={0}
            max={255}
            step={1}
            value={data.hue_bpm}
            onChange={handleSliderChange('hue_bpm')}
          />
          <FormLabel>Hue Delta Min</FormLabel>
          <Slider
            min={0}
            max={8192}
            step={32}
            value={data.hue_delta_min}
            onChange={handleSliderChange('hue_delta_min')}
          />
          <FormLabel>Hue Delta Max</FormLabel>
          <Slider
            min={0}
            max={8192}
            step={32}
            value={data.hue_delta_max}
            onChange={handleSliderChange('hue_delta_max')}
          />
        </Box>
      </div>
    );
  }
}

export default audioLightMode(AudioLightPrideMode);
