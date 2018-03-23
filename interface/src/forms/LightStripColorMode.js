import React from 'react';
import PropTypes from 'prop-types';
import { SketchPicker } from 'react-color'
import { FormLabel, FormControl, FormGroup } from 'material-ui/Form';

export class LightStripColorMode extends React.Component {

  render() {
    const { lightStripSettings, handleColorChange } = this.props;
    return (
      <div>
        <FormControl component="color">
          <FormLabel component="legend">Color</FormLabel>
          <FormGroup>
            <SketchPicker
              color={ lightStripSettings.color }
              disableAlpha={true}
              width={400}
              onChangeComplete={handleColorChange("color")}
            />
          </FormGroup>
        </FormControl>
      </div>
    );
  }
}

LightStripColorMode.propTypes = {
  lightStripSettings: PropTypes.object,
  handleValueChange: PropTypes.func.isRequired,
  handleColorChange: PropTypes.func.isRequired
};
