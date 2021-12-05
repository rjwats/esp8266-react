import { FC } from 'react';

import { Box, FormLabel, Slider, Switch } from '@mui/material';

import { RainbowModeSettings } from '../types';

import { useAudioLightMode, AudioLightModeRendererProps } from './AudioLightMode';

const AudioLightRotateMode: FC<AudioLightModeRendererProps> = ({ data, updateData }) => {
  const { settings, handleSliderChange, handleValueChange } = useAudioLightMode<RainbowModeSettings>(data, updateData);

  return (
    <div>
      <Box my={2}>
        <FormLabel>Audio Enabled</FormLabel>
        <Switch
          checked={settings.audio_enabled}
          onChange={handleValueChange('audio_enabled')}
          value="enabled"
          color="primary"
        />
      </Box>
      <Box my={2}>
        <FormLabel>Brightness</FormLabel>
        <Slider
          min={0}
          max={255}
          step={1}
          value={settings.brightness}
          onChange={handleSliderChange('brightness')}
        />
        <FormLabel>Rotate Speed</FormLabel>
        <Slider
          min={0}
          max={255}
          step={1}
          value={settings.rotate_speed}
          onChange={handleSliderChange('rotate_speed')}
        />
        <FormLabel>Hue Delta</FormLabel>
        <Slider
          min={0}
          max={255}
          step={1}
          value={settings.hue_delta}
          onChange={handleSliderChange('hue_delta')}
        />
      </Box>
    </div>
  );
};

export default AudioLightRotateMode;
