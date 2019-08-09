import React, { Component } from 'react';
import PropTypes from 'prop-types';
import { withSnackbar } from 'notistack';

import { SCAN_NETWORKS_ENDPOINT, LIST_NETWORKS_ENDPOINT } from '../constants/Endpoints';
import SectionContent from '../components/SectionContent';
import WiFiNetworkSelector from '../forms/WiFiNetworkSelector';
import { redirectingAuthorizedFetch } from '../authentication/Authentication';

const NUM_POLLS = 10
const POLLING_FREQUENCY = 500
const RETRY_EXCEPTION_TYPE = "retry"

class WiFiNetworkScanner extends Component {

  constructor(props) {
    super(props);
    this.pollCount = 0;
    this.state = {
      scanningForNetworks: true,
      errorMessage: null,
      networkList: null
    };
    this.pollNetworkList = this.pollNetworkList.bind(this);
    this.requestNetworkScan = this.requestNetworkScan.bind(this);
  }

  componentDidMount() {
    this.scanNetworks();
  }

  requestNetworkScan() {
    const { scanningForNetworks } = this.state;
    if (!scanningForNetworks) {
      this.scanNetworks();
    }
  }

  scanNetworks() {
    this.pollCount = 0;
    this.setState({ scanningForNetworks: true, networkList: null, errorMessage: null });
    redirectingAuthorizedFetch(SCAN_NETWORKS_ENDPOINT).then(response => {
      if (response.status === 202) {
        this.schedulePollTimeout();
        return;
      }
      throw Error("Scanning for networks returned unexpected response code: " + response.status);
    }).catch(error => {
      this.props.enqueueSnackbar("Problem scanning: " + error.message, {
        variant: 'error',
      });
      this.setState({ scanningForNetworks: false, networkList: null, errorMessage: error.message });
    });
  }

  schedulePollTimeout() {
    setTimeout(this.pollNetworkList, POLLING_FREQUENCY);
  }

  retryError() {
    return {
      name: RETRY_EXCEPTION_TYPE,
      message: "Network list not ready, will retry in " + POLLING_FREQUENCY + "ms."
    };
  }

  compareNetworks(network1, network2) {
    if (network1.rssi < network2.rssi)
      return 1;
    if (network1.rssi > network2.rssi)
      return -1;
    return 0;
  }

  pollNetworkList() {
    redirectingAuthorizedFetch(LIST_NETWORKS_ENDPOINT)
      .then(response => {
        if (response.status === 200) {
          return response.json();
        }
        if (response.status === 202) {
          if (++this.pollCount < NUM_POLLS) {
            this.schedulePollTimeout();
            throw this.retryError();
          } else {
            throw Error("Device did not return network list in timely manner.");
          }
        }
        throw Error("Device returned unexpected response code: " + response.status);
      })
      .then(json => {
        json.networks.sort(this.compareNetworks)
        this.setState({ scanningForNetworks: false, networkList: json, errorMessage: null })
      })
      .catch(error => {
        if (error.name !== RETRY_EXCEPTION_TYPE) {
          this.props.enqueueSnackbar("Problem scanning: " + error.message, {
            variant: 'error',
          });
          this.setState({ scanningForNetworks: false, networkList: null, errorMessage: error.message });
        }
      });
  }

  render() {
    const { scanningForNetworks, networkList, errorMessage } = this.state;
    return (
      <SectionContent title="Network Scanner">
        <WiFiNetworkSelector scanningForNetworks={scanningForNetworks}
          networkList={networkList}
          errorMessage={errorMessage}
          requestNetworkScan={this.requestNetworkScan}
          selectNetwork={this.props.selectNetwork}
        />
      </SectionContent>
    )
  }

}

WiFiNetworkScanner.propTypes = {
  selectNetwork: PropTypes.func.isRequired
};

export default withSnackbar(WiFiNetworkScanner);
