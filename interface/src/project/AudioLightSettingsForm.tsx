import React, { Fragment } from 'react';
import { TextField, MenuItem } from '@material-ui/core';
import SaveIcon from '@material-ui/icons/Save';
import LoadIcon from '@material-ui/icons/SaveAlt';

import AudioLightLightningMode from './modes/AudioLightLightningMode';
import AudioLightRainbowMode from './modes/AudioLightRainbowMode';
import AudioLightFireMode from './modes/AudioLightFireMode';
import AudioLightColorMode from './modes/AudioLightColorMode';

import { FormActions, FormButton, WebSocketFormProps } from '../components';
import { AudioLightModeType, AudioLightMode } from './types';
import AudioLightConfettiMode from './modes/AudioLightConfettiMode';
import { redirectingAuthorizedFetch } from '../authentication';
import { AUDIO_LIGHT_LOAD_SETTINGS_ENDPOINT, AUDIO_LIGHT_SAVE_SETTINGS_ENDPOINT } from './AudioLightSettingsController';

type AudioLightSettingsFormProps = WebSocketFormProps<Partial<AudioLightMode>>;

class AudioLightSettingsForm extends React.Component<AudioLightSettingsFormProps> {

  saveMode = async () => {
    const { enqueueSnackbar } = this.props;
    try {
      const response = await redirectingAuthorizedFetch(AUDIO_LIGHT_SAVE_SETTINGS_ENDPOINT, { method: 'POST' });
      if (response.status === 200) {
        enqueueSnackbar("Mode settings saved", { variant: 'success' });
      } else {
        throw new Error(`Unexpected response code ${response.status}`);
      }
    } catch (error) {
      const errorMessage = error.message || "Unknown error";
      enqueueSnackbar("Problem saving: " + errorMessage, { variant: 'error' });
    }
  }

  loadMode = async () => {
    const { enqueueSnackbar } = this.props;
    try {
      const response = await redirectingAuthorizedFetch(AUDIO_LIGHT_LOAD_SETTINGS_ENDPOINT, { method: 'POST' });
      if (response.status === 200) {
        enqueueSnackbar("Mode settings loaded", { variant: 'success' });
      } else {
        throw new Error(`Unexpected response code ${response.status}`);
      }
    } catch (error) {
      const errorMessage = error.message || "Unknown error";
      enqueueSnackbar("Problem loading: " + errorMessage, { variant: 'error' });
    }
  }

  selectModeComponent(): React.ElementType | null {
    const mode_id = this.props.data.mode_id;
    switch (mode_id) {
      case AudioLightModeType.COLOR:
        return AudioLightColorMode;
      case AudioLightModeType.RAINBOW:
        return AudioLightRainbowMode;
      case AudioLightModeType.LIGHTNING:
        return AudioLightLightningMode;
      case AudioLightModeType.CONFETTI:
        return AudioLightConfettiMode;
      case AudioLightModeType.FIRE:
        return AudioLightFireMode;
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
          <MenuItem value={AudioLightModeType.FIRE}>Fire</MenuItem>
          <MenuItem value={AudioLightModeType.PACIFICA}>Pacifica</MenuItem>
          <MenuItem value={AudioLightModeType.PRIDE}>Pride</MenuItem>
        </TextField>
        { ModeComponent && <ModeComponent {...this.props} />}
        <FormActions>
          <FormButton startIcon={<SaveIcon />} variant="contained" color="primary" onClick={this.saveMode}>
            Save Settings
          </FormButton>
          <FormButton startIcon={<LoadIcon />} variant="contained" color="secondary" onClick={this.loadMode}>
            Load Settings
          </FormButton>
        </FormActions>
      </Fragment>
    );
  }
}

export default AudioLightSettingsForm;
