import React, { Fragment } from 'react';

import { WebSocketFormProps } from '../components';
import { AudioLightModeType, AudioLightMode } from './types';
import { TextField, MenuItem } from '@material-ui/core';
import AudioLightLightningMode from './modes/AudioLightLightningMode';

type AudioLightSettingsFormProps = WebSocketFormProps<Partial<AudioLightMode>>;

class AudioLightSettingsForm extends React.Component<AudioLightSettingsFormProps> {

  selectModeComponent(): React.ElementType | null {
    const mode_id = this.props.data.mode_id;
    switch (mode_id) {
      // case AudioLightModeType.COLOR:
      //   return AudioLightColorMode;
      // case AudioLightModeType.SPECTRUM:
      //    return AudioLightSpectrumMode;
      // case AudioLightModeType.RAINBOW:
      //    return AudioLightRainbowMode;
      case AudioLightModeType.LIGHTNING:
        return AudioLightLightningMode;
      //case AudioLightModeType.CONFETTI:
      //   return AudioLightConfettiMode;
      //case AudioLightModeType.FIRE:
      //  return AudioLightFireMode;
    }
    return null;
  }

  render() {
    const { data, saveDataAndClear } = this.props;
    const ModeComponent = this.selectModeComponent();
    return (
      <Fragment>
        <TextField
          name="mode_id"
          label="Select Mode&hellip;"
          value={data.mode_id}
          onChange={event => saveDataAndClear({ mode_id: event.target.value as AudioLightModeType })}
          fullWidth
          margin="normal"
          variant="outlined"
          select>
          <MenuItem value={AudioLightModeType.OFF}>Off</MenuItem>
          <MenuItem value={AudioLightModeType.COLOR}>Single Color</MenuItem>
          <MenuItem value={AudioLightModeType.RAINBOW}>Rainbow</MenuItem>
          <MenuItem value={AudioLightModeType.LIGHTNING}>Lightning</MenuItem>
          <MenuItem value={AudioLightModeType.CONFETTI}>Confetti</MenuItem>

        </TextField>
        { ModeComponent && <ModeComponent {...this.props} />}
      </Fragment>
    );
  }
}

export default AudioLightSettingsForm;
