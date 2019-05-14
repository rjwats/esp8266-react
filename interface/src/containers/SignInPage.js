import React, { Component } from 'react';

import { withStyles } from '@material-ui/core/styles';
import { TextValidator, ValidatorForm } from 'react-material-ui-form-validator';
import Paper from '@material-ui/core/Paper';
import Typography from '@material-ui/core/Typography';
import Fab from '@material-ui/core/Fab';
import { APP_NAME } from '../constants/App';
import ForwardIcon from '@material-ui/icons/Forward';
import { withNotifier } from '../components/SnackbarNotification';
import { SIGN_IN_ENDPOINT } from '../constants/Endpoints';
import { withAuthenticationContext } from '../authentication/Context';

const styles = theme => ({
  loginPage: {
    padding: theme.spacing.unit * 2,
    height: "100vh",
    display: "flex"
  },
  loginPanel: {
    margin: "auto",
    padding: theme.spacing.unit * 2,
    paddingTop: "200px",
    backgroundImage: 'url("/app/icon.png")',
    backgroundRepeat: "no-repeat",
    backgroundPosition: "50% " + theme.spacing.unit * 2 + "px",
    backgroundSize: "auto 150px",
    textAlign: "center"
  },
  extendedIcon: {
    marginRight: theme.spacing.unit,
  },
  loadingSettings: {
    margin: theme.spacing.unit,
  },
  loadingSettingsDetails: {
    margin: theme.spacing.unit * 4,
    textAlign: "center"
  },
  textField: {
    width: "100%"
  },
  button: {
    marginRight: theme.spacing.unit * 2,
    marginTop: theme.spacing.unit * 2,
  }
});

class LoginPage extends Component {

  constructor(props) {
    super(props);
    this.state = {
      username: '',
      password: '',
      fetched: false
    };
  }

  handleValueChange = name => event => {
    this.setState({ [name]: event.target.value });
  };

  onSubmit = () => {
    const { username, password } = this.state;
    const { authenticationContext } = this.props;
    this.setState({ fetched: false });
    fetch(SIGN_IN_ENDPOINT, {
      method: 'POST',
      body: JSON.stringify({ username, password }),
      headers: new Headers({
        'Content-Type': 'application/json'
      })
    })
      .then(response => {
        if (response.status === 200) {
          return response.json();
        } else if (response.status === 401) {
          throw Error("Login details invalid!");
        } else {
          throw Error("Invalid status code: " + response.status);
        }
      }).then(json =>{
        authenticationContext.signIn(json.access_token);
      })
      .catch(error => {
        this.props.raiseNotification(error.message);
        this.setState({ fetched: true });
      });
  };

  render() {
    const { username, password } = this.state;
    const { classes } = this.props;
    return (
      <div className={classes.loginPage}>
        <Paper className={classes.loginPanel}>
          <Typography variant="h4">{APP_NAME}</Typography>
          <ValidatorForm onSubmit={this.onSubmit}>
            <TextValidator
              validators={['required']}
              errorMessages={['Username is required']}
              name="username"
              label="Username"
              className={classes.textField}
              value={username}
              onChange={this.handleValueChange('username')}
              margin="normal"
            />
            <TextValidator
              validators={['required']}
              errorMessages={['Password is required']}
              name="password"
              label="Password"
              className={classes.textField}
              value={password}
              onChange={this.handleValueChange('password')}
              margin="normal"
            />
            <Fab variant="extended" color="primary" className={classes.button} type="submit">
              <ForwardIcon className={classes.extendedIcon} />
              Sign In
            </Fab>
          </ValidatorForm>
        </Paper>
      </div>
    );
  }

}

export default withAuthenticationContext(withNotifier(withStyles(styles)(LoginPage)));
