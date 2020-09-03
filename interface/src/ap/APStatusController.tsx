import React, { Component } from 'react';

import { restController, RestControllerProps, RestFormLoader, SectionContent } from '../components';
import { AP_STATUS_ENDPOINT } from '../api';

import APStatusForm from './APStatusForm';
import { APStatus } from './types';

type APStatusControllerProps = RestControllerProps<APStatus>;

class APStatusController extends Component<APStatusControllerProps> {

  timeout?: NodeJS.Timeout;

  componentDidMount() {
    this.processTimeout();
  }

  processTimeout = () => {
    // show loader on first request only
    this.props.loadData(!this.timeout);

    // schedule next poll in 1 second
    this.timeout = setTimeout(this.processTimeout, 1000);
  }


  componentWillUnmount() {
    if (this.timeout) {
      clearInterval(this.timeout);
    }
  }

  render() {
    return (
      <SectionContent title="Access Point Status">
        <RestFormLoader
          {...this.props}
          render={formProps => <APStatusForm {...formProps} />}
        />
      </SectionContent>
    )
  }
}

export default restController(AP_STATUS_ENDPOINT, APStatusController);
