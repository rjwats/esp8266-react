import React from 'react';
import PropTypes from 'prop-types';

import { withStyles } from '@material-ui/core/styles';
import Button from '@material-ui/core/Button';
import LinearProgress from '@material-ui/core/LinearProgress';
import Typography from '@material-ui/core/Typography';
import MenuItem from '@material-ui/core/MenuItem';
import Switch from '@material-ui/core/Switch';
import FormControlLabel from '@material-ui/core/FormControlLabel';
import { TextValidator, ValidatorForm, SelectValidator } from 'react-material-ui-form-validator';

const styles = theme => ({
  loadingSettings: {
    margin: theme.spacing.unit,
  },
  loadingSettingsDetails: {
    margin: theme.spacing.unit * 4,
    textAlign: "center"
  },
  switchControl: {
    width: "100%",
    marginTop: theme.spacing.unit * 2,
    marginBottom: theme.spacing.unit
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
  }
});

class ChamberSettingsForm extends React.Component {

  sensorMenuItem(sensorId, sensors) {
    return <MenuItem key={sensorId} value={sensorId}>{sensorId} - {sensors[sensorId].temp_c.toFixed(1)} &#x2103;</MenuItem>
  }

  createSensorMenuItems() {
    const { chamberStatus } = this.props;
    return (chamberStatus.sensors ? Object.keys(chamberStatus.sensors) : []).map(sensorId => this.sensorMenuItem(sensorId, chamberStatus.sensors));
  }

  sensorValue(sensorValue){
    const { chamberStatus } = this.props;
    return chamberStatus.sensors ? chamberStatus.sensors[sensorValue] ? sensorValue : "" : "";
  }

