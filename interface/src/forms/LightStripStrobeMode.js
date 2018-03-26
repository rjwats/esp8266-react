import React from 'react';
import PropTypes from 'prop-types';
import { HuePicker, TwitterPicker } from 'react-color'
import { SimpleColors } from '../constants/Colors.js'

import Slider from 'react-rangeslider';
import 'react-rangeslider/lib/index.css'

export class LightStripStrobeMode extends React.Component {

  render() {
    const { lightStripSettings, handleColorChange, handleChange } = this.props;
    return (
      <div>
        <HuePicker
          width="100%"
          color={ lightStripSettings.color }
          onChange={handleColorChange("color")}
        />
        <TwitterPicker
          width="100%"
          triangle="hide"
          color={ lightStripSettings.color }
          onChange={handleColorChange("color")}
          colors={ SimpleColors }
        />
        <Slider
          min={0}
          max={255}
          step={1}
          value={lightStripSettings.strobe_speed}
          onChange={handleChange('strobe_speed')}
        />
      </div>
    );
  }
}

LightStripStrobeMode.propTypes = {
  lightStripSettings: PropTypes.object,
  onSubmit: PropTypes.func.isRequired,
  handleColorChange: PropTypes.func.isRequired,
  handleChange: PropTypes.func.isRequired
};
