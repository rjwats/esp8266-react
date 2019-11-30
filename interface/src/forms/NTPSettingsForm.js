import React from 'react';
import PropTypes from 'prop-types';
import { TextValidator, ValidatorForm } from 'react-material-ui-form-validator';

import { withStyles } from '@material-ui/core/styles';
import Button from '@material-ui/core/Button';
import SaveIcon from '@material-ui/icons/Save';

import isIP from '../validators/isIP';
import isHostname from '../validators/isHostname';
import or from '../validators/or';

const styles = theme => ({
  textField: {
    width: "100%"
  },
  button: {
    marginRight: theme.spacing(2),
    marginTop: theme.spacing(2),
  }
});

class NTPSettingsForm extends React.Component {

  componentWillMount() {
    ValidatorForm.addValidationRule('isIPOrHostname', or(isIP, isHostname));
  }

  render() {
    const { classes, ntpSettings, handleValueChange, onSubmit, onReset } = this.props;
    return (
      <ValidatorForm onSubmit={onSubmit}>
        <TextValidator
          validators={['required', 'isIPOrHostname']}
          errorMessages={['Server is required', "Not a valid IP address or hostname"]}
          name="server"
          label="Server"
          className={classes.textField}
          value={ntpSettings.server}
          onChange={handleValueChange('server')}
          margin="normal"
        />
        <TextValidator
          validators={['required', 'isNumber', 'minNumber:60', 'maxNumber:86400']}
          errorMessages={['Interval is required', 'Interval must be a number', 'Must be at least 60 seconds', "Must not be more than 86400 seconds (24 hours)"]}
          name="interval"
          label="Interval (Seconds)"
          className={classes.textField}
          value={ntpSettings.interval}
          type="number"
          onChange={handleValueChange('interval')}
          margin="normal"
        />
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

NTPSettingsForm.propTypes = {
  classes: PropTypes.object.isRequired,
  ntpSettings: PropTypes.object,
  onSubmit: PropTypes.func.isRequired,
  onReset: PropTypes.func.isRequired,
  handleValueChange: PropTypes.func.isRequired,
};

export default withStyles(styles)(NTPSettingsForm);
