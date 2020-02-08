import React, { Component } from 'react';

import { CssBaseline } from '@material-ui/core';
import { MuiThemeProvider, createMuiTheme, StylesProvider } from '@material-ui/core/styles';
import { blueGrey, indigo, orange, red, green } from '@material-ui/core/colors';

const theme = createMuiTheme({
  palette: {
    primary: indigo,
    secondary: blueGrey,
    info: {
      main: blueGrey[900]
    },
    warning: {
      main: orange[500]
    },
    error: {
      main: red[500]
    },
    success: {
      main: green[500]
    }
  },
});

export default class CustomMuiTheme extends Component {

  render() {
    return (
      <StylesProvider>
        <MuiThemeProvider theme={theme}>
          <CssBaseline />
          {this.props.children}
        </MuiThemeProvider>
      </StylesProvider>
    );
  }

}
