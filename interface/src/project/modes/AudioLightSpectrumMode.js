import React from 'react';

import ColorPicker  from '../../components/ColorPicker.js'
import Slider from 'react-rangeslider';
import FormLabel from '@material-ui/core/FormLabel';

export class AudioLightSpectrumMode extends React.Component {

  render() {
    const { audioLightSettings, handleChange, handleColorChange } = this.props;
    return (
      <div>
        <FormLabel>Bar Color</FormLabel>        
        <ColorPicker
          color={ audioLightSettings.bar_color }
          onChange={handleColorChange("bar_color")}
        />

        <FormLabel>Peak Color</FormLabel>
        <ColorPicker
          color={ audioLightSettings.peak_color }
          onChange={handleColorChange("peak_color")}
        />

        <FormLabel>Brightness</FormLabel>
        <Slider
          min={0}
          max={255}
          step={1}
          value={audioLightSettings.brightness}
          onChange={handleChange('brightness')}
        />

        <FormLabel>Decay Speed</FormLabel>
        <Slider
          min={0}
          max={255}
          step={1}
          value={audioLightSettings.decay_speed}
          onChange={handleChange('decay_speed')}
        />
      </div>
    );
  }
}
