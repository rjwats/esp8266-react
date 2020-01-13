import React, { Fragment } from 'react';
import { TextValidator, ValidatorForm, SelectValidator } from 'react-material-ui-form-validator';

import { isAPEnabled } from '../constants/WiFiAPModes';
import PasswordValidator from '../components/PasswordValidator';

import { withStyles, Theme, createStyles, WithStyles } from '@material-ui/core/styles';
import Button from '@material-ui/core/Button';
import MenuItem from '@material-ui/core/MenuItem';
import SaveIcon from '@material-ui/icons/Save';
import { RestFormProps } from '../components/RestFormLoader';
import { APSettingsData } from '../containers/APSettings';

const styles = (theme: Theme) => createStyles({
  button: {
    marginRight: theme.spacing(2),
    marginTop: theme.spacing(2),
  }
});

type APSettingsFormProps = RestFormProps<APSettingsData> & WithStyles<typeof styles>;

class APSettingsForm extends React.Component<APSettingsFormProps> {

  render() {
    const { classes, data, handleValueChange, saveData, loadData } = this.props;
    return (
      <ValidatorForm onSubmit={saveData} ref="APSettingsForm">
        <SelectValidator name="provision_mode"
          label="Provide Access Point..."
          value={data.provision_mode}
          fullWidth
          variant="outlined"
          onChange={handleValueChange('provision_mode')}>
          <MenuItem value={0}>Always</MenuItem>
          <MenuItem value={1}>When WiFi Disconnected</MenuItem>
          <MenuItem value={2}>Never</MenuItem>
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

export default withStyles(styles)(APSettingsForm);
