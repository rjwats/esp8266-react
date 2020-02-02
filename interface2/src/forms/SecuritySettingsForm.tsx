import React from 'react';
import { ValidatorForm } from 'react-material-ui-form-validator';

import { Box, Typography } from '@material-ui/core';
import SaveIcon from '@material-ui/icons/Save';

import PasswordValidator from '../components/PasswordValidator';
import { SecuritySettings } from '../containers/SecuritySettingsController';
import { RestFormProps } from '../components/RestFormLoader';
import { withAuthenticatedContext, AuthenticatedContextProps } from '../authentication/AuthenticationContext';
import FormActions from '../components/FormActions';
import FormButton from '../components/FormButton';

type SecuritySettingsFormProps = RestFormProps<SecuritySettings> & AuthenticatedContextProps;

class SecuritySettingsForm extends React.Component<SecuritySettingsFormProps> {

  onSubmit = () => {
    this.props.saveData();
    this.props.authenticatedContext.refresh();
  }

  render() {
    const { data, handleValueChange, loadData } = this.props;
    return (
      <ValidatorForm onSubmit={this.onSubmit}>
        <PasswordValidator
          validators={['required', 'matchRegexp:^.{1,64}$']}
          errorMessages={['JWT Secret Required', 'JWT Secret must be 64 characters or less']}
          name="jwt_secret"
          label="JWT Secret"
          fullWidth
          variant="outlined"
          value={data.jwt_secret}
          onChange={handleValueChange('jwt_secret')}
          margin="normal"
        />
        <Typography component="div" variant="body1">
          <Box bgcolor="primary.main" color="primary.contrastText" p={2} mt={2} mb={2}>
            If you modify the JWT Secret, all users will be logged out.
          </Box>
        </Typography>
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

export default withAuthenticatedContext(SecuritySettingsForm);
