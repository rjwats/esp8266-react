import React, { Component } from 'react';

import {Line} from 'react-chartjs-2';

import { withStyles } from '@material-ui/core/styles';
import Button from '@material-ui/core/Button';
import LinearProgress from '@material-ui/core/LinearProgress';
import Typography from '@material-ui/core/Typography';
import Grid from '@material-ui/core/Grid';
import {restComponent} from '../components/RestComponent';
import SectionContent from '../components/SectionContent'
import { LOG_DATA_ENDPOINT }  from  '../constants/Endpoints';

import moment from 'moment';

const styles = theme => ({
  fetching: {
    margin: theme.spacing.unit * 4,
    textAlign: "center"
  },
  button: {
    marginRight: theme.spacing.unit * 2,
    marginTop: theme.spacing.unit * 2,
  }
});

class ChamberLog extends Component {

  componentDidMount() {
      this.props.loadData();
  }

  renderChamberLog(data, classes) {
    const dataArray = data.data;

    const labels = [];

    const chamberDataset = {
      label: 'Chamber',
      radius: 0,
      backgroundColor: 'rgba(144,64,0,0.4)',
      borderColor: 'rgba(144,64,0,1)',
      fill: false,
      yAxisID: "y-axis-temperature",
      data: []
    };

    const ambientDataset = {
      label: 'Ambient',
      radius: 0,
      backgroundColor: 'rgba(11,144,0,0.4)',
      borderColor: 'rgba(11,144,0,1)',
      fill: false,
      yAxisID: "y-axis-temperature",
      data: [],
    };

    const heatingDataset = {
      label: 'Heating',
      radius: 0,
      lineTension: 0,
      backgroundColor: 'rgba(244,68,85,0.4)',
      borderColor: 'rgba(244,68,85,0.4)',
      fill: true,
      yAxisID: "y-axis-status",
      data: [],
    };

    const coolingDataset = {
      label: 'Cooling',
      radius: 0,
      lineTension: 0,
      backgroundColor: 'rgba(48,80,240,0.4)',
      borderColor: 'rgba(48,80,240,0.4)',
      fill: true,
      yAxisID: "y-axis-status",
      data: [],
    };

    dataArray.forEach(entry => {
      labels.push(moment.unix(entry.time));
      chamberDataset.data.push(entry.chamber_temp);
      ambientDataset.data.push(entry.ambient_temp);
      heatingDataset.data.push(entry.status === 1 ? 1 : null);
      coolingDataset.data.push(entry.status === 2 ? 1 : null);
    });

    const chartData =  {
      labels:labels,
      datasets:[chamberDataset,ambientDataset,heatingDataset,coolingDataset]
    };

    const chartOptions = {
      fill: false,
      responsive: true,
      legend: {
          display: true,
          position: 'bottom'
      },
      animation: {
          duration: 0
      },
      scales: {
          xAxes: [{
              type: 'time',
              display: true
          }],
          yAxes: [{
              position: "left",
              ticks: {
                  beginAtZero: false,
              },
              display: true,
              scaleLabel: {
                  display: true,
                  labelString: "Temperature °C",
              },
              id: "y-axis-temperature"
          },{
              ticks: {
                  beginAtZero: true,
                  max: 1,
                  stepSize: 1,
              },
              display: false,
              id: "y-axis-status"
          }]
        }
     };

    return  (
      <div>
        <Line data={chartData} options={chartOptions} />
        <Button variant="raised" color="secondary" className={classes.button} onClick={this.props.loadData}>
          Refresh
        </Button>
      </div>
    );
  }

  render() {
    const { data, fetched, errorMessage, classes }  = this.props;

    return (
      <Grid container spacing={0}>
        <Grid item sm={12} md={6}>
          <SectionContent title="Chamber Log">
            {
             !fetched ?
             <div>
               <LinearProgress className={classes.fetching}/>
               <Typography variant="display1" className={classes.fetching}>
                 Loading...
               </Typography>
             </div>
           :
            data ? this.renderChamberLog(data, classes)
           :
            <div>
              <Typography variant="display1" className={classes.fetching}>
                {errorMessage}
              </Typography>
              <Button variant="raised" color="secondary" className={classes.button} onClick={this.props.loadData}>
                Refresh
              </Button>
            </div>
          }
          </SectionContent>
        </Grid>
        <Grid item sm={12} md={6}>
          <SectionContent title="Chamber Log">
            {
             !fetched ?
             <div>
               <LinearProgress className={classes.fetching}/>
               <Typography variant="display1" className={classes.fetching}>
                 Loading...
               </Typography>
             </div>
           :
            data ? this.renderChamberLog(data, classes)
           :
            <div>
              <Typography variant="display1" className={classes.fetching}>
                {errorMessage}
              </Typography>
              <Button variant="raised" color="secondary" className={classes.button} onClick={this.props.loadData}>
                Refresh
              </Button>
            </div>
          }
          </SectionContent>
        </Grid>
      </Grid>
    )
  }

}

export default restComponent(LOG_DATA_ENDPOINT, withStyles(styles)(ChamberLog));
