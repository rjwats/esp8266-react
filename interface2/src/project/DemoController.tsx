import React, { Component } from 'react';
import { ValidatorForm } from 'react-material-ui-form-validator';

import { makeStyles, createStyles, Theme } from '@material-ui/core/styles';
import { Button, Typography, Slider } from '@material-ui/core';
import SaveIcon from '@material-ui/icons/Save';

import { ENDPOINT_ROOT } from '../constants/Env';
import SectionContent from '../components/SectionContent';
import { restComponent, RestComponentProps } from '../components/RestComponent';
import LoadingNotification from '../components/LoadingNotification';

export const DEMO_SETTINGS_ENDPOINT = ENDPOINT_ROOT + "demoSettings";

const valueToPercentage = (value: number) => `${Math.round(value / 255 * 100)}%`;

interface DemoSettings {
  blink_speed: number;
}

class DemoController extends Component<RestComponentProps<DemoSettings>> {

  componentDidMount() {
    this.props.loadData();
  }

  render() {
    const { data, fetched, errorMessage, saveData, loadData, handleSliderChange } = this.props;
    return (
      <SectionContent title="Controller" titleGutter>
        <LoadingNotification
          onReset={loadData}
          fetched={fetched}
          errorMessage={errorMessage}
          render={() =>
            <DemoControllerForm
              demoSettings={data!}
              onReset={loadData}
              onSubmit={saveData}
              handleSliderChange={handleSliderChange}
            />
          }
        />
      </SectionContent>
    )
  }
}

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

interface DemoControllerFormProps {
  demoSettings: DemoSettings;
  onSubmit: () => void;
  onReset: () => void;
  handleSliderChange: (name: keyof DemoSettings) => (event: React.ChangeEvent<{}>, value: number | number[]) => void;
}

function DemoControllerForm(props: DemoControllerFormProps) {
  const { demoSettings, onSubmit, onReset, handleSliderChange } = props;
  const classes = useStyles();
  return (
    <ValidatorForm onSubmit={onSubmit}>
      <Typography id="blink-speed-slider" className={classes.blinkSpeedLabel}>
        Blink Speed
      </Typography>
      <Slider
        value={demoSettings.blink_speed}
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
      <Button variant="contained" color="secondary" className={classes.button} onClick={onReset}>
        Reset
      </Button>
    </ValidatorForm>
  );
}

export default restComponent(DEMO_SETTINGS_ENDPOINT, DemoController);
