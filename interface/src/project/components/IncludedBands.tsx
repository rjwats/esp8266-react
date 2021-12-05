import React, { FC } from 'react';
import { Switch } from '@mui/material';

interface IncludedBandProps {
  value: boolean[];
  onChange: (value: boolean[]) => void;
}
const IncludedBands: FC<IncludedBandProps> = ({ value, onChange }) => {

  const handleChange = (ordinal: number) => (event: React.ChangeEvent<HTMLInputElement>) => {
    const newValue = [...value];
    newValue[ordinal] = event.target.checked;
    onChange(newValue);
  };

  return (
    <div>
      {value.map((v, i) => (
        <Switch
          key={"band" + i}
          checked={v}
          onChange={handleChange(i)}
          value="enabled"
          color="primary"
        />
      ))}
    </div>
  );

};

export default IncludedBands;
