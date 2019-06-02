import React from 'react';
import PropTypes from 'prop-types';

import { withStyles } from '@material-ui/core/styles';
import Button from '@material-ui/core/Button';
import Switch from '@material-ui/core/Switch';
import LinearProgress from '@material-ui/core/LinearProgress';
import { TextValidator, ValidatorForm } from 'react-material-ui-form-validator';
import Typography from '@material-ui/core/Typography';
import FormControlLabel from '@material-ui/core/FormControlLabel';

import isIP from '../validators/isIP';
import isHostname from '../validators/isHostname';
import or from '../validators/or';
import PasswordValidator from '../components/PasswordValidator';

const styles = theme => ({
  loadingSettings: {
    margin: theme.spacing(0.5),
  },
  loadingSettingsDetails: {
    margin: theme.spacing(4),
    textAlign: "center"
  },
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

class OTASettingsForm extends React.Component {

  componentWillMount() {
    ValidatorForm.addValidationRule('isIPOrHostname', or(isIP, isHostname));
  }

  render() {
    const { classes, otaSettingsFetched, otaSettings, errorMessage, handleValueChange, handleCheckboxChange, onSubmit, onReset } = this.props;
    return (
      <div>
        {
         !otaSettingsFetched ?

         <div className={classes.loadingSettings}>
           <LinearProgress className={classes.loadingSettingsDetails}/>
           <Typography variant="h4" className={classes.loadingSettingsDetails}>
             Loading...
           </Typography>
         </div>

         : otaSettings ?

      	 <ValidatorForm onSubmit={onSubmit}>

            <FormControlLabel className={classes.switchControl}
               control={
                 <Switch
                        checked={otaSettings.enabled}
                        onChange={handleCheckboxChange('enabled')}
                        value="enabled"
                        color="primary"
                 />
               }
              label="Enable OTA Updates?"
             />

           <TextValidator
               validators={['required', 'isNumber', 'minNumber:1025', 'maxNumber:65535']}
               errorMessages={['Port is required', "Must be a number", "Must be greater than 1024 ", "Max value is 65535"]}
               name="port"
               label="Port"
               className={classes.textField}
               value={otaSettings.port}
               type="number"
               onChange={handleValueChange('port')}
               margin="normal"
             />

             <PasswordValidator
                   validators={['required', 'matchRegexp:^.{1,64}$']}
                   errorMessages={['OTA Password is required', 'OTA Point Password must be 64 characters or less']}
                   name="password"
                   label="Password"
                   className={classes.textField}
                   value={otaSettings.password}
                   onChange={handleValueChange('password')}
                   margin="normal"
             />

          <Button variant="contained" color="primary" className={classes.button} type="submit">
            Save
          </Button>
          <Button variant="contained" color="secondary" className={classes.button} onClick={onReset}>
      		  Reset
      		</Button>

         </ValidatorForm>

        :

        <div className={classes.loadingSettings}>
          <Typography variant="h4" className={classes.loadingSettingsDetails}>
            {errorMessage}
          </Typography>
          <Button variant="contained" color="secondary" className={classes.button} onClick={onReset}>
      		  Reset
      		</Button>
        </div>
      }
      </div>
    );
  }
}

OTASettingsForm.propTypes = {
  classes: PropTypes.object.isRequired,
  otaSettingsFetched: PropTypes.bool.isRequired,
  otaSettings: PropTypes.object,
  errorMessage: PropTypes.string,
  onSubmit: PropTypes.func.isRequired,
  onReset: PropTypes.func.isRequired,
  handleValueChange: PropTypes.func.isRequired,
  handleCheckboxChange: PropTypes.func.isRequired,
};

export default withStyles(styles)(OTASettingsForm);
