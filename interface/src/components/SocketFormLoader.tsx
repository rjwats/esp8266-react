import React from 'react';

import { makeStyles, Theme, createStyles } from '@material-ui/core/styles';
import { LinearProgress, Typography } from '@material-ui/core';

import { SocketControllerProps } from '.';

const useStyles = makeStyles((theme: Theme) =>
  createStyles({
    loadingSettings: {
      margin: theme.spacing(0.5),
    },
    loadingSettingsDetails: {
      margin: theme.spacing(4),
      textAlign: "center"
    }
  })
);

export type SocketFormProps<D> = Omit<SocketControllerProps<D>, "connected"> & { data: D };

interface SocketFormLoaderProps<D> extends SocketControllerProps<D> {
  render: (props: SocketFormProps<D>) => JSX.Element;
}

export default function SocketFormLoader<D>(props: SocketFormLoaderProps<D>) {
  const { connected, render, data, ...rest } = props;
  const classes = useStyles();
  if (!connected || !data) {
    return (
      <div className={classes.loadingSettings}>
        <LinearProgress className={classes.loadingSettingsDetails} />
        <Typography variant="h6" className={classes.loadingSettingsDetails}>
          Connecting to WebSocket...
        </Typography>
      </div>
    );
  }
  return render({ ...rest, data });
}
