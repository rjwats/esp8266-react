import React from 'react';

import FormLabel from '@material-ui/core/FormLabel';
import Slider from '@material-ui/core/Slider';

import { ColorModeSettings } from '../types';
import { audioLightMode, AudioLightModeProps } from './AudioLightMode';
import { Box, Switch } from '@material-ui/core';
import ColorPicker from '../components/ColorPicker';
import IncludedBands from '../components/IncludedBands';

type AudioLightColorModeProps = AudioLightModeProps<ColorModeSettings>;

class AudioLightColorMode extends React.Component<AudioLightColorModeProps> {

  render() {
    const { data, handleChange, handleValueChange, handleSliderChange, handleColorChange } = this.props;

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
          <FormLabel>Color</FormLabel>
          <ColorPicker
            color={data.color}
            onChange={handleColorChange("color")}
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
        </Box>
        <FormLabel>Included Bands</FormLabel>
        <IncludedBands
          value={data.included_bands}
          onChange={handleChange('included_bands')}
        />
      </div>
    );
  }
}

export default audioLightMode(AudioLightColorMode);
