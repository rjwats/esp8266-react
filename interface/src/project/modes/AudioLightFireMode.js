import React from 'react';
import FormLabel from '@material-ui/core/FormLabel';
import Switch from '@material-ui/core/Switch';
import Slider from '@material-ui/core/Slider';

export class AudioLightFireMode extends React.Component {

  render() {
    const { audioLightSettings, classes, handleValueChange } = this.props;

    return (
      <div>
        <FormLabel>Cooling</FormLabel>
        <Slider
          min={1}
          max={255}
          step={1}
          value={audioLightSettings.cooling}
          onChange={handleValueChange('cooling')}
        />

        <FormLabel>Sparking</FormLabel>
        <Slider
          min={0}
          max={255}
          step={1}
          value={audioLightSettings.sparking}
          onChange={handleValueChange('sparking')}
        />

        <FormLabel>Reverse</FormLabel>
        <div className={classes.formControl}>
          <Switch
            checked={audioLightSettings.reverse}
            onChange={handleValueChange('reverse')}
            color="primary"
          />
        </div>

      </div>
    );
  }
}

