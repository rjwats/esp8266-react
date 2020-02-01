import React from 'react';
import { TextValidator, ValidatorForm, SelectValidator } from 'react-material-ui-form-validator';

import SaveIcon from '@material-ui/icons/Save';

import isIP from '../validators/isIP';
import isHostname from '../validators/isHostname';
import or from '../validators/or';
import { RestFormProps } from '../components/RestFormLoader';
import { NTPSettingsData } from '../containers/NTPSettings';
import FormActions from '../components/FormActions';
import FormButton from '../components/FormButton';
import BlockFormControlLabel from '../components/BlockFormControlLabel';
import { Checkbox, MenuItem } from '@material-ui/core';
import { TIME_ZONES, timeZoneSelectItems, selectedTimeZone } from '../constants/TZ';

type NTPSettingsFormProps = RestFormProps<NTPSettingsData>;

class NTPSettingsForm extends React.Component<NTPSettingsFormProps> {

  componentDidMount() {
    ValidatorForm.addValidationRule('isIPOrHostname', or(isIP, isHostname));
  }

  changeTimeZone = (event: React.ChangeEvent<HTMLSelectElement>) => {
    const { data, setData } = this.props;
    setData({
      ...data,
      tz_label: event.target.value,
      tz_format: TIME_ZONES[event.target.value]
    });
  }

  render() {
    const { data, handleValueChange, handleCheckboxChange, saveData, loadData } = this.props;
    return (
      <ValidatorForm onSubmit={saveData}>
        <BlockFormControlLabel
          control={
            <Checkbox
              checked={data.enabled}
              onChange={handleCheckboxChange('enabled')}
              value="enabled"
              color="primary"
            />
          }
          label="Enable NTP?"
        />
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
        <SelectValidator
          validators={['required']}
          errorMessages={['Time zone is required']}
          name="tz_label"
          labelId="tz_label"
          label="Time zone"
          fullWidth
          variant="outlined"
          native
          value={selectedTimeZone(data.tz_label, data.tz_format)}
          onChange={this.changeTimeZone}
          margin="normal"
        >
          <MenuItem disabled={true}>Time zone...</MenuItem>
          {timeZoneSelectItems()}
        </SelectValidator>
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
