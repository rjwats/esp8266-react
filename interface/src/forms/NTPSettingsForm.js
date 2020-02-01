import React from 'react';
import PropTypes from 'prop-types';
import { TextValidator, ValidatorForm, SelectValidator } from 'react-material-ui-form-validator';

import { withStyles } from '@material-ui/core/styles';
import FormControlLabel from '@material-ui/core/FormControlLabel';
import MenuItem from '@material-ui/core/MenuItem';
import Switch from '@material-ui/core/Switch';
import Button from '@material-ui/core/Button';
import SaveIcon from '@material-ui/icons/Save';

import isIP from '../validators/isIP';
import isHostname from '../validators/isHostname';
import or from '../validators/or';
import { timeZoneSelectItems, selectedTimeZone, TIME_ZONES } from '../constants/TZ';

const styles = theme => ({
  switchControl: {
    width: "100%",
    marginTop: theme.spacing(2),
    marginBottom: theme.spacing(0.5)
  },
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

  changeTimeZone = (event) => {
    const { ntpSettings, setData } = this.props;
    setData({
      ...ntpSettings,
      tz_label: event.target.value,
      tz_format: TIME_ZONES[event.target.value]
    });
  }

  render() {
    const { classes, ntpSettings, handleValueChange, handleCheckboxChange, onSubmit, onReset } = this.props;
    return (
      <ValidatorForm onSubmit={onSubmit}>
        <FormControlLabel className={classes.switchControl}
          control={
            <Switch
              checked={ntpSettings.enabled}
              onChange={handleCheckboxChange('enabled')}
              value="enabled"
              color="primary"
            />
          }
          label="Enable NTP?"
        />        
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
        <SelectValidator
          native
          validators={['required']}
          errorMessages={['Time zone is required']}
          labelId="tz_label"
          label="Time zone"
          value={selectedTimeZone(ntpSettings.tz_label, ntpSettings.tz_format)}
          onChange={this.changeTimeZone}
          className={classes.textField}
          margin="normal"
        >
          <MenuItem disabled={true}>Time zone...</MenuItem>
          {timeZoneSelectItems()}
        </SelectValidator>
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
