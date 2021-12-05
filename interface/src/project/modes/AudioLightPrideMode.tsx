import { Box, FormLabel, Slider } from '@mui/material';
import { FC } from 'react';

import { PrideModeSettings } from '../types';

import { AudioLightModeRendererProps, useAudioLightMode } from './AudioLightMode';

const AudioLightPrideMode: FC<AudioLightModeRendererProps> = ({ data, updateData }) => {
  const { settings, handleSliderChange } = useAudioLightMode<PrideModeSettings>(data, updateData);

  return (
    <div>
      <Box my={2}>
        <FormLabel>Brightness BPM</FormLabel>
        <Slider
          min={0}
          max={255}
          step={1}
          value={settings.brightness_bpm}
          onChange={handleSliderChange('brightness_bpm')}
        />
        <FormLabel>Brightness Freq Min</FormLabel>
        <Slider
          min={0}
          max={255}
          step={1}
          value={settings.brightness_freq_min}
          onChange={handleSliderChange('brightness_freq_min')}
        />
        <FormLabel>Brightness Freq Max</FormLabel>
        <Slider
          min={0}
          max={255}
          step={1}
          value={settings.brightness_freq_max}
          onChange={handleSliderChange('brightness_freq_max')}
        />
        <FormLabel>Hue BPM</FormLabel>
        <Slider
          min={0}
          max={255}
          step={1}
          value={settings.hue_bpm}
          onChange={handleSliderChange('hue_bpm')}
        />
        <FormLabel>Hue Delta Min</FormLabel>
        <Slider
          min={0}
          max={8192}
          step={32}
          value={settings.hue_delta_min}
          onChange={handleSliderChange('hue_delta_min')}
        />
        <FormLabel>Hue Delta Max</FormLabel>
        <Slider
          min={0}
          max={8192}
          step={32}
          value={settings.hue_delta_max}
          onChange={handleSliderChange('hue_delta_max')}
        />
      </Box>
    </div>
  );
};

export default AudioLightPrideMode;
