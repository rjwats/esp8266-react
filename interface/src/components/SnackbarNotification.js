import React, {Fragment} from 'react';
import PropTypes from 'prop-types';
import { withStyles } from '@material-ui/core/styles';
import Snackbar from '@material-ui/core/Snackbar';
import IconButton from '@material-ui/core/IconButton';
import CloseIcon from '@material-ui/icons/Close';

const styles = theme => ({
  close: {
    width: theme.spacing.unit * 4,
    height: theme.spacing.unit * 4,
  },
});

class SnackbarNotification extends React.Component {

  constructor(props) {
    super(props);
    this.raiseNotification=this.raiseNotification.bind(this);
  }

  static childContextTypes = {
    raiseNotification: PropTypes.func.isRequired
  }

  getChildContext = () => {
    return {raiseNotification : this.raiseNotification};
  };

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

  render() {
    const { classes } = this.props;
    return (
      <Fragment>
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
              aria-label="Close"
              color="inherit"
              className={classes.close}
              onClick={this.handleClose}
            >
              <CloseIcon />
            </IconButton>
            }
          />
          {this.props.children}
        </Fragment>
    );
  }
}

SnackbarNotification.propTypes = {
  classes: PropTypes.object.isRequired
};

export default withStyles(styles)(SnackbarNotification);

export function withNotifier(WrappedComponent) {
  return class extends React.Component {
    static contextTypes = {
      raiseNotification: PropTypes.func.isRequired
    };
    render() {
      return <WrappedComponent raiseNotification={this.context.raiseNotification} {...this.props} />;
    }
  };
}
