import React, { FC } from 'react';

import { LogEvent, LogLevel } from '../../types';
import { Theme, makeStyles, Box } from '@mui/material';
import { useWindowSize } from '../../components';
import { formatIsoDateTimeToHr } from "../../utils";

interface LogEventConsoleProps {
  events: LogEvent[];
}
interface Offsets {
  topOffset: () => number;
  leftOffset: () => number;
}

const topOffset = () => document.getElementById('serial-tabs')?.getBoundingClientRect().bottom || 0;
const leftOffset = () => document.getElementById('serial-tabs')?.getBoundingClientRect().left || 0;

const useStyles = makeStyles((theme: Theme) => ({
  console: {
    padding: theme.spacing(2),
    position: "absolute",
    left: (offsets: Offsets) => offsets.leftOffset(),
    right: 0,
    top: (offsets: Offsets) => offsets.topOffset(),
    bottom: 0,
    backgroundColor: "black",
    overflow: "auto"
  },
  entry: {
    color: "#bbbbbb",
    fontFamily: "Courier New, monospace",
    fontSize: "14px",
    letterSpacing: "normal",
    whiteSpace: "nowrap"
  },
  file: {
    color: "#00ffff"
  },
  debug: {
    color: "#0000ff"
  },
  info: {
    color: "#00ff00"
  },
  warning: {
    color: "#ffff00"
  },
  error: {
    color: "#ff0000"
  },
  unknown: {
    color: "#ffffff"
  }
}));


const LogEventConsole: FC<LogEventConsoleProps> = (props) => {
  useWindowSize();
  const classes = useStyles({ topOffset, leftOffset });
  const { events } = props;

  const styleLevel = (level: LogLevel) => {
    switch (level) {
      case LogLevel.DEBUG:
        return classes.debug;
      case LogLevel.INFO:
        return classes.info;
      case LogLevel.WARNING:
        return classes.warning;
      case LogLevel.ERROR:
        return classes.error;
      default:
        return classes.unknown;
    }
  }

  const levelLabel = (level: LogLevel) => {
    switch (level) {
      case LogLevel.DEBUG:
        return "DEBUG";
      case LogLevel.INFO:
        return "INFO";
      case LogLevel.WARNING:
        return "WARNING";
      case LogLevel.ERROR:
        return "ERROR";
      default:
        return "UNKNOWN";
    }
  }

  const paddedLevelLabel = (level: LogLevel) => {
    let label = levelLabel(level);
    return label.padStart(7, '\xa0');
  }

  return (
    <Box className={classes.console}>
      {events.map(e => (
        <div className={classes.entry}>
          <span>{formatIsoDateTimeToHr(e.time)} </span>
          <span>{e.message}</span>
        </div>
      ))}
    </Box>
  );
};



export default LogEventConsole;