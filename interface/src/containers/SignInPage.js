import React, { Component } from 'react';
import { withStyles } from '@material-ui/core/styles';
import { TextValidator, ValidatorForm } from 'react-material-ui-form-validator';
import Paper from '@material-ui/core/Paper';
import Typography from '@material-ui/core/Typography';
import Fab from '@material-ui/core/Fab';
import { PROJECT_NAME } from '../constants/Env';
import ForwardIcon from '@material-ui/icons/Forward';
import { withSnackbar } from 'notistack';
import { SIGN_IN_ENDPOINT } from '../constants/Endpoints';
import { withAuthenticationContext } from '../authentication/Context';
import PasswordValidator from '../components/PasswordValidator';

const styles = theme => {
  return {
    loginPage: {
      display: "flex",
      height: "100vh",
      margin: "auto",
      padding: theme.spacing(2),
      justifyContent: "center",
      flexDirection: "column",
      maxWidth: theme.breakpoints.values.sm
    },
    loginPanel: {
      textAlign: "center",
      padding: theme.spacing(2),
      paddingTop: "200px",
      backgroundImage: 'url("/app/icon.png")',
      backgroundRepeat: "no-repeat",
      backgroundPosition: "50% " + theme.spacing(2) + "px",
      backgroundSize: "auto 150px",
      width: "100%"
    },
    extendedIcon: {
      marginRight: theme.spacing(0.5),
    },
    textField: {
      width: "100%"
    },
    button: {
      marginRight: theme.spacing(2),
      marginTop: theme.spacing(2),
    }
  }
}


class SignInPage extends Component {

  constructor(props) {
    super(props);
    this.state = {
      username: '',
      password: '',
      processing: false
    };
  }

  handleValueChange = name => event => {
    this.setState({ [name]: event.target.value });
  };

  onSubmit = () => {
    const { username, password } = this.state;
    const { authenticationContext } = this.props;
    this.setState({ processing: true });
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
          throw Error("Invalid login details.");
        } else {
          throw Error("Invalid status code: " + response.status);
        }
      }).then(json => {
        authenticationContext.signIn(json.access_token);
      })
      .catch(error => {
        this.props.enqueueSnackbar(error.message, {
          variant: 'warning',
        });
        this.setState({ processing: false });
      });
  };

  render() {
    const { username, password, processing } = this.state;
    const { classes } = this.props;
    return (
      <div className={classes.loginPage}>
        <Paper className={classes.loginPanel}>
          <Typography variant="h4">{PROJECT_NAME}</Typography>
          <ValidatorForm onSubmit={this.onSubmit}>
            <TextValidator
              disabled={processing}
              validators={['required']}
              errorMessages={['Username is required']}
              name="username"
              label="Username"
              className={classes.textField}
              value={username}
              onChange={this.handleValueChange('username')}
              margin="normal"
            />
            <PasswordValidator
              disabled={processing}
              validators={['required']}
              errorMessages={['Password is required']}
              name="password"
              label="Password"
              className={classes.textField}
              value={password}
              onChange={this.handleValueChange('password')}
              margin="normal"
            />
            <Fab variant="extended" color="primary" className={classes.button} type="submit" disabled={processing}>
              <ForwardIcon className={classes.extendedIcon} />
              Sign In
            </Fab>
          </ValidatorForm>
        </Paper>
      </div>
    );
  }

}

export default withAuthenticationContext(
  withSnackbar(withStyles(styles)(SignInPage))
);
