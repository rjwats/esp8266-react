import { FC } from 'react';

import { Box, FormLabel, Slider } from '@mui/material';

import { RotateModeSettings } from '../types';
import ModeTransferList from '../components/ModeTransferList';

import { useAudioLightMode, AudioLightModeRendererProps } from './AudioLightMode';

const millisToMinutesAndSeconds = (millis: number) => {
  var minutes = Math.floor(millis / 60000);
  var seconds = Math.floor((millis % 60000) / 1000);
  return minutes + ":" + (seconds < 10 ? '0' : '') + seconds;
};

const AudioLightRotateMode: FC<AudioLightModeRendererProps> = ({ data, updateData }) => {
  const { settings, handleSliderChange, handleChange } = useAudioLightMode<RotateModeSettings>(data, updateData);

  return (
    <div>
      <Box my={2}>
        <FormLabel>Delay</FormLabel>
        <Slider
          min={10000}
          max={600000}
          step={10000}
          value={settings.delay}
          marks={true}
          onChange={handleSliderChange('delay')}
          valueLabelDisplay="on"
          valueLabelFormat={millisToMinutesAndSeconds}
        />
      </Box>
      <Box my={2}>
        <FormLabel>Modes</FormLabel>
        <ModeTransferList selected={settings.modes} onSelectionChanged={handleChange("modes")} />
      </Box>
    </div>
  );
};

export default AudioLightRotateMode;
