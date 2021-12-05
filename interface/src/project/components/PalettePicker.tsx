import React, { FC, useContext } from 'react';
import { Box, MenuItem, TextField, ListItemText } from '@mui/material';

import { PaletteSettingsContext } from '../PaletteSettingsContext';
import { generateGradient } from '../types';

interface PalettePickerProps {
  name: string;
  label: string;
  value: string;
  onChange: (event: React.ChangeEvent<HTMLInputElement>) => void;
}

const PalettePicker: FC<PalettePickerProps> = ({ name, label, value, onChange }) => {
  const context = useContext(PaletteSettingsContext);
  return (
    <TextField
      name={name}
      label={label}
      value={value}
      onChange={onChange}
      fullWidth
      margin="normal"
      variant="outlined"
      select
    >
      {context.paletteSettings.palettes.map((palette) => (
        <MenuItem key={palette.name} value={palette.name} >
          <Box display="flex" width="100%">
            <Box flex="1">
              <ListItemText primary={palette.name} />
            </Box>
            <Box flex="1" style={{ background: generateGradient(palette) }} />
          </Box>
        </MenuItem>
      ))}
    </TextField>
  );
};

export default PalettePicker;
