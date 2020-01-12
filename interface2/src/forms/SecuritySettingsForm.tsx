import React from 'react';
import { ValidatorForm } from 'react-material-ui-form-validator';

import { withStyles, Theme, createStyles, WithStyles } from '@material-ui/core/styles';
import { Box, Button, Typography } from '@material-ui/core';
import SaveIcon from '@material-ui/icons/Save';

import PasswordValidator from '../components/PasswordValidator';
import { SecuritySettingsData } from '../containers/SecuritySettings';
import { RestFormProps } from '../components/RestFormLoader';
import { withAuthenticatedContext, AuthenticatedContextProps } from '../authentication/AuthenticationContext';

const styles = (theme: Theme) => createStyles({
  textField: {
    width: "100%"
  },
  button: {
    marginRight: theme.spacing(2),
    marginTop: theme.spacing(2),
  }
});

type SecuritySettingsFormProps = RestFormProps<SecuritySettingsData> & AuthenticatedContextProps & WithStyles<typeof styles>;

class SecuritySettingsForm extends React.Component<SecuritySettingsFormProps> {

  onSubmit = () => {
    this.props.saveData();
    this.props.authenticatedContext.refresh();
  }

  render() {
    const { classes, data, handleValueChange, loadData } = this.props;
    return (
      <ValidatorForm onSubmit={this.onSubmit}>
        <PasswordValidator
          validators={['required', 'matchRegexp:^.{1,64}$']}
          errorMessages={['JWT Secret Required', 'JWT Secret must be 64 characters or less']}
          name="jwt_secret"
          label="JWT Secret"
          className={classes.textField}
          value={data.jwt_secret}
          onChange={handleValueChange('jwt_secret')}
          margin="normal"
        />
        <Typography component="div" variant="body1">
          <Box bgcolor="primary.main" color="primary.contrastText" p={2} mt={2} mb={2}>
            If you modify the JWT Secret, all users will be logged out.
          </Box>
        </Typography>
        <Button startIcon={<SaveIcon />} variant="contained" color="primary" className={classes.button} type="submit">
          Save
        </Button>
        <Button variant="contained" color="secondary" className={classes.button} onClick={loadData}>
          Reset
      	</Button>
      </ValidatorForm>
    );
  }

}

export default withAuthenticatedContext(withStyles(styles)(SecuritySettingsForm));
