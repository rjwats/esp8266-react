import React, { Fragment } from 'react';
import PropTypes from 'prop-types';

import { withStyles } from '@material-ui/core/styles';
import Button from '@material-ui/core/Button';
import LinearProgress from '@material-ui/core/LinearProgress';
import Typography from '@material-ui/core/Typography';
import MenuItem from '@material-ui/core/MenuItem';

import { TextValidator, ValidatorForm, SelectValidator } from 'react-material-ui-form-validator';
import { InputAdornment } from '@material-ui/core';
import { RemoveRedEye } from '@material-ui/icons';

import {isAPEnabled} from '../constants/WiFiAPModes';

const styles = theme => ({
  loadingSettings: {
    margin: theme.spacing.unit,
  },
  loadingSettingsDetails: {
    margin: theme.spacing.unit * 4,
    textAlign: "center"
  },
  textField: {
    width: "100%"
  },
  selectField:{
    width: "100%",
    marginTop: theme.spacing.unit * 2,
    marginBottom: theme.spacing.unit
  },
  button: {
    marginRight: theme.spacing.unit * 2,
    marginTop: theme.spacing.unit * 2,
  },
  eye: {
    cursor: 'pointer',
  }
});

class APSettingsForm extends React.Component {

  constructor(props) {
    super(props);
    this.state = {passwordIsMasked: true};
  }
  
  togglePasswordMask = () => {
    this.setState(prevState => ({passwordIsMasked: !prevState.passwordIsMasked}));
  }

  render() {
    const { classes, apSettingsFetched, apSettings, errorMessage, handleValueChange, onSubmit, onReset } = this.props;
    const { passwordIsMasked } = this.state;
    return (
      <div>
        {
         !apSettingsFetched ?

         <div className={classes.loadingSettings}>
           <LinearProgress className={classes.loadingSettingsDetails}/>
           <Typography variant="display1" className={classes.loadingSettingsDetails}>
             Loading...
           </Typography>
         </div>

         : apSettings ?

        <ValidatorForm onSubmit={onSubmit} ref="APSettingsForm">

          <SelectValidator name="provision_mode" label="Provide Access Point..." value={apSettings.provision_mode}  className={classes.selectField}
           onChange={handleValueChange('provision_mode')}>
            <MenuItem value={0}>Always</MenuItem>
            <MenuItem value={1}>When WiFi Disconnected</MenuItem>
            <MenuItem value={2}>Never</MenuItem>
          </SelectValidator>

          {
            isAPEnabled(apSettings.provision_mode) &&
            <Fragment>
              <TextValidator
                validators={['required', 'matchRegexp:^.{0,32}$']}
                errorMessages={['Access Point SSID is required', 'Access Point SSID must be 32 characeters or less']}
                name="ssid"
                label="Access Point SSID"
                className={classes.textField}
                value={apSettings.ssid}
                onChange={handleValueChange('ssid')}
                margin="normal"
              />
              <TextValidator
                    validators={['required', 'matchRegexp:^.{0,64}$']}
                    errorMessages={['Access Point Password is required', 'Access Point Password must be 64 characters or less']}
                    name="password"
                    label="Access Point Password"
                    className={classes.textField}
                    value={apSettings.password}
                    onChange={handleValueChange('password')}
                    margin="normal"
                    type={passwordIsMasked ? 'password' : 'text'}
                    InputProps={{
                      endAdornment: (
                        <InputAdornment position="end">
                          <RemoveRedEye
                            className={classes.eye}
                            onClick={this.togglePasswordMask}
                          />
                        </InputAdornment>
                      ),
                    }}
              />
            </Fragment>
          }

          <Button variant="raised" color="primary" className={classes.button} type="submit">
            Save
          </Button>
          <Button variant="raised" color="secondary" className={classes.button} onClick={onReset}>
      		  Reset
      		</Button>

        </ValidatorForm>

        :

        <div className={classes.loadingSettings}>
          <Typography variant="display1" className={classes.loadingSettingsDetails}>
            {errorMessage}
          </Typography>
          <Button variant="raised" color="secondary" className={classes.button} onClick={onReset}>
      		  Reset
      		</Button>
        </div>
      }
      </div>
    );
  }
}

APSettingsForm.propTypes = {
  classes: PropTypes.object.isRequired,
  apSettingsFetched: PropTypes.bool.isRequired,
  apSettings: PropTypes.object,
  errorMessage: PropTypes.string,
  onSubmit: PropTypes.func.isRequired,
  onReset: PropTypes.func.isRequired,
  handleValueChange: PropTypes.func.isRequired
};

export default withStyles(styles)(APSettingsForm);
