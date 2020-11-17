import React from 'react';

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
      <Box>
        <HuePicker
          width="100%"
          color={color}
          onChange={onChange}
        />
        <TwitterPicker
          width="100%"
          triangle="hide"
          color={color}
          onChange={onChange}
          colors={SimpleColors}
        />
      </Box>
    );
  }
}

export default ColorPicker;
