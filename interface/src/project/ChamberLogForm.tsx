import React, { Component, Fragment } from 'react';
import { ChartDataSets, ChartData, ChartOptions } from "chart.js";
import { Line } from 'react-chartjs-2';
import moment, { Moment } from 'moment';

import { RestFormProps, FormActions, FormButton } from '../components';

import { ChamberDataSet, ChamberData } from './types';
import ChartWrapper from './ChartWrapper';

type ChamberLogFormProps = RestFormProps<ChamberDataSet>;

class ChamberLogForm extends Component<ChamberLogFormProps> {

  render() {
    const { data, loadData } = this.props;
    const labels: Moment[] = [];

    const chamberDataset: ChartDataSets = {
      label: 'Chamber',
      radius: 0,
      backgroundColor: 'rgba(144,64,0,0.4)',
      borderColor: 'rgba(144,64,0,1)',
      fill: false,
      yAxisID: "y-axis-temperature",
      data: []
    };

    const ambientDataset: ChartDataSets = {
      label: 'Ambient',
      radius: 0,
      backgroundColor: 'rgba(11,144,0,0.4)',
      borderColor: 'rgba(11,144,0,1)',
      fill: false,
      yAxisID: "y-axis-temperature",
      data: [],
    };

    const heatingDataset: ChartDataSets = {
      label: 'Heating',
      radius: 0,
      lineTension: 0,
      backgroundColor: 'rgba(244,68,85,0.4)',
      borderColor: 'rgba(244,68,85,0.4)',
      fill: true,
      yAxisID: "y-axis-status",
      data: [],
    };

    const coolingDataset: ChartDataSets = {
      label: 'Cooling',
      radius: 0,
      lineTension: 0,
      backgroundColor: 'rgba(48,80,240,0.4)',
      borderColor: 'rgba(48,80,240,0.4)',
      fill: true,
      yAxisID: "y-axis-status",
      data: [],
    };

    data.data.forEach((entry: ChamberData) => {
      labels.push(moment.unix(entry.time));
      chamberDataset.data!.push(entry.chamber_temp);
      ambientDataset.data!.push(entry.ambient_temp);
      (heatingDataset.data! as Array<number | null>).push(entry.status === 1 ? 1 : null);
      (coolingDataset.data! as Array<number | null>).push(entry.status === 2 ? 1 : null);
    });

    const chartData: ChartData = {
      labels: labels,
      datasets: [chamberDataset, ambientDataset, heatingDataset, coolingDataset]
    };

    const chartOptions: ChartOptions = {
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
        }, {
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

    return (
      <Fragment>
        <ChartWrapper>
        <Line data={chartData} options={chartOptions} />
        </ChartWrapper>
        <FormActions>
          <FormButton variant="contained" color="secondary" onClick={loadData}>
            Refresh
          </FormButton>
        </FormActions>
      </Fragment>
    );

  }

}

export default ChamberLogForm;
