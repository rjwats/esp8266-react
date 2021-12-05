import { FC } from 'react';
import { WEB_SOCKET_ROOT } from '../api/endpoints';
import { useSnackbar } from 'notistack';

import { TextField, MenuItem, Button, Container } from '@mui/material';
import SaveIcon from '@mui/icons-material/Save';
import LoadIcon from '@mui/icons-material/SaveAlt';

import * as ProjectApi from "./api";
import { ButtonRow, FormLoader, SectionContent } from '../components';
import { AudioLightModeType, AudioLightMode, AUDIO_LIGHT_MODE_METADATA } from './types';
import { extractErrorMessage, useWs } from '../utils';
import PaletteSettingsLoader from './PaletteSettingsLoader';

export const AUDIO_LIGHT_SETTINGS_WEBSOCKET_URL = WEB_SOCKET_ROOT + "audioLightSettings";

const AudioLightSettingsForm: FC = () => {
  const { connected, data, updateData } = useWs<Partial<AudioLightMode>>(AUDIO_LIGHT_SETTINGS_WEBSOCKET_URL);
  const { enqueueSnackbar } = useSnackbar();

  if (!connected || !data) {
    return (<FormLoader message="Connecting to WebSocket…" />);
  }

  const saveMode = async () => {
    try {
      await ProjectApi.saveModeSettings();
      enqueueSnackbar("Settings saved", { variant: "success" });
    } catch (error: any) {
      enqueueSnackbar(extractErrorMessage(error, "Failed to save config, please try again"), { variant: "error" });
    }
  };

  const loadMode = async () => {
    try {
      await ProjectApi.loadModeSettings();
      enqueueSnackbar("Settings loaded", { variant: "success" });
    } catch (error: any) {
      enqueueSnackbar(extractErrorMessage(error, "Failed to load config, please try again"), { variant: "error" });
    }
  };

  const selectModeComponent = () => data.mode_id && AUDIO_LIGHT_MODE_METADATA[data.mode_id].renderer;

  const ModeComponent = selectModeComponent();

  return (
    <Container maxWidth="md" disableGutters>
      <PaletteSettingsLoader>
        <SectionContent title="Lighting Settings" titleGutter>
          <TextField
            name="mode_id"
            label="Select Mode"
            value={data.mode_id}
            onChange={(event) => updateData({ mode_id: event.target.value as AudioLightModeType }, true, true)}
            fullWidth
            margin="normal"
            variant="outlined"
            select
          >
            {
              Object.entries(AudioLightModeType).map(([, mode_id]) => (
                <MenuItem key={mode_id} value={mode_id}>
                  {AUDIO_LIGHT_MODE_METADATA[mode_id].label}
                </MenuItem>
              ))
            }
          </TextField>
          {console.log(data)}
          {data.settings && ModeComponent && <ModeComponent data={data as AudioLightMode} updateData={updateData} />}
          <ButtonRow mt={2}>
            <Button startIcon={<SaveIcon />} variant="contained" color="primary" onClick={saveMode}>
              Save Settings
            </Button>
            <Button startIcon={<LoadIcon />} variant="contained" color="secondary" onClick={loadMode}>
              Load Settings
            </Button>
          </ButtonRow>
        </SectionContent>
      </PaletteSettingsLoader>
    </Container>
  );
};

export default AudioLightSettingsForm;
