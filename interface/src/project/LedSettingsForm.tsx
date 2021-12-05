import { FC } from 'react';

import { Box, Button, Container, FormLabel, Slider } from '@mui/material';
import SaveIcon from '@mui/icons-material/Save';

import * as ProjectApi from "./api";
import { ButtonRow, FormLoader, SectionContent } from '../components';
import { LedSettings } from './types';
import { useRest } from '../utils';

const milliwatsToWatts = (milliwatts: number) => milliwatts / 1000;

const LedSettingsForm: FC = () => {
  const { loadData, data, setData, saveData, errorMessage } = useRest<LedSettings>(
    { read: ProjectApi.readLedSettings, update: ProjectApi.updateLedSettings }
  );

  if (!data) {
    return (<FormLoader onRetry={loadData} errorMessage={errorMessage} />);
  }

  const handleSliderChange = (name: keyof LedSettings) => (event: Event, value: number | number[]) => {
    setData({ ...data, [name]: value });
  };

  return (
    <Container maxWidth="md" disableGutters>
      <SectionContent title="LED Settings" titleGutter>
        <Box my={2}>
          <FormLabel>LED Max Power in Watts (0 = unlimited)</FormLabel>
          <Slider
            min={0}
            max={500000}
            step={1000}
            value={data.max_power_milliwatts}
            onChange={handleSliderChange('max_power_milliwatts')}
            valueLabelDisplay="on"
            valueLabelFormat={milliwatsToWatts}
          />
          <FormLabel>Audio Dead Zone</FormLabel>
          <Slider
            min={0}
            max={1500}
            step={10}
            value={data.dead_zone}
            onChange={handleSliderChange('dead_zone')}
          />
          <FormLabel>Audio Smoothing Factor</FormLabel>
          <Slider
            min={0}
            max={.8}
            step={0.05}
            value={data.smoothing_factor}
            onChange={handleSliderChange('smoothing_factor')}
          />
        </Box>
        <ButtonRow mt={2}>
          <Button startIcon={<SaveIcon />} variant="contained" color="primary" type="submit" onClick={saveData}>
            Save
          </Button>
        </ButtonRow>
      </SectionContent>
    </Container>
  );

};

export default LedSettingsForm;
