import React from 'react';
import PropTypes from 'prop-types';
import { withStyles } from 'material-ui/styles';
import Snackbar from 'material-ui/Snackbar';
import IconButton from 'material-ui/IconButton';
import CloseIcon from 'material-ui-icons/Close';

const styles = theme => ({
  close: {
    width: theme.spacing.unit * 4,
    height: theme.spacing.unit * 4,
  },
});

class SnackbarNotification extends React.Component {
  state = {
    open: false,
    message: null
  };

  raiseNotification = (message) => {
    this.setState({ open: true, message:message });
  };

  handleClose = (event, reason) => {
    if (reason === 'clickaway') {
      return;
    }
    this.setState({ open: false });
  };

  componentWillReceiveProps(nextProps){
    if (nextProps.notificationRef){
      nextProps.notificationRef(this.raiseNotification);
    }
  }

  render() {
    const { classes } = this.props;
    return (
        <Snackbar
          anchorOrigin={{
            vertical: 'bottom',
            horizontal: 'left',
          }}
          open={this.state.open}
          autoHideDuration={6000}
          onClose={this.handleClose}
          SnackbarContentProps={{
            'aria-describedby': 'message-id',
          }}
          message={<span id="message-id">{this.state.message}</span>}
          action={
            <IconButton
              key="close"
              aria-label="Close"
              color="inherit"
              className={classes.close}
              onClick={this.handleClose}
            >
              <CloseIcon />
            </IconButton>
          }
        />
    );
  }
}

SnackbarNotification.propTypes = {
  classes: PropTypes.object.isRequired,
  notificationRef: PropTypes.func.isRequired,
};

export default withStyles(styles)(SnackbarNotification);
