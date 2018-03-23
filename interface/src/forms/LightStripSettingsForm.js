import React from 'react';
import PropTypes from 'prop-types';

import { withStyles } from 'material-ui/styles';
import Button from 'material-ui/Button';
import { LinearProgress } from 'material-ui/Progress';
import Typography from 'material-ui/Typography';
import { MenuItem } from 'material-ui/Menu';
import { LightStripColorMode } from './LightStripColorMode';
import { ValidatorForm, SelectValidator } from 'react-material-ui-form-validator';

import * as IRCodes from '../constants/IRCodes';

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
  }
});

class LightStripSettingsForm extends React.Component {

  selectFormComponent() {
    if (this.props.lightStripSettings){
      const modeCode = this.props.lightStripSettings.mode_code;
      switch (modeCode) {
        case IRCodes.IR_ON:
          return LightStripColorMode;
        default:
      }
    }
    return props => { return null };
  }

  render() {
    const FormComponent = this.selectFormComponent();
    const {
      classes,
      lightStripSettingsFetched,
      lightStripSettings,
      errorMessage,
      handleValueChange,
      handleColorChange,
      onSubmit,
      onReset
    } = this.props;

    return (
      <div>
        {
         !lightStripSettingsFetched ?

         <div className={classes.loadingSettings}>
           <LinearProgress className={classes.loadingSettingsDetails}/>
           <Typography variant="display1" className={classes.loadingSettingsDetails}>
             Loading...
           </Typography>
         </div>

         : lightStripSettings ?

        <ValidatorForm onSubmit={onSubmit}>

          <SelectValidator name="mode_code" label="Mode" value={lightStripSettings.mode_code}  className={classes.selectField}
           onChange={handleValueChange('mode_code', onSubmit)}>
            <MenuItem value={IRCodes.IR_OFF}>Off</MenuItem>
            <MenuItem value={IRCodes.IR_ON}>Single Color</MenuItem>
            <MenuItem value={IRCodes.IR_FLASH}>Dual Color</MenuItem>
            <MenuItem value={IRCodes.IR_STROBE}>Strobe</MenuItem>
            <MenuItem value={IRCodes.IR_FADE}>Pulsate</MenuItem>
            <MenuItem value={IRCodes.IR_SMOOTH}>Color Cycle</MenuItem>
          </SelectValidator>

          <FormComponent
            classes={classes}
            lightStripSettings={lightStripSettings}
            handleValueChange={handleValueChange}
            handleColorChange={handleColorChange}
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

LightStripSettingsForm.propTypes = {
  classes: PropTypes.object.isRequired,
  lightStripSettingsFetched: PropTypes.bool.isRequired,
  lightStripSettings: PropTypes.object,
  errorMessage: PropTypes.string,
  onSubmit: PropTypes.func.isRequired,
  onReset: PropTypes.func.isRequired,
  handleValueChange: PropTypes.func.isRequired,
  handleColorChange: PropTypes.func.isRequired
};

export default withStyles(styles)(LightStripSettingsForm);
