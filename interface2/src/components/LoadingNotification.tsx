import React from 'react';

import { makeStyles, Theme, createStyles } from '@material-ui/core/styles';
import { Button, LinearProgress, Typography } from '@material-ui/core';

const useStyles = makeStyles((theme: Theme) =>
  createStyles({
    loadingSettings: {
      margin: theme.spacing(0.5),
    },
    loadingSettingsDetails: {
      margin: theme.spacing(4),
      textAlign: "center"
    },
    button: {
      marginRight: theme.spacing(2),
      marginTop: theme.spacing(2),
    }
  })
);

interface LoadingNotificationProps {
  fetched: boolean;
  errorMessage?: string;
  onReset: () => void;
  render: () => JSX.Element;
}

export default function LoadingNotification(props: LoadingNotificationProps) {
  const { fetched, errorMessage, onReset, render } = props;
  const classes = useStyles();
  return (
    <div>
      {
        fetched ?
          errorMessage ?
            <div className={classes.loadingSettings}>
              <Typography variant="h6" className={classes.loadingSettingsDetails}>
                {errorMessage}
              </Typography>
              <Button variant="contained" color="secondary" className={classes.button} onClick={onReset}>
                Reset
              </Button>
            </div>
            :
            render()
          :
          <div className={classes.loadingSettings}>
            <LinearProgress className={classes.loadingSettingsDetails} />
            <Typography variant="h6" className={classes.loadingSettingsDetails}>
              Loading...
            </Typography>
          </div>
      }
    </div>
  );
}
