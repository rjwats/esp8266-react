import React, { Fragment } from 'react';
import { TextField, MenuItem } from '@material-ui/core';
import SaveIcon from '@material-ui/icons/Save';
import LoadIcon from '@material-ui/icons/SaveAlt';

import { FormActions, FormButton, WebSocketFormProps } from '../components';
import { AudioLightModeType, AudioLightMode, AUDIO_LIGHT_LOAD_SETTINGS_ENDPOINT, AUDIO_LIGHT_SAVE_SETTINGS_ENDPOINT, AUDIO_LIGHT_MODE_METADATA } from './types';
import { redirectingAuthorizedFetch } from '../authentication';

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

  selectModeComponent(): React.ElementType | undefined {
    const { mode_id } = this.props.data;
    return mode_id && AUDIO_LIGHT_MODE_METADATA[mode_id].renderer;
  }

  render() {
    const { data, saveDataAndClear } = this.props;
    const ModeComponent = this.selectModeComponent();
    return (
      <Fragment>
        <TextField
          name="mode_id"
          label="Select Mode"
          value={data.mode_id}
          onChange={event => saveDataAndClear({ mode_id: event.target.value as AudioLightModeType })}
          fullWidth
          margin="normal"
          variant="outlined"
          select>
          {
            Object.entries(AudioLightModeType).map(([, mode_id]) => (
              <MenuItem key={mode_id} value={mode_id}>
                {AUDIO_LIGHT_MODE_METADATA[mode_id].label}
              </MenuItem>
            ))
          }
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
