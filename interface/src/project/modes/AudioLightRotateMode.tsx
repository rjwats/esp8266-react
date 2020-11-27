import { Box, FormLabel, Input, MenuItem, Select } from '@material-ui/core';
import React from 'react';

import { AudioLightModeType, AUDIO_LIGHT_MODE_METADATA, RotateMode } from '../types';
import { audioLightMode, AudioLightModeProps } from './AudioLightMode';

type AudioLightRotateModeProps = AudioLightModeProps<RotateMode>;

class AudioLightRotateMode extends React.Component<AudioLightRotateModeProps> {

  changeModes = this.props.handleChange("modes");

  render() {
    const { data } = this.props;
    return (
      <div>
        <Box my={2}>
          <FormLabel>Audio Enabled</FormLabel>
          <Select
            labelId="demo-mutiple-chip-label"
            id="demo-mutiple-chip"
            multiple
            value={data.modes}
            onChange={(e) => this.changeModes(e.target.value)}
            input={<Input id="select-multiple-chip" />}
            fullWidth
            variant="outlined"
          >
            {
              Object.entries(AudioLightModeType)
                .filter(([, mode_id]) => AUDIO_LIGHT_MODE_METADATA[mode_id].rotate)
                .map(([, mode_id]) => (
                  <MenuItem key={mode_id} value={mode_id}>
                    {AUDIO_LIGHT_MODE_METADATA[mode_id].label}
                  </MenuItem>
                ))
            }
          </Select>
        </Box>
      </div>
    );
  }
}

export default audioLightMode(AudioLightRotateMode);
