import React, { Component } from 'react';

import AppRouting from './AppRouting';

import JssProvider from 'react-jss/lib/JssProvider';
import { create } from 'jss';

import Reboot from 'material-ui/Reboot';

import blueGrey from 'material-ui/colors/blueGrey';
import indigo from 'material-ui/colors/indigo';
import orange from 'material-ui/colors/orange';
import red from 'material-ui/colors/red';
import green from 'material-ui/colors/green';

import {
  MuiThemeProvider,
  createMuiTheme,
  createGenerateClassName,
  jssPreset,
} from 'material-ui/styles';

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
				<Reboot />
        <AppRouting />
			</MuiThemeProvider>
		 </JssProvider>
		)
	}
}

export default App
