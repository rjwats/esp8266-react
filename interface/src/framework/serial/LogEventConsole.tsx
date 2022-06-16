import React, { FC } from 'react';

import { LogEvent, LogLevel } from '../../types';
import { Box } from '@mui/system';
import { useWindowSize } from '../../components';
import { formatIsoDateTimeToHr } from "../../utils";

import { useTheme } from "@mui/material/styles"

interface LogEventConsoleProps {
  events: LogEvent[];
}
interface Offsets {
  topOffset: () => number;
  leftOffset: () => number;
}

const topOffset = () => document.getElementById('serial-tabs')?.getBoundingClientRect().bottom || 0;
const leftOffset = () => document.getElementById('serial-tabs')?.getBoundingClientRect().left || 0;


const LogEventConsole: FC<LogEventConsoleProps> = (props) => {
  useWindowSize();
  const { events } = props;
  const theme = useTheme()

  const styleLevel = (level: LogLevel) => {
    switch (level) {
      case LogLevel.DEBUG:
        return styles.debug;
      case LogLevel.INFO:
        return styles.info;
      case LogLevel.WARNING:
        return styles.warning;
      case LogLevel.ERROR:
        return styles.error;
      default:
        return styles.unknown;
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

  const  styles = {
    console: {
      padding: theme.spacing(2),
      position: "absolute",
      //left: (offsets: Offsets) => offsets.leftOffset(),
      right: 0,
      //top: (offsets: Offsets) => offsets.topOffset(),
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
  };

  return (
    <Box sx={styles.console}>
      {events.map(e => (
        <Box sx={styles.entry}>
           <Box component="span">{formatIsoDateTimeToHr(e.time)} </Box>
          <Box component="span">{e.message}</Box>
        </Box>
      ))}
    </Box>
  );
};



export default LogEventConsole;