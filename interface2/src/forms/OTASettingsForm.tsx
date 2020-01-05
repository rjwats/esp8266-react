import React from 'react';
import { TextValidator, ValidatorForm } from 'react-material-ui-form-validator';

import { withStyles, Theme, createStyles, WithStyles } from '@material-ui/core/styles';
import { Button, Switch, FormControlLabel } from '@material-ui/core';
import SaveIcon from '@material-ui/icons/Save';

import PasswordValidator from '../components/PasswordValidator';
import isIP from '../validators/isIP';
import isHostname from '../validators/isHostname';
import or from '../validators/or';
import { OTASettingsData } from '../containers/OTASettings';
import { RestFormProps } from '../components/RestFormLoader';

const styles = (theme: Theme) => createStyles({
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

type OTASettingsFormProps = RestFormProps<OTASettingsData> & WithStyles<typeof styles>;

class OTASettingsForm extends React.Component<OTASettingsFormProps> {

  componentWillMount() {
    ValidatorForm.addValidationRule('isIPOrHostname', or(isIP, isHostname));
  }

  render() {
    const { classes, data, handleValueChange, handleCheckboxChange, saveData, loadData } = this.props;
    return (
      <ValidatorForm onSubmit={saveData}>
        <FormControlLabel className={classes.switchControl}
          control={
            <Switch
              checked={data.enabled}
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
          value={data.port}
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
          value={data.password}
          onChange={handleValueChange('password')}
          margin="normal"
        />
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

export default withStyles(styles)(OTASettingsForm);
