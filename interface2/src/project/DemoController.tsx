import React, { Component } from 'react';
import { ValidatorForm } from 'react-material-ui-form-validator';

import { makeStyles, createStyles, Theme } from '@material-ui/core/styles';
import { Button, Typography, Slider } from '@material-ui/core';
import SaveIcon from '@material-ui/icons/Save';

import { ENDPOINT_ROOT } from '../constants/Env';
import SectionContent from '../components/SectionContent';
import RestFormLoader, { RestFormProps } from '../components/RestFormLoader';
import { RestControllerProps, restController } from '../components/RestController';

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
      <SectionContent title="Controller" titleGutter>
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

const useStyles = makeStyles((theme: Theme) =>
  createStyles({
    button: {
      marginRight: theme.spacing(2),
      marginTop: theme.spacing(2),
    },
    blinkSpeedLabel: {
      marginBottom: theme.spacing(5),
    }
  })
);

const valueToPercentage = (value: number) => `${Math.round(value / 255 * 100)}%`;

type DemoControllerFormProps = RestFormProps<DemoSettings>;

function DemoControllerForm(props: DemoControllerFormProps) {
  const { data, saveData, loadData, handleSliderChange } = props;
  const classes = useStyles();
  return (
    <ValidatorForm onSubmit={saveData}>
      <Typography id="blink-speed-slider" className={classes.blinkSpeedLabel}>
        Blink Speed
      </Typography>
      <Slider
        value={data.blink_speed}
        valueLabelFormat={valueToPercentage}
        aria-labelledby="blink-speed-slider"
        valueLabelDisplay="on"
        min={0}
        max={255}
        onChange={handleSliderChange('blink_speed')}
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


