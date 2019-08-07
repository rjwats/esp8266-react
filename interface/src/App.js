import React, { Component } from 'react';
import { Redirect, Route, Switch } from 'react-router';

import AppRouting from './AppRouting';
import { SnackbarProvider } from 'notistack';

import CssBaseline from '@material-ui/core/CssBaseline';
import blueGrey from '@material-ui/core/colors/blueGrey';
import indigo from '@material-ui/core/colors/indigo';
import orange from '@material-ui/core/colors/orange';
import red from '@material-ui/core/colors/red';
import green from '@material-ui/core/colors/green';

import { create } from 'jss';
import { StylesProvider, jssPreset } from '@material-ui/styles';

import {
  MuiThemeProvider,
  createMuiTheme
} from '@material-ui/core/styles';

// Our theme
const theme = createMuiTheme({
  palette: {
    primary: indigo,
    secondary: blueGrey,
    highlight_idle: blueGrey[900],
    highlight_warn: orange[500],
    highlight_error: red[500],
    highlight_success: green[500],
  },
});

// JSS instance
const jss = create(jssPreset());

// this redirect forces a call to authenticationContext.refresh() which invalidates the JWT if it is invalid.
const unauthorizedRedirect = () => <Redirect to="/" />;

class App extends Component {
  render() {
    return (
      <StylesProvider jss={jss}>
        <MuiThemeProvider theme={theme}>
          <SnackbarProvider maxSnack={3}>
            <CssBaseline />
            <Switch>
              <Route exact path="/unauthorized" component={unauthorizedRedirect} />
              <Route component={AppRouting} />
            </Switch>
          </SnackbarProvider>
        </MuiThemeProvider>
      </StylesProvider>
    );
  }
}

export default App
