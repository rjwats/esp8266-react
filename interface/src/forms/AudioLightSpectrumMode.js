import React from 'react';
import PropTypes from 'prop-types';
import Slider from 'react-rangeslider';

export class AudioLightSpectrumMode extends React.Component {

  render() {
    const { audioLightSettings, handleChange } = this.props;
    return (
      <div>
        <Slider
          min={0}
          max={255}
          step={1}
          value={audioLightSettings.brightness}
          onChange={handleChange('brightness')}
        />
      </div>
    );
  }
}

AudioLightSpectrumMode.propTypes = {
  audioLightSettings: PropTypes.object,
  handleChange: PropTypes.func.isRequired
};
