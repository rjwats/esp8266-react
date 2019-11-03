import React from 'react';

import FormLabel from '@material-ui/core/FormLabel';
import Slider from '@material-ui/core/Slider';

import ColorPicker  from '../../components/ColorPicker.js'
import IncludedBands  from '../../components/IncludedBands'

export class AudioLightLightningMode extends React.Component {

  render() {
    
    const { audioLightSettings, handleChange, handleValueChange, handleColorChange } = this.props;

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
          onChange={handleValueChange('brightness')}
        />

        <FormLabel>Flashes</FormLabel>
        <Slider
          min={1}
          max={20}
          step={1}
          value={audioLightSettings.flashes}
          onChange={handleValueChange('flashes')}
        />

        <FormLabel>Threshold</FormLabel>
        <Slider
          min={0}
          max={255}
          step={1}
          value={audioLightSettings.threshold}
          onChange={handleValueChange('threshold')}
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
