import React from 'react';
import PropTypes from 'prop-types';
import ColorPicker  from '../../components/ColorPicker'
import IncludedBands  from '../../components/IncludedBands'

import FormLabel from '@material-ui/core/FormLabel';
import Switch from '@material-ui/core/Switch';
import Slider from 'react-rangeslider';

import 'react-rangeslider/lib/index.css'

export class AudioLightColorMode extends React.Component {

  render() {
    const { audioLightSettings, handleColorChange, handleChange, handleCheckboxChange, classes } = this.props;
    return (
      <div>
        <FormLabel>Color</FormLabel>
        <ColorPicker
          color={ audioLightSettings.color }
          onChange={handleColorChange("color")}
        />

        <FormLabel>Brightness</FormLabel>
        <Slider
          min={0}
          max={255}
          step={1}
          value={audioLightSettings.brightness}
          onChange={handleChange('brightness')}
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

        <FormLabel>Included Bands</FormLabel>
        <IncludedBands
          value={audioLightSettings.included_bands}
          onChange={handleChange('included_bands')}
        />

      </div>
    );
  }
}

AudioLightColorMode.propTypes = {
  audioLightSettings: PropTypes.object,
  onSubmit: PropTypes.func.isRequired,
  handleColorChange: PropTypes.func.isRequired,
  handleChange: PropTypes.func.isRequired
};
