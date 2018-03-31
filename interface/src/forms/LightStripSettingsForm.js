import React from 'react';
import PropTypes from 'prop-types';

import { withStyles } from 'material-ui/styles';
import { LinearProgress } from 'material-ui/Progress';
import Typography from 'material-ui/Typography';
import { MenuItem } from 'material-ui/Menu';
import { LightStripColorMode } from './LightStripColorMode';
import { LightStripStrobeMode } from './LightStripStrobeMode';
import { LightStripColorCycleMode } from './LightStripColorCycleMode';
import { LightStripPulsateMode } from './LightStripPulsateMode';
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
  }
});

class LightStripSettingsForm extends React.Component {


  constructor(props) {
    super(props);
    this.immediateValueChange = this.immediateValueChange.bind(this);
    this.immediateColorChange = this.immediateColorChange.bind(this);
  }

  /*
  * Saves the settings and submits it right away, works well for our
  * interactive light control!
  */
  immediateColorChange = fieldId => value =>
    this.props.handleColorChange(fieldId, () => this.props.onSubmit(true, true))(value);

  immediateValueChange = fieldId => value =>
    this.props.handleValueChange(fieldId, () => this.props.onSubmit(true, true))(value);

  immediateChange = fieldId => value =>
    this.props.handleChange(fieldId, () => this.props.onSubmit(true, true))(value);

  /*
  * Saves the settings and submits it right away, works well for our
  * interactive light control!

  immediateColorChange = (fieldId) => (value) =>
    this.props.handleColorChange(fieldId)(value, () => this.props.onSubmit(false));
  */

  selectFormComponent() {
    if (this.props.lightStripSettings){
      const modeCode = this.props.lightStripSettings.mode_code;
      switch (modeCode) {
        case IRCodes.IR_ON:
          return LightStripColorMode;
        case IRCodes.IR_STROBE:
          return LightStripStrobeMode;
        case IRCodes.IR_SMOOTH:
          return LightStripColorCycleMode;
        case IRCodes.IR_FADE:
          return LightStripPulsateMode;
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
      onSubmit
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
           onChange={this.props.handleChangeMode}>
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
            handleValueChange={this.immediateValueChange}
            handleColorChange={this.immediateColorChange}
            handleChange={this.immediateChange}
            onSubmit={onSubmit}
          />

        </ValidatorForm>

        :

        <div className={classes.loadingSettings}>
          <Typography variant="display1" className={classes.loadingSettingsDetails}>
            {errorMessage}
          </Typography>
        </div>
      }
      </div>
    );
  }
}

LightStripSettingsForm.propTypes = {
  classes: PropTypes.object.isRequired,
  lightStripSettingsFetched: PropTypes.bool,
  lightStripSettings: PropTypes.object,
  errorMessage: PropTypes.string,
  onSubmit: PropTypes.func.isRequired,
  handleValueChange: PropTypes.func.isRequired,
  handleColorChange: PropTypes.func.isRequired,
  handleChange: PropTypes.func.isRequired,
  handleChangeMode: PropTypes.func.isRequired
};

export default withStyles(styles)(LightStripSettingsForm);
