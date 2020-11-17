import React from 'react';

import Switch from '@material-ui/core/Switch';

interface IncludedBandProps {
  value: boolean[];
  onChange: (value: boolean[]) => void;
}

class IncludedBands extends React.Component<IncludedBandProps> {

  handleChange = (ordinal: number) => (event: React.ChangeEvent<HTMLInputElement>) => {
    const { value, onChange } = this.props;
    const newValue = [...value];
    newValue[ordinal] = event.target.checked;
    onChange(newValue);
  };

  render() {
    const { value } = this.props;
    return (
      <div>
        {value.map((v, i) => (
          <Switch
            key={"band" + i}
            checked={v}
            onChange={this.handleChange(i)}
            value="enabled"
            color="primary"
          />
        ))}
      </div>
    );
  }
}

export default IncludedBands;