import React, { Component } from 'react';

import { withStyles } from '@material-ui/core/styles';
import { TextValidator, ValidatorForm } from 'react-material-ui-form-validator';
import Paper from '@material-ui/core/Paper';
import Typography from '@material-ui/core/Typography';
import Fab from '@material-ui/core/Fab';
import { APP_NAME } from '../constants/App';
import ForwardIcon from '@material-ui/icons/Forward';

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
    backgroundPosition: "50% "+ theme.spacing.unit * 2 +"px",
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
      password: ''
    };
  }

  handleValueChange = name => event => {
    this.setState({ [name]: event.target.value });
  };

  onSubmit = event => {
    // TODO
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
              Login
            </Fab>
          </ValidatorForm>
        </Paper>
      </div>
    );
  }

}

export default withStyles(styles)(LoginPage);
