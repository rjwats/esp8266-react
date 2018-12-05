import React from 'react';
import PropTypes from 'prop-types';

import FormLabel from '@material-ui/core/FormLabel';
import Switch from '@material-ui/core/Switch';

import Slider from 'react-rangeslider';

export class AudioLightRainbowMode extends React.Component {

  render() {
    const { audioLightSettings, handleChange, handleCheckboxChange, classes } = this.props;    
    return (
      <div>
        <FormLabel>Brightness</FormLabel>
        <Slider
          min={0}
          max={255}
          step={1}
          value={audioLightSettings.brightness}
          onChange={handleChange('brightness')}
        />

        <FormLabel>Rotate Speed</FormLabel>
        <Slider
          min={0}
          max={255}
          step={1}
          value={audioLightSettings.rotate_speed}
          onChange={handleChange('rotate_speed')}
        />

        <FormLabel>Audio Enabled</FormLabel>
        <div className={classes.formControl}>
          <Switch
            checked={audioLightSettings.audio_enabled}
            onChange={handleCheckboxChange('audio_enabled')}
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
          onChange={handleChange('hue_delta')}
        />
      </div>
    );
  }
}

AudioLightRainbowMode.propTypes = {
  audioLightSettings: PropTypes.object,
  handleChange: PropTypes.func.isRequired
};
