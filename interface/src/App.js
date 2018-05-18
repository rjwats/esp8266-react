import React, { Component } from 'react';

import AppRouting from './AppRouting';
import SnackbarNotification from './components/SnackbarNotification';

import CssBaseline from '@material-ui/core/CssBaseline';
import blueGrey from '@material-ui/core/colors/blueGrey';
import indigo from '@material-ui/core/colors/indigo';
import orange from '@material-ui/core/colors/orange';
import red from '@material-ui/core/colors/red';
import green from '@material-ui/core/colors/green';

import JssProvider from 'react-jss/lib/JssProvider';
import { create } from 'jss';

import {
  MuiThemeProvider,
  createMuiTheme,
  createGenerateClassName,
  jssPreset,
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

// Class name generator.
const generateClassName = createGenerateClassName();

class App extends Component {
	render() {
	   return (
		 <JssProvider jss={jss} generateClassName={generateClassName}>
			<MuiThemeProvider theme={theme}>
        <SnackbarNotification>
				  <CssBaseline />
          <AppRouting />
        </SnackbarNotification>
			</MuiThemeProvider>
		 </JssProvider>
		)
	}
}

export default App
