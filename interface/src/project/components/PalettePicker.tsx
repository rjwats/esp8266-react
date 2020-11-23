import React, { FC, useContext } from 'react';
import { MenuItem, TextField } from '@material-ui/core';
import { PaletteSettingsContext } from '../PaletteSettingsContext';

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
      select>
      {context.paletteSettings.palettes.map(palette => (
        <MenuItem id={palette.id} value={palette.id}>{palette.id}</MenuItem>
      ))}
    </TextField>
  );
}


export default PalettePicker;