  render() {
    const {
      classes,
      chamberStatus, chamberSettings, fetched, errorMessage,
      handleValueChange, handleCheckboxChange, onSubmit, onReset
    } = this.props;
    return (
      <div>
        {
         !fetched ?

         <div className={classes.loadingSettings}>
           <LinearProgress className={classes.loadingSettingsDetails}/>
           <Typography variant="display1" className={classes.loadingSettingsDetails}>
             Loading...
           </Typography>
         </div>

         : chamberSettings && chamberStatus ?

        <ValidatorForm onSubmit={onSubmit}>

          <SelectValidator name="chamber_sensor_address" label="Chamber Sensor" value={this.sensorValue(chamberSettings.chamber_sensor_address)}  className={classes.selectField}
           onChange={handleValueChange('chamber_sensor_address')}>
            <MenuItem value="">
              <em>Select chamber sensor...</em>
            </MenuItem>
            {this.createSensorMenuItems()}
          </SelectValidator>

          <SelectValidator name="ambient_sensor_address" label="Ambient Sensor" value={this.sensorValue(chamberSettings.ambient_sensor_address)}  className={classes.selectField}
           onChange={handleValueChange('ambient_sensor_address')}>
            <MenuItem value="">
              <em>Select ambient sensor...</em>
            </MenuItem>
            {this.createSensorMenuItems()}
          </SelectValidator>

          <TextValidator
              validators={['required', 'isFloat', 'minNumber:0', 'maxNumber:25']}
              errorMessages={['Target temperature is required', "Must be a number", "Must be greater than 0 ", "Max value is 25"]}
              name="target_temp"
              label="Target temperature"
              className={classes.textField}
              value={chamberSettings.target_temp}
              type="number"
              onChange={handleValueChange('target_temp')}
              margin="normal"
            />

            <TextValidator
                validators={['required', 'isFloat', 'minNumber:0', 'maxNumber:5']}
                errorMessages={['Hysteresis high is required', "Must be a number", "Must be greater than 0 ", "Max value is 5"]}
                name="hysteresis_high"
                label="Hysteresis high"
                className={classes.textField}
                value={chamberSettings.hysteresis_high}
                type="number"
                onChange={handleValueChange('hysteresis_high')}
                margin="normal"
            />

            <TextValidator
                  validators={['required', 'isFloat', 'minNumber:0', 'maxNumber:5']}
                  errorMessages={['Hysteresis low is required', "Must be a number", "Must be greater than 0 ", "Max value is 5"]}
                  name="hysteresis_low"
                  label="Hysteresis low"
                  className={classes.textField}
                  value={chamberSettings.hysteresis_low}
                  type="number"
                  onChange={handleValueChange('hysteresis_low')}
                  margin="normal"
            />

            <TextValidator
                  validators={['required', 'isFloat', 'minNumber:0', 'maxNumber:1']}
                  errorMessages={['Hysteresis low is required', "Must be a number", "Must be a positive value", "Must be no greater than 1"]}
                  name="hysteresis_factor"
                  label="Hysteresis factor"
                  className={classes.textField}
                  value={chamberSettings.hysteresis_factor}
                  type="number"
                  onChange={handleValueChange('hysteresis_factor')}
                  margin="normal"
            />

            <FormControlLabel className={classes.switchControl}
               control={
                 <Switch
                        checked={chamberSettings.enable_heater}
                        onChange={handleCheckboxChange('enable_heater')}
                        value="enable_heater"
                        color="primary"
                 />
               }
              label="Enable Heater?"
             />

            <TextValidator
                validators={['required', 'isNumber', 'minNumber:0', 'maxNumber:3600000']}
                errorMessages={['Min heater on duration is required', "Must be a number", "Must be greater than 0 ", "Max value is 3600000"]}
                name="min_heater_on_duration"
                label="Min heater on duration"
                className={classes.textField}
                value={chamberSettings.min_heater_on_duration}
                type="number"
                onChange={handleValueChange('min_heater_on_duration')}
                margin="normal"
            />

            <TextValidator
                validators={['required', 'isNumber', 'minNumber:0', 'maxNumber:3600000']}
                errorMessages={['Min heater off duration is required', "Must be a number", "Must be greater than 0 ", "Max value is 3600000"]}
                name="min_heater_off_duration"
                label="Min heater off duration"
                className={classes.textField}
                value={chamberSettings.min_heater_off_duration}
                type="number"
                onChange={handleValueChange('min_heater_off_duration')}
                margin="normal"
            />

            <FormControlLabel className={classes.switchControl}
               control={
                 <Switch
                        checked={chamberSettings.enable_cooler}
                        onChange={handleCheckboxChange('enable_cooler')}
                        value="enable_cooler"
                        color="primary"
                 />
               }
              label="Enable Cooler?"
             />

            <TextValidator
                validators={['required', 'isNumber', 'minNumber:0', 'maxNumber:3600000']}
                errorMessages={['Min cooler on duration is required', "Must be a number", "Must be greater than 0 ", "Max value is 3600000"]}
                name="min_cooler_on_duration"
                label="Min cooler on duration"
                className={classes.textField}
                value={chamberSettings.min_cooler_on_duration}
                type="number"
                onChange={handleValueChange('min_cooler_on_duration')}
                margin="normal"
            />

            <TextValidator
                validators={['required', 'isNumber', 'minNumber:0', 'maxNumber:3600000']}
                errorMessages={['Min cooler off duration is required', "Must be a number", "Must be greater than 0 ", "Max value is 3600000"]}
                name="min_cooler_off_duration"
                label="Min cooler off duration"
                className={classes.textField}
                value={chamberSettings.min_cooler_off_duration}
                type="number"
                onChange={handleValueChange('min_cooler_off_duration')}
                margin="normal"
            />

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

ChamberSettingsForm.propTypes = {
  classes: PropTypes.object.isRequired,
  chamberSettings: PropTypes.object,
  chamberStatus: PropTypes.object,
  fetched: PropTypes.bool.isRequired,
  errorMessage: PropTypes.string,
  onSubmit: PropTypes.func.isRequired,
  onReset: PropTypes.func.isRequired,
  handleValueChange: PropTypes.func.isRequired,
  handleCheckboxChange: PropTypes.func.isRequired
};

export default withStyles(styles)(ChamberSettingsForm);
