import React from 'react';

import { Box, FormLabel, Slider, withWidth, WithWidthProps } from '@material-ui/core';
import SaveIcon from '@material-ui/icons/Save';
import { ValidatorForm } from 'react-material-ui-form-validator';

import { FormActions, FormButton, RestFormProps } from '../components';
import { LedSettings } from './types';

type LedSettingsFormProps = RestFormProps<LedSettings> & WithWidthProps;

const milliwatsToWatts = (milliwatts: number) => milliwatts / 1000;

class LedSettingsForm extends React.Component<LedSettingsFormProps> {

  handleSliderChange = (name: keyof LedSettings) => (event: React.ChangeEvent<{}>, value: number | number[]) => {
    const { setData } = this.props;
    setData({ ...this.props.data!, [name]: value });
  }  

  render() {
    const { data, saveData } = this.props;
    return (
      <ValidatorForm onSubmit={saveData}>
        <Box my={2}>
          <FormLabel>LED Max Power in Watts (0 = unlimited)</FormLabel>
          <Slider
            min={0}
            max={500000}
            step={1000}
            value={data.max_power_milliwatts}
            onChange={this.handleSliderChange('max_power_milliwatts')}
            valueLabelDisplay="on"
            valueLabelFormat={milliwatsToWatts}
          />
          <FormLabel>Audio Dead Zone</FormLabel>
          <Slider
            min={0}
            max={1500}
            step={10}
            value={data.dead_zone}
            onChange={this.handleSliderChange('dead_zone')}
          />
          <FormLabel>Audio Smoothing Factor</FormLabel>
          <Slider
            min={0}
            max={.8}
            step={0.05}
            value={data.smoothing_factor}
            onChange={this.handleSliderChange('smoothing_factor')}
          />
        </Box>
        <FormActions>
          <FormButton startIcon={<SaveIcon />} variant="contained" color="primary" type="submit">
            Save
          </FormButton>
        </FormActions>
      </ValidatorForm>
    );
  }

}

export default withWidth()(LedSettingsForm);

