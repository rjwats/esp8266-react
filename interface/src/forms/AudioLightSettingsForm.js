import React from 'react';
import PropTypes from 'prop-types';

import { withStyles } from '@material-ui/core/styles';
import LinearProgress from '@material-ui/core/LinearProgress';
import Typography from '@material-ui/core/Typography';
import MenuItem from '@material-ui/core/MenuItem';
import FormLabel from '@material-ui/core/FormLabel';

import { AudioLightColorMode } from './AudioLightColorMode';
import { AudioLightSpectrumMode } from './AudioLightSpectrumMode';
import { AudioLightRainbowMode } from './AudioLightRainbowMode';
import { AudioLightLightningMode } from './AudioLightLightningMode';
import { AudioLightConfettiMode } from './AudioLightConfettiMode';
import { AudioLightFireMode } from './AudioLightFireMode';
import { ValidatorForm, SelectValidator } from 'react-material-ui-form-validator';

import * as AudioLightModes from '../constants/AudioLightModes';

const styles = theme => ({
  form: {
    "margin-top": theme.spacing.unit,
    "margin-bottom": theme.spacing.unit
  },
  formControl: {
    "margin-top": theme.spacing.unit,
    "margin-bottom": theme.spacing.unit*2
  },
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

class AudioLightSettingsForm extends React.Component {

  constructor(props) {
    super(props);
    this.handleCheckboxChange = this.handleCheckboxChange.bind(this);
    this.immediateColorChange = this.immediateColorChange.bind(this);
    this.immediateValueChange = this.immediateValueChange.bind(this);
    this.immediateChange = this.immediateChange.bind(this);
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

  handleCheckboxChange = fieldId => value =>
    this.props.handleCheckboxChange(fieldId, () => this.props.onSubmit(true, true))(value);

  selectFormComponent() {
    if (this.props.audioLightSettings) {
      const mode_id = this.props.audioLightSettings.mode_id;
      switch (mode_id) {
        case AudioLightModes.COLOR:
          return AudioLightColorMode;
        case AudioLightModes.SPECTRUM:
          return AudioLightSpectrumMode;
        case AudioLightModes.RAINBOW:
          return AudioLightRainbowMode;
        case AudioLightModes.LIGHTNING:
          return AudioLightLightningMode;
          case AudioLightModes.CONFETTI:
          return AudioLightConfettiMode;
          case AudioLightModes.FIRE:
          return AudioLightFireMode;                    
        default:
      }
    }
    return props => { return null };
  }

  render() {
    const FormComponent = this.selectFormComponent();
    const {
      classes,
      audioLightSettingsFetched,
      audioLightSettings,
      errorMessage,
      onSubmit
    } = this.props;

    return (
      <div>
        {
          !audioLightSettingsFetched ?

            <div className={classes.loadingSettings}>
              <LinearProgress className={classes.loadingSettingsDetails} />
              <Typography variant="display1" className={classes.loadingSettingsDetails}>
                Loading...
              </Typography>
            </div>

            : audioLightSettings ?

              <ValidatorForm className={classes.form} onSubmit={onSubmit}>

                <FormLabel>Mode</FormLabel>
                <div className={classes.formControl}>
                  <SelectValidator
                    name="mode_id"
                    value={audioLightSettings.mode_id}                   
                    onChange={this.props.handleChangeMode}>
                    <MenuItem value={AudioLightModes.OFF}>Off</MenuItem>
                    <MenuItem value={AudioLightModes.COLOR}>Single Color</MenuItem>
                    <MenuItem value={AudioLightModes.SPECTRUM}>Spectrum</MenuItem>
                    <MenuItem value={AudioLightModes.RAINBOW}>Rainbow</MenuItem>
                    <MenuItem value={AudioLightModes.LIGHTNING}>Lightning</MenuItem>
                    <MenuItem value={AudioLightModes.CONFETTI}>Confetti</MenuItem>
                    <MenuItem value={AudioLightModes.FIRE}>Fire</MenuItem>
                  </SelectValidator>
                </div>

                <FormComponent
                  classes={classes}
                  audioLightSettings={audioLightSettings}
                  handleValueChange={this.immediateValueChange}
                  handleCheckboxChange={this.handleCheckboxChange}
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

AudioLightSettingsForm.propTypes = {
  classes: PropTypes.object.isRequired,
  audioLightSettingsFetched: PropTypes.bool,
  audioLightSettings: PropTypes.object,
  errorMessage: PropTypes.string,
  onSubmit: PropTypes.func.isRequired,
  handleValueChange: PropTypes.func.isRequired,
  handleColorChange: PropTypes.func.isRequired,
  handleChange: PropTypes.func.isRequired,
  handleChangeMode: PropTypes.func.isRequired,
  handleCheckboxChange: PropTypes.func.isRequired
};

export default withStyles(styles)(AudioLightSettingsForm);
