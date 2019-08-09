import React, { Component } from 'react';
import SectionContent from '../components/SectionContent';
import { restComponent } from '../components/RestComponent';
import LoadingNotification from '../components/LoadingNotification';

import Button from '@material-ui/core/Button';
import Typography from '@material-ui/core/Typography';
import Slider from '@material-ui/core/Slider';
import { makeStyles } from '@material-ui/core/styles';
import { ValidatorForm } from 'react-material-ui-form-validator';

export const DEMO_SETTINGS_ENDPOINT = process.env.REACT_APP_ENDPOINT_ROOT + "demoSettings";

const valueToPercentage = (value) => `${Math.round(value / 255 * 100)}%`;

class DemoController extends Component {
  componentDidMount() {
    this.props.loadData();
  }
  
  render() {
    const { data, fetched, errorMessage, saveData, loadData, handleValueChange } = this.props;
    return (
      <SectionContent title="Controller" titleGutter>
        <LoadingNotification
          onReset={loadData}
          fetched={fetched}
          errorMessage={errorMessage}>
          <DemoControllerForm
            demoSettings={data}
            onReset={loadData}
            onSubmit={saveData}
            handleValueChange={handleValueChange}
          />
        </LoadingNotification>
      </SectionContent>
    )
  }
}

const useStyles = makeStyles(theme => ({
  button: {
    marginRight: theme.spacing(2),
    marginTop: theme.spacing(2),
  },
  blinkSpeedLabel:{
    marginBottom: theme.spacing(5),
  }
}));
  
function DemoControllerForm(props) {
  const { demoSettings, onSubmit, onReset, handleValueChange } = props;
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
        onChange={handleValueChange('blink_speed')}
      />
      <Button variant="contained" color="primary" className={classes.button} type="submit">
        Save
      </Button>
      <Button variant="contained" color="secondary" className={classes.button} onClick={onReset}>
        Reset
      </Button>
    </ValidatorForm>
  );
}

export default restComponent(DEMO_SETTINGS_ENDPOINT, DemoController);
