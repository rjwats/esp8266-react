import React from 'react';
import PropTypes from 'prop-types';
import { TextValidator, ValidatorForm, SelectValidator } from 'react-material-ui-form-validator';

import { withStyles } from '@material-ui/core/styles';
import Button from '@material-ui/core/Button';
import SaveIcon from '@material-ui/icons/Save';

import isIP from '../validators/isIP';
import isHostname from '../validators/isHostname';
import or from '../validators/or';
import { timeZoneSelectItems, TIME_ZONES } from '../constants/TZ';

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

  changeTimeZone = (event) => {
    const { ntpSettings, setData } = this.props;
    setData({
      ...ntpSettings,
      tz_label: event.target.value,
      tz_format: TIME_ZONES[event.target.value]
    });
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
        <SelectValidator
          native
          validators={['required']}
          errorMessages={['Timezone is required']}
          labelId="tz_label"
          value={ntpSettings.tz_label}
          onChange={this.changeTimeZone}
          className={classes.textField}
          margin="normal"
        >
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
