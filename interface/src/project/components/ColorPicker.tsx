import { Box } from '@mui/material';
import { FC, Fragment } from 'react';

import { ColorChangeHandler, HuePicker, TwitterPicker } from 'react-color';

import { SimpleColors } from './Colors';

interface ColorPickerProps {
  color: string;
  onChange: ColorChangeHandler;
}

const ColorPicker: FC<ColorPickerProps> = ({ color, onChange }) => (
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

export default ColorPicker;
