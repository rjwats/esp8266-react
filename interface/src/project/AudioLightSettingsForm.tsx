import React from 'react';

import { RestFormProps } from '../components';
import { AudioLightMode, AudioLightSettings } from './types';
import { TextField, MenuItem } from '@material-ui/core';

type AudioLightSettingsFormProps = RestFormProps<AudioLightSettings>;

class AudioLightSettingsForm extends React.Component<AudioLightSettingsFormProps> {


  render() {
    const { data, handleValueChange, saveData } = this.props;
    return (
      <TextField
        name="mode_id"
        label="Select Mode&hellip;"
        value={data.mode_id}
        onChange={handleValueChange("mode_id", saveData)}
        fullWidth
        margin="normal"
        variant="outlined"
        select>
        <MenuItem value={AudioLightMode.OFF}>Off</MenuItem>
        <MenuItem value={AudioLightMode.COLOR}>Single Color</MenuItem>
        <MenuItem value={AudioLightMode.RAINBOW}>Rainbow</MenuItem>
        <MenuItem value={AudioLightMode.LIGHTNING}>Lightning</MenuItem>
        <MenuItem value={AudioLightMode.CONFETTI}>Confetti</MenuItem>
        <MenuItem value={AudioLightMode.FIRE}>Fire</MenuItem>
      </TextField>
    );
  }
}

export default AudioLightSettingsForm;
