import React from 'react';

import { PacificaModeSettings } from '../types';
import { audioLightMode, AudioLightModeProps } from './AudioLightMode';
import PalettePicker from '../components/PalettePicker';

type AudioLightPacificaModeProps = AudioLightModeProps<PacificaModeSettings>;

class AudioLightPacificaMode extends React.Component<AudioLightPacificaModeProps> {

  render() {
    const { data, handleValueChange } = this.props;

    return (
      <div>
        <PalettePicker
          name="palette1"
          label="Palette 1"
          value={data.palette1}
          onChange={handleValueChange('palette1')} />
        <PalettePicker
          name="palette2"
          label="Palette 2"
          value={data.palette2}
          onChange={handleValueChange('palette2')} />
        <PalettePicker
          name="palette3"
          label="Palette 3"
          value={data.palette3}
          onChange={handleValueChange('palette3')} />
      </div>
    );
  }
}

export default audioLightMode(AudioLightPacificaMode);
