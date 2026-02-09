import { useEffect, FC, useState, useCallback, useRef } from 'react';
import { useSnackbar } from 'notistack';

import { Button, } from '@mui/material';
import PermScanWifiIcon from '@mui/icons-material/PermScanWifi';

import * as WiFiApi from "../../api/wifi";
import { WiFiNetwork, WiFiNetworkList } from '../../types';
import { ButtonRow, FormLoader, SectionContent } from '../../components';
import { extractErrorMessage } from '../../utils';

import WiFiNetworkSelector from './WiFiNetworkSelector';
import useInterval from '../../utils/useInterval';

const NUM_POLLS = 10;
const POLLING_FREQUENCY = 500;

const compareNetworks = (network1: WiFiNetwork, network2: WiFiNetwork) => {
  if (network1.rssi < network2.rssi) {
    return 1;
  }
  if (network1.rssi > network2.rssi) {
    return -1;
  }
  return 0;
};

const WiFiNetworkScanner: FC = () => {

  const { enqueueSnackbar } = useSnackbar();

  const pollCount = useRef(0);
  const [networkList, setNetworkList] = useState<WiFiNetworkList>();
  const [errorMessage, setErrorMessage] = useState<string>();
  const [pollingDelay, setPollingDelay] = useState<number | undefined>(undefined);

  const stopPolling = useCallback(() => {
    setPollingDelay(undefined);
    pollCount.current = 0;
  }, []);

  const finishedWithError = useCallback((message: string) => {
    enqueueSnackbar(message, { variant: 'error' });
    setNetworkList(undefined);
    setErrorMessage(message);
    stopPolling();
  }, [enqueueSnackbar, stopPolling]);

  const pollNetworkList = useCallback(async () => {
    try {
      const response = await WiFiApi.listNetworks();
      if (response.status === 202) {
        pollCount.current += 1;
        if (pollCount.current >= NUM_POLLS) {
          finishedWithError("Device did not return network list in a timely manner");
        }
      } else {
        const newNetworkList = response.data;
        newNetworkList.networks.sort(compareNetworks);
        setNetworkList(newNetworkList);
        stopPolling();
      }
    } catch (error: any) {
      finishedWithError(extractErrorMessage(error, 'Problem listing WiFi networks'));
    }
  }, [finishedWithError, stopPolling]);

  useInterval(pollNetworkList, pollingDelay);

  const startNetworkScan = useCallback(async () => {
    setNetworkList(undefined);
    setErrorMessage(undefined);
    pollCount.current = 0;

    try {
      await WiFiApi.scanNetworks();
      setPollingDelay(POLLING_FREQUENCY);
    } catch (error: any) {
      finishedWithError(extractErrorMessage(error, 'Problem scanning for WiFi networks'));
    }
  }, [finishedWithError]);

  useEffect(() => {
    startNetworkScan();
  }, [startNetworkScan]);

  const renderNetworkScanner = () => {
    if (!networkList) {
      return (<FormLoader message="Scanning&hellip;" errorMessage={errorMessage} />);
    }
    return (
      <WiFiNetworkSelector networkList={networkList} />
    );
  };

  return (
    <SectionContent title="Network Scanner">
      {renderNetworkScanner()}
      <ButtonRow pt={1}>
        <Button
          startIcon={<PermScanWifiIcon />}
          variant="contained"
          color="secondary"
          onClick={startNetworkScan}
          disabled={!errorMessage && !networkList}
        >
          Scan again&hellip;
        </Button>
      </ButtonRow>
    </SectionContent>
  );

};

export default WiFiNetworkScanner;
