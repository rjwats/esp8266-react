import React, { FC } from 'react';

import { LogEvent, LogLevel } from './types';
import { Theme, makeStyles, Box } from '@material-ui/core';
import { useWindowSize } from '../components';
import { formatIsoDateTimeToHr } from '../ntp/TimeFormat';

interface LogEventConsoleProps {
    events: LogEvent[];
}

const topOffset = () => {
    const wrapper = document.getElementById('system-tabs');
    if (wrapper) {
        return wrapper.getBoundingClientRect().bottom;
    }
    return 0;
}

const useStyles = makeStyles((theme: Theme) => ({
    console: {
        padding: theme.spacing(2),
        height: (topOffset: () => number) => `calc(100vh - ${topOffset()}px)`,
        backgroundColor: "black"
    },
    entry: {
        color: "#bbbbbb",
        fontFamily: "Courier New, monospace",
        fontSize: "14px",
        letterSpacing: "normal"
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
    const classes = useStyles(topOffset);
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
                    <span className={styleLevel(e.level)}>{paddedLevelLabel(e.level)} </span>
                    <span className={classes.file}>{`${e.file}[${e.line}]`} </span>
                    <span>{e.message}</span>
                </div>
            ))}
        </Box>
    );
};



export default LogEventConsole;