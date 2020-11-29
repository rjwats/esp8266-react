import React from 'react';

import FormLabel from '@material-ui/core/FormLabel';
import Slider from '@material-ui/core/Slider';
import { Box } from '@material-ui/core';

import { ConfettiModeSettings } from '../types';
import { audioLightMode, AudioLightModeProps } from './AudioLightMode';
import PalettePicker from '../components/PalettePicker';

type AudioLightConfettiModeProps = AudioLightModeProps<ConfettiModeSettings>;

class AudioLightConfettiMode extends React.Component<AudioLightConfettiModeProps> {

  render() {
    const { data, handleSliderChange, handleValueChange } = this.props;

    return (
      <div>
        <PalettePicker
          name="palette1"
          label="Palette 1"
          value={data.palette1}
          onChange={handleValueChange('palette1')} />
        <PalettePicker
          name="palette2"
          label="Palette 2"
          value={data.palette2}
          onChange={handleValueChange('palette2')} />
        <PalettePicker
          name="palette3"
          label="Palette 3"
          value={data.palette3}
          onChange={handleValueChange('palette3')} />
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
