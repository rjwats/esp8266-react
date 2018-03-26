import React from 'react';
import PropTypes from 'prop-types';

import Slider from 'react-rangeslider';
import 'react-rangeslider/lib/index.css'

export class LightStripColorCycleMode extends React.Component {

  render() {
    const { lightStripSettings, handleChange } = this.props;
    return (
      <div>
        <Slider
          min={0}
          max={255}
          step={1}
          value={lightStripSettings.rotation_speed}
          onChange={handleChange('rotation_speed')}
        />
      </div>
    );
  }
}

LightStripColorCycleMode.propTypes = {
  lightStripSettings: PropTypes.object,
  onSubmit: PropTypes.func.isRequired,
  handleChange: PropTypes.func.isRequired
};
