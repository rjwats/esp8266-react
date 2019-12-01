import React, { Component } from 'react';
import { Redirect, Route, Switch } from 'react-router';

import AppRouting from './AppRouting';
import { PROJECT_NAME } from './constants/Env';

import { SnackbarProvider } from 'notistack';
import { create } from 'jss';

import { CssBaseline, IconButton, MuiThemeProvider, createMuiTheme } from '@material-ui/core';
import { StylesProvider, jssPreset } from '@material-ui/styles';
import { blueGrey, indigo, orange, red, green } from '@material-ui/core/colors';
import CloseIcon from '@material-ui/icons/Close';


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

  notistackRef = React.createRef();

  componentDidMount() {
    document.title = PROJECT_NAME;
  }

  onClickDismiss = (key) => () => {
    this.notistackRef.current.closeSnackbar(key);
  }

  render() {
    return (
      <StylesProvider jss={jss}>
        <MuiThemeProvider theme={theme}>
          <SnackbarProvider maxSnack={3} anchorOrigin={{ vertical: 'bottom', horizontal: 'left' }}
            ref={this.notistackRef}
            action={(key) => (
              <IconButton onClick={this.onClickDismiss(key)} size="small">
                <CloseIcon />
              </IconButton>
            )}>
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
