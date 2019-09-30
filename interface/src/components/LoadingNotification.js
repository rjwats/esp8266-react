import React from 'react';
import PropTypes from 'prop-types';

import { makeStyles } from '@material-ui/core/styles';
import Button from '@material-ui/core/Button';
import LinearProgress from '@material-ui/core/LinearProgress';
import Typography from '@material-ui/core/Typography';

const useStyles = makeStyles(theme => ({
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
}));

export default function LoadingNotification(props) {
  const classes = useStyles();
  const { fetched, errorMessage, onReset, render } = props;
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

LoadingNotification.propTypes = {
  fetched: PropTypes.bool.isRequired,
  onReset: PropTypes.func.isRequired,
  errorMessage: PropTypes.string,
  render: PropTypes.func.isRequired
};
