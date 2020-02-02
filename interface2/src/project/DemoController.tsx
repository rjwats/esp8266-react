import React, { Component } from 'react';
import { ValidatorForm } from 'react-material-ui-form-validator';

import { Typography, Slider, Box } from '@material-ui/core';
import SaveIcon from '@material-ui/icons/Save';

import { restController, RestControllerProps, RestFormLoader, RestFormProps, FormActions, FormButton, SectionContent } from '../components';
import { ENDPOINT_ROOT } from '../constants/Env';

export const DEMO_SETTINGS_ENDPOINT = ENDPOINT_ROOT + "demoSettings";

interface DemoSettings {
  blink_speed: number;
}

type DemoControllerProps = RestControllerProps<DemoSettings>;

class DemoController extends Component<DemoControllerProps> {

  componentDidMount() {
    this.props.loadData();
  }

  render() {
    return (
      <SectionContent title='Demo Controller' titleGutter>
        <RestFormLoader
          {...this.props}
          render={props => (
            <DemoControllerForm {...props} />
          )}
        />
      </SectionContent>
    )
  }

}

export default restController(DEMO_SETTINGS_ENDPOINT, DemoController);

const valueToPercentage = (value: number) => `${Math.round(value / 255 * 100)}%`;

type DemoControllerFormProps = RestFormProps<DemoSettings>;

function DemoControllerForm(props: DemoControllerFormProps) {
  const { data, saveData, loadData, handleSliderChange } = props;
  return (
    <ValidatorForm onSubmit={saveData}>
      <Typography id="blink-speed-slider">
        Blink Speed
      </Typography>
      <Box pt={5}>
        <Slider
          value={data.blink_speed}
          valueLabelFormat={valueToPercentage}
          aria-labelledby="blink-speed-slider"
          valueLabelDisplay="on"
          min={0}
          max={255}
          onChange={handleSliderChange('blink_speed')}
        />
      </Box>
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


