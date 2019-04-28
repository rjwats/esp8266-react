import React from 'react';
import PropTypes from 'prop-types';

export class AudioLightConfettiMode extends React.Component {

  render() {
    return (
      <div>
        stuff
      </div>
    );
  }
}

AudioLightConfettiMode.propTypes = {
  audioLightSettings: PropTypes.object,
  handleChange: PropTypes.func.isRequired
};
