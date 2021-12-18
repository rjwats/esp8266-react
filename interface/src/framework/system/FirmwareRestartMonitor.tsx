import { useEffect } from 'react';
import { FC, useRef, useState } from 'react';

import * as SystemApi from "../../api/system";
import { FormLoader } from '../../components';

const RESTART_TIMEOUT = 2 * 60 * 1000;
const POLL_TIMEOUT = 2000;
const POLL_INTERVAL = 5000;

const FirmwareRestartMonitor: FC = () => {

  const [failed, setFailed] = useState<boolean>(false);
  const [timeoutId, setTimeoutId] = useState<NodeJS.Timeout>();

  const timeoutAt = useRef(new Date().getTime() + RESTART_TIMEOUT);
  const poll = useRef(async () => {
    try {
      await SystemApi.readSystemStatus(POLL_TIMEOUT);
      document.location.href = "/firmwareUpdated";
    } catch (error: any) {
      if (new Date().getTime() < timeoutAt.current) {
        setTimeoutId(setTimeout(poll.current, POLL_INTERVAL));
      } else {
        setFailed(true);
      }
    }
  });

  useEffect(
    () => {
      poll.current();
    },
    []
  );

  useEffect(() => () => timeoutId && clearTimeout(timeoutId), [timeoutId]);

  return (
    <FormLoader
      message="Device restarting, please wait&hellip;"
      errorMessage={failed ? "Timed out waiting for device to restart." : undefined}
    />
  );

};

export default FirmwareRestartMonitor;
