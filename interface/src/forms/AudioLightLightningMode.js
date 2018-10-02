import React from 'react';
import PropTypes from 'prop-types';

export class AudioLightLightningMode extends React.Component {

  render() {
    return (
      <div>
        Lightning Mode Settings Will Go Here
      </div>
    );
  }
}

AudioLightLightningMode.propTypes = {
  audioLightSettings: PropTypes.object,
  handleChange: PropTypes.func.isRequired
};
