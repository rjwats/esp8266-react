import React from 'react';
import PropTypes from 'prop-types';

export class AudioLightRainbowMode extends React.Component {

  render() {
    return (
      <div>
        Rainbow Mode Settings Will Go Here
      </div>
    );
  }
}

AudioLightRainbowMode.propTypes = {
  audioLightSettings: PropTypes.object,
  handleChange: PropTypes.func.isRequired
};
