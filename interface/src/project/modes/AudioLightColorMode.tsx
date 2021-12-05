import { FC } from 'react';

import { Box, FormLabel, Slider, Switch } from '@mui/material';

import { ColorModeSettings } from '../types';
import ColorPicker from '../components/ColorPicker';
import IncludedBands from '../components/IncludedBands';

import { useAudioLightMode, AudioLightModeRendererProps } from './AudioLightMode';

const AudioLightColorMode: FC<AudioLightModeRendererProps> = ({ data, updateData }) => {
  const {
    settings, handleChange, handleValueChange, handleSliderChange, handleColorChange
  } = useAudioLightMode<ColorModeSettings>(data, updateData);

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
        <FormLabel>Color</FormLabel>
        <ColorPicker
          color={settings.color}
          onChange={handleColorChange("color")}
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
      </Box>
      <FormLabel>Included Bands</FormLabel>
      <IncludedBands
        value={settings.included_bands}
        onChange={handleChange('included_bands')}
      />
    </div>
  );
};

export default AudioLightColorMode;
