import React from 'react';

import FormLabel from '@material-ui/core/FormLabel';
import Slider from '@material-ui/core/Slider';

import { FireMode } from '../types';
import { audioLightMode, AudioLightModeProps } from './AudioLightMode';
import { Box, MenuItem, Switch, TextField } from '@material-ui/core';

type AudioLightFireModeProps = AudioLightModeProps<FireMode>;

class AudioLightFireMode extends React.Component<AudioLightFireModeProps> {

  render() {
    const { data, handleValueChange, handleSliderChange } = this.props;

    return (
      <div>
        <TextField
          name="palette"
          label="Palette&hellip;"
          value={data.palette}
          onChange={handleValueChange('palette')}
          fullWidth
          margin="normal"
          variant="outlined"
          select>
          <MenuItem value="rainbow">Rainbow</MenuItem>
          <MenuItem value="party">Party</MenuItem>
          <MenuItem value="heat">Heat</MenuItem>
          <MenuItem value="rainbowstripe">Rainbow Stripe</MenuItem>
          <MenuItem value="cloud">Cloud</MenuItem>
          <MenuItem value="lava">Lava</MenuItem>
          <MenuItem value="ocean">Ocean</MenuItem>
          <MenuItem value="forest">Forest</MenuItem>
          <MenuItem value="pacifica1">Pacifica 1</MenuItem>
          <MenuItem value="pacifica2">Pacifica 2</MenuItem>
          <MenuItem value="pacifica3">Pacifica 3</MenuItem>
        </TextField>

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
