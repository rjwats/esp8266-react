import React, { Component } from 'react';

import { restController, RestControllerProps, SectionContent, RestFormLoader } from '../components';

import ChamberLogForm from './ChamberLogForm';
import { LOG_DATA_ENDPOINT, ChamberDataSet } from './types';

type ChamberLogControllerProps = RestControllerProps<ChamberDataSet>;

class ChamberLogController extends Component<ChamberLogControllerProps> {

  componentDidMount() {
    this.props.loadData();
  }

  render() {
    return (
      <SectionContent title="Chamber Log" titleGutter>
        <RestFormLoader
          {...this.props}
          render={formProps => <ChamberLogForm {...formProps} />}
        />
      </SectionContent>
    );
  }

}

export default restController(LOG_DATA_ENDPOINT, ChamberLogController);
