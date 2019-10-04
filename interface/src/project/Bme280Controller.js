import React, { Component } from 'react';

import { ENDPOINT_ROOT } from '../constants/Env';
import SectionContent from '../components/SectionContent';
import { restComponent } from '../components/RestComponent';
import LoadingNotification from '../components/LoadingNotification';

import { makeStyles } from '@material-ui/core/styles';

import Table from '@material-ui/core/Table';
import TableHead from '@material-ui/core/TableHead';
import TableCell from '@material-ui/core/TableCell';
import TableBody from '@material-ui/core/TableBody';
import TableRow from '@material-ui/core/TableRow';

export const BME280_SETTINGS_ENDPOINT = ENDPOINT_ROOT + "bme280";

class Bme280Controller extends Component {
  interval = null;

  componentDidMount() {
    this.props.loadData();

    this.interval = setInterval(() => {
      this.props.loadData();
    }, 5000);
  }

  componentWillUnmount() {
    if (this.interval) {
      clearInterval(this.interval);
    }
  }

  render() {
    const { data, fetched, errorMessage, loadData } = this.props;
    return (
      <SectionContent title="Sensors" titleGutter>
        <LoadingNotification
          onReset={loadData}
          fetched={fetched}
          errorMessage={errorMessage}
          render={() =>
            <Bme280ControllerForm
              data={data}
            />
          }
        />
      </SectionContent>
    )
  }
}

const useStyles = makeStyles(theme => ({
  button: {
    marginRight: theme.spacing(2),
    marginTop: theme.spacing(2),
  },
  blinkSpeedLabel: {
    marginBottom: theme.spacing(5),
  }
}));

function Bme280ControllerForm(props) {
  const { data } = props;
  const classes = useStyles();
  return (
    <Table className={classes.fileTable}>
        <TableHead>
          <TableRow>
            <TableCell>
              Sensor
            </TableCell>
            <TableCell>
              Value
            </TableCell>
          </TableRow>
        </TableHead>
        <TableBody>
          <TableRow>
            <TableCell>
              Temperature
            </TableCell>
            <TableCell>
              { data.temperature.toFixed(2) }°C
            </TableCell>
          </TableRow>
          <TableRow>
            <TableCell>
              Humidity
            </TableCell>
            <TableCell>
              { data.humidity.toFixed(2) }%
            </TableCell>
          </TableRow>
          <TableRow>
            <TableCell>
              Pressure
            </TableCell>
            <TableCell>
              { data.pressure.toFixed(2) }hPa
            </TableCell>
          </TableRow>
          <TableRow>
            <TableCell>
              Altitude
            </TableCell>
            <TableCell>
              { data.altitude.toFixed(2) }m
            </TableCell>
          </TableRow>
        </TableBody>
      </Table>
  );
}

export default restComponent(BME280_SETTINGS_ENDPOINT, Bme280Controller);
