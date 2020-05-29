import React, { Fragment } from 'react';
import { TextValidator, ValidatorForm, SelectValidator } from 'react-material-ui-form-validator';

import MenuItem from '@material-ui/core/MenuItem';
import SaveIcon from '@material-ui/icons/Save';

import {PasswordValidator, RestFormProps, FormActions, FormButton} from '../components';

import { isAPEnabled } from './APModes';
import { APSettings, APProvisionMode } from './types';

type APSettingsFormProps = RestFormProps<APSettings>;

class APSettingsForm extends React.Component<APSettingsFormProps> {

  render() {
    const { data, handleValueChange, saveData, loadData } = this.props;
    return (
      <ValidatorForm onSubmit={saveData} ref="APSettingsForm">
        <SelectValidator name="provision_mode"
          label="Provide Access Point..."
          value={data.provision_mode}
          fullWidth
          variant="outlined"
          onChange={handleValueChange('provision_mode')}
          margin="normal">
          <MenuItem value={APProvisionMode.AP_MODE_ALWAYS}>Always</MenuItem>
          <MenuItem value={APProvisionMode.AP_MODE_DISCONNECTED}>When WiFi Disconnected</MenuItem>
          <MenuItem value={APProvisionMode.AP_NEVER}>Never</MenuItem>
        </SelectValidator>
        {
          isAPEnabled(data) &&
          <Fragment>
            <TextValidator
              validators={['required', 'matchRegexp:^.{1,32}$']}
              errorMessages={['Access Point SSID is required', 'Access Point SSID must be 32 characters or less']}
              name="ssid"
              label="Access Point SSID"
              fullWidth
              variant="outlined"
              value={data.ssid}
              onChange={handleValueChange('ssid')}
              margin="normal"
            />
            <PasswordValidator
              validators={['required', 'matchRegexp:^.{1,64}$']}
              errorMessages={['Access Point Password is required', 'Access Point Password must be 64 characters or less']}
              name="password"
              label="Access Point Password"
              fullWidth
              variant="outlined"
              value={data.password}
              onChange={handleValueChange('password')}
              margin="normal"
            />
          </Fragment>
        }
        <FormActions>
          <FormButton startIcon={<SaveIcon />} variant="contained" color="primary" type="submit">
            Save
          </FormButton>
          <FormButton variant="contained" color="secondary" onClick={loadData}>
            Reset
          </FormButton>
        </FormActions>
      </ValidatorForm>
    );
  }
}

export default APSettingsForm;
