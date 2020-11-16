import React from 'react';

import { WebSocketFormProps } from '../components';
import { AudioLightModeType, AudioLightSettings } from './types';
import { TextField, MenuItem } from '@material-ui/core';

type AudioLightSettingsFormProps = WebSocketFormProps<AudioLightSettings>;

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
        <MenuItem value={AudioLightModeType.OFF}>Off</MenuItem>
        <MenuItem value={AudioLightModeType.COLOR}>Single Color</MenuItem>
        <MenuItem value={AudioLightModeType.RAINBOW}>Rainbow</MenuItem>
        <MenuItem value={AudioLightModeType.LIGHTNING}>Lightning</MenuItem>
        <MenuItem value={AudioLightModeType.CONFETTI}>Confetti</MenuItem>
        <MenuItem value={AudioLightModeType.FIRE}>Fire</MenuItem>
      </TextField>
    );
  }
}

export default AudioLightSettingsForm;
