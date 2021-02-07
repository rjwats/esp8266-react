import React from 'react';
import { TextValidator, ValidatorForm, SelectValidator } from 'react-material-ui-form-validator';

import { Checkbox, Switch, MenuItem } from '@material-ui/core';
import SaveIcon from '@material-ui/icons/Save';

import { RestFormProps, FormActions, FormButton, BlockFormControlLabel } from '../components';

import { SerialSettings } from './types';

type SerialSettingsFormProps = RestFormProps<SerialSettings>;

class SerialSettingsForm extends React.Component<SerialSettingsFormProps> {

  componentDidMount() {
  }

  render() {
    const { data, handleValueChange, saveData } = this.props;

    const changeSerialServiceOn = (event: React.ChangeEvent<HTMLInputElement>) => {
    }
    return (
      <ValidatorForm onSubmit={saveData}>
          <BlockFormControlLabel
            control={
              <Switch
                checked={true}
                onChange={changeSerialServiceOn}
                color="primary"
              />
            }
          label="Enable Serial"
        />
        <TextValidator
          validators={['required', 'isNumber', 'minNumber:0', 'maxNumber:36']}
          errorMessages={['rxpin is required', "Must be a number", "Must be greater than 0 ", "Max value is 36"]}
          name="rxpin"
          label="rxpin"
          fullWidth
          variant="outlined"
          value={data.rxpin}
          type="number"
          onChange={handleValueChange('rxpin')}
          margin="normal"
        />
        <TextValidator
          validators={['required', 'isNumber', 'minNumber:0', 'maxNumber:26']}
          errorMessages={['txpin is required', "Must be a number", "Must be greater than 0 ", "Max value is 26"]}
          name="txpin"
          label="txpin"
          fullWidth
          variant="outlined"
          value={data.txpin}
          type="number"
          onChange={handleValueChange('txpin')}
          margin="normal"
        />
        <TextValidator
          validators={['required', 'isNumber', 'minNumber:0', 'maxNumber:115200']}
          errorMessages={['Baud rate is required', "Must be a number", "Must be greater than 0 ", "Max value is 115200"]}
          name="baud"
          label="Baud"
          fullWidth
          variant="outlined"
          value={data.baud}
          type="number"
          onChange={handleValueChange('baud')}
          margin="normal"
        />
        <BlockFormControlLabel
          control={
            <Checkbox
              checked={data.invert}
              onChange={handleValueChange('invert')}
              value="invert"
            />
          }
          label="Inverted signal"
        />
        <FormActions>
          <FormButton startIcon={<SaveIcon />} variant="contained" color="primary" type="submit">
            Save
          </FormButton>
        </FormActions>
      </ValidatorForm>
    );
  }
}

export default SerialSettingsForm;
