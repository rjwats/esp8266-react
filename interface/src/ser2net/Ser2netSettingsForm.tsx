import React from 'react';
import { TextValidator, ValidatorForm, SelectValidator } from 'react-material-ui-form-validator';

import { Switch, MenuItem } from '@material-ui/core';
import SaveIcon from '@material-ui/icons/Save';

import { RestFormProps, FormActions, FormButton, BlockFormControlLabel } from '../components';

import { Ser2netSettings } from './types';

type Ser2netSettingsFormProps = RestFormProps<Ser2netSettings>;

class Ser2netSettingsForm extends React.Component<Ser2netSettingsFormProps> {

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
          label="Enable Ser2net"
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

export default Ser2netSettingsForm;
