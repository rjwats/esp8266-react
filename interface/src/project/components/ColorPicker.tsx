import React, { Fragment } from 'react';

import { Box } from '@material-ui/core';
import { ColorChangeHandler, HuePicker, TwitterPicker } from 'react-color'

import { SimpleColors } from './Colors'

interface ColorPickerProps {
  color: string;
  onChange: ColorChangeHandler;
}

class ColorPicker extends React.Component<ColorPickerProps> {
  render() {
    const {
      color,
      onChange
    } = this.props;
    return (
      <Fragment>
        <Box my={2}>
          <HuePicker
            width="100%"
            color={color}
            onChange={onChange}
          />
        </Box>
        <Box my={2}>
          <TwitterPicker
            width="100%"
            triangle="hide"
            color={color}
            onChange={onChange}
            colors={SimpleColors}
          />
        </Box>
      </Fragment>
    );
  }
}

export default ColorPicker;
