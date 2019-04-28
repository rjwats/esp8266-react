import React from 'react';
import PropTypes from 'prop-types';

import FormLabel from '@material-ui/core/FormLabel';

import ColorPicker  from '../components/ColorPicker.js'
import IncludedBands  from '../components/IncludedBands'
import Slider from 'react-rangeslider';

export class AudioLightLightningMode extends React.Component {

  render() {
    
    const { audioLightSettings, handleChange, handleColorChange } = this.props;

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

        <FormLabel>Flashes</FormLabel>
        <Slider
          min={1}
          max={20}
          step={1}
          value={audioLightSettings.flashes}
          onChange={handleChange('flashes')}
        />

        <FormLabel>Threshold</FormLabel>
        <Slider
          min={0}
          max={255}
          step={1}
          value={audioLightSettings.threshold}
          onChange={handleChange('threshold')}
        />

        <FormLabel>Included Bands</FormLabel>
        <IncludedBands
          value={audioLightSettings.included_bands}
          onChange={handleChange('included_bands')}
        />        
      </div>
    );
  }
}

AudioLightLightningMode.propTypes = {
  audioLightSettings: PropTypes.object,
  handleChange: PropTypes.func.isRequired
};
