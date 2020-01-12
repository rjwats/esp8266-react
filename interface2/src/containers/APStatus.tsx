import React, { Component } from 'react';

import RestFormLoader from '../components/RestFormLoader';
import SectionContent from '../components/SectionContent';
import { restController, RestControllerProps } from '../components/RestController';
import APStatusForm from '../forms/APStatusForm';
import { AP_STATUS_ENDPOINT } from '../constants/Endpoints';

export interface APStatusData {
  active: boolean;
  ip_address: string;
  mac_address: string;
  station_num: number;
}

type APStatusProps = RestControllerProps<APStatusData>;

class APStatus extends Component<APStatusProps> {

  componentDidMount() {
    this.props.loadData();
  }

  render() {
    return (
      <SectionContent title="AP Status">
        <RestFormLoader
          {...this.props}
          render={formProps => <APStatusForm {...formProps} />}
        />
      </SectionContent>
    )
  }
}

export default restController(AP_STATUS_ENDPOINT, APStatus);
