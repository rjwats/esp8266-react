import React from 'react';

import FormLabel from '@material-ui/core/FormLabel';
import Slider from '@material-ui/core/Slider';

export class AudioLightConfettiMode extends React.Component {

  render() {
    const { audioLightSettings, handleValueChange } = this.props;
    return (
      <div>
        <FormLabel>Palette Changes (per cycle)</FormLabel>
        <Slider
          min={0}
          max={255}
          step={1}
          value={audioLightSettings.max_changes}
          onChange={handleValueChange('max_changes')}
        />
        <FormLabel>Brightness</FormLabel>
        <Slider
          min={0}
          max={255}
          step={1}
          value={audioLightSettings.brightness}
          onChange={handleValueChange('brightness')}
        />
        <FormLabel>Delay</FormLabel>
        <Slider
          min={0}
          max={255}
          step={1}
          value={audioLightSettings.delay}
          onChange={handleValueChange('delay')}
        />

      </div>
    );
  }
}
