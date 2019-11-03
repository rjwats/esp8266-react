import React from 'react';

import FormLabel from '@material-ui/core/FormLabel';
import Switch from '@material-ui/core/Switch';
import Slider from '@material-ui/core/Slider';

export class AudioLightRainbowMode extends React.Component {

  render() {
    const { audioLightSettings, handleValueChange, classes } = this.props;    
    return (
      <div>
        <FormLabel>Brightness</FormLabel>
        <Slider
          min={0}
          max={255}
          step={1}
          value={audioLightSettings.brightness}
          onChange={handleValueChange('brightness')}
        />

        <FormLabel>Rotate Speed</FormLabel>
        <Slider
          min={0}
          max={255}
          step={1}
          value={audioLightSettings.rotate_speed}
          onChange={handleValueChange('rotate_speed')}
        />

        <FormLabel>Audio Enabled</FormLabel>
        <div className={classes.formControl}>
          <Switch
            checked={audioLightSettings.audio_enabled}
            onChange={handleValueChange('audio_enabled')}
            value="enabled"
            color="primary"
          />          
        </div>

        <FormLabel>Hue Delta</FormLabel>
        <Slider
          min={0}
          max={255}
          step={1}
          value={audioLightSettings.hue_delta}
          onChange={handleValueChange('hue_delta')}
        />
      </div>
    );
  }
}
