import React from 'react';
import { TextValidator, ValidatorForm } from 'react-material-ui-form-validator';

import SaveIcon from '@material-ui/icons/Save';

import isIP from '../validators/isIP';
import isHostname from '../validators/isHostname';
import or from '../validators/or';
import { RestFormProps } from '../components/RestFormLoader';
import { NTPSettingsData } from '../containers/NTPSettings';
import FormActions from '../components/FormActions';
import FormButton from '../components/FormButton';

type NTPSettingsFormProps = RestFormProps<NTPSettingsData>;

class NTPSettingsForm extends React.Component<NTPSettingsFormProps> {

  componentDidMount() {
    ValidatorForm.addValidationRule('isIPOrHostname', or(isIP, isHostname));
  }

  render() {
    const { data, handleValueChange, saveData, loadData } = this.props;
    return (
      <ValidatorForm onSubmit={saveData}>
        <TextValidator
          validators={['required', 'isIPOrHostname']}
          errorMessages={['Server is required', "Not a valid IP address or hostname"]}
          name="server"
          label="Server"
          fullWidth
          variant="outlined"
          value={data.server}
          onChange={handleValueChange('server')}
          margin="normal"
        />
        <TextValidator
          validators={['required', 'isNumber', 'minNumber:60', 'maxNumber:86400']}
          errorMessages={['Interval is required', 'Interval must be a number', 'Must be at least 60 seconds', "Must not be more than 86400 seconds (24 hours)"]}
          name="interval"
          label="Interval (Seconds)"
          fullWidth
          variant="outlined"
          value={data.interval}
          type="number"
          onChange={handleValueChange('interval')}
          margin="normal"
        />
        <FormActions>
          <FormButton startIcon={<SaveIcon />} variant="contained" color="primary" type="submit">
            Save
          </FormButton>
          <FormButton variant="contained" color="secondary" onClick={loadData}>
            Reset
          </FormButton>
        </FormActions>
      </ValidatorForm>
    );
  }
}

export default NTPSettingsForm;
