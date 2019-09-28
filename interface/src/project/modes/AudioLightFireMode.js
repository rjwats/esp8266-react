import React from 'react';
import PropTypes from 'prop-types';

export class AudioLightFireMode extends React.Component {

  render() {
    return (
      <div>
        stuff
      </div>
    );
  }
}

AudioLightFireMode.propTypes = {
  audioLightSettings: PropTypes.object,
  handleChange: PropTypes.func.isRequired
};
