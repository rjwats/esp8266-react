import React from 'react';

import FormLabel from '@material-ui/core/FormLabel';
import Slider from '@material-ui/core/Slider';

import { FireModeSettings } from '../types';
import { audioLightMode, AudioLightModeProps } from './AudioLightMode';
import { Box, Switch } from '@material-ui/core';
import PalettePicker from '../components/PalettePicker';

type AudioLightFireModeProps = AudioLightModeProps<FireModeSettings>;

class AudioLightFireMode extends React.Component<AudioLightFireModeProps> {

  render() {
    const { data, handleValueChange, handleSliderChange } = this.props;

    return (
      <div>
        <PalettePicker
          name="palette"
          label="Palette"
          value={data.palette}
          onChange={handleValueChange('palette')} />
        <Box my={2}>
          <FormLabel>Reverse</FormLabel>
          <Switch
            checked={data.reverse}
            onChange={handleValueChange('reverse')}
            color="primary"
          />
        </Box>
        <Box my={2}>
          <FormLabel>Cooling</FormLabel>
          <Slider
            min={20}
            max={100}
            step={1}
            value={data.cooling}
            onChange={handleSliderChange('cooling')}
          />
          <FormLabel>Sparking</FormLabel>
          <Slider
            min={0}
            max={255}
            step={1}
            value={data.sparking}
            onChange={handleSliderChange('sparking')}
          />
        </Box>
      </div>
    );
  }
}

export default audioLightMode(AudioLightFireMode);
