import React from 'react';
import PropTypes from 'prop-types';
import { ValidatorForm } from 'react-material-ui-form-validator';

import { withStyles } from '@material-ui/core/styles';
import Button from '@material-ui/core/Button';
import Typography from '@material-ui/core/Typography';
import Box from '@material-ui/core/Box';
import SaveIcon from '@material-ui/icons/Save';

import PasswordValidator from '../components/PasswordValidator';
import { withAuthenticationContext } from '../authentication/Context';

const styles = theme => ({
  textField: {
    width: "100%"
  },
  button: {
    marginRight: theme.spacing(2),
    marginTop: theme.spacing(2),
  }
});

class SecuritySettingsForm extends React.Component {

  onSubmit = () => {
    this.props.onSubmit();
    this.props.authenticationContext.refresh();
  }

  render() {
    const { classes, securitySettings, handleValueChange, onReset } = this.props;
    return (
      <ValidatorForm onSubmit={this.onSubmit} ref="SecuritySettingsForm">
        <PasswordValidator
          validators={['required', 'matchRegexp:^.{1,64}$']}
          errorMessages={['JWT Secret Required', 'JWT Secret must be 64 characters or less']}
          name="jwt_secret"
          label="JWT Secret"
          className={classes.textField}
          value={securitySettings.jwt_secret}
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
        <Button variant="contained" color="secondary" className={classes.button} onClick={onReset}>
          Reset
      	</Button>
      </ValidatorForm>
    );
  }
}

SecuritySettingsForm.propTypes = {
  classes: PropTypes.object.isRequired,
  securitySettings: PropTypes.object,
  onSubmit: PropTypes.func.isRequired,
  onReset: PropTypes.func.isRequired,
  handleValueChange: PropTypes.func.isRequired,
  authenticationContext: PropTypes.object.isRequired,
};

export default withAuthenticationContext(withStyles(styles)(SecuritySettingsForm));
