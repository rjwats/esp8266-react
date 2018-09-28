import React from 'react';
import PropTypes from 'prop-types';

import { withStyles } from '@material-ui/core/styles';
import LinearProgress from '@material-ui/core/LinearProgress';
import Typography from '@material-ui/core/Typography';
import MenuItem from '@material-ui/core/MenuItem';
import { AudioLightColorMode } from './AudioLightColorMode';
import { AudioLightSpectrumMode } from './AudioLightSpectrumMode';
import { ValidatorForm, SelectValidator } from 'react-material-ui-form-validator';

import * as AudioLightModes from '../constants/AudioLightModes';

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

class AudioLightSettingsForm extends React.Component {

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

  selectFormComponent() {
    if (this.props.audioLightSettings) {
      const mode_id = this.props.audioLightSettings.mode_id;
      switch (mode_id) {
        case AudioLightModes.COLOR:
          return AudioLightColorMode;
        case AudioLightModes.SPECTRUM:
          return AudioLightSpectrumMode;
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

              <ValidatorForm onSubmit={onSubmit}>

                <SelectValidator
                  name="mode_id"
                  label="Mode"
                  value={audioLightSettings.mode_id}
                  className={classes.selectField}
                  onChange={this.props.handleChangeMode}>
                  <MenuItem value={AudioLightModes.OFF}>Off</MenuItem>
                  <MenuItem value={AudioLightModes.COLOR}>Single Color</MenuItem>
                </SelectValidator>

                <FormComponent
                  classes={classes}
                  audioLightSettings={audioLightSettings}
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

AudioLightSettingsForm.propTypes = {
  classes: PropTypes.object.isRequired,
  audioLightSettingsFetched: PropTypes.bool,
  audioLightSettings: PropTypes.object,
  errorMessage: PropTypes.string,
  onSubmit: PropTypes.func.isRequired,
  handleValueChange: PropTypes.func.isRequired,
  handleColorChange: PropTypes.func.isRequired,
  handleChange: PropTypes.func.isRequired,
  handleChangeMode: PropTypes.func.isRequired
};

export default withStyles(styles)(AudioLightSettingsForm);
