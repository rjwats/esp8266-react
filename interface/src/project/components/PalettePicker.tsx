import React from 'react';
import { MenuItem, TextField } from '@material-ui/core';

interface PalettePickerProps {
  name: string;
  label: string;
  value: string;
  onChange: (event: React.ChangeEvent<HTMLInputElement>) => void;
}

class PalettePicker extends React.Component<PalettePickerProps> {
  render() {
    const {
      name,
      label,
      value,
      onChange
    } = this.props;
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
        <MenuItem value="Rainbow">Rainbow</MenuItem>
        <MenuItem value="Party">Party</MenuItem>
        <MenuItem value="Heat">Heat</MenuItem>
        <MenuItem value="Rainbow Stripe">Rainbow Stripe</MenuItem>
        <MenuItem value="Cloud">Cloud</MenuItem>
        <MenuItem value="Lava">Lava</MenuItem>
        <MenuItem value="Ocean">Ocean</MenuItem>
        <MenuItem value="Forest">Forest</MenuItem>
        <MenuItem value="Pacifica 1">Pacifica 1</MenuItem>
        <MenuItem value="Pacifica 2">Pacifica 2</MenuItem>
        <MenuItem value="Pacifica 3">Pacifica 3</MenuItem>
      </TextField>
    );
  }
}

export default PalettePicker;
