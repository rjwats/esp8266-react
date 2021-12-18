import { FC } from 'react';

import { CssBaseline } from '@mui/material';
import { createTheme, responsiveFontSizes, ThemeProvider } from '@mui/material/styles';
import { indigo, blueGrey, orange, red, grey, green } from '@mui/material/colors';

const theme = responsiveFontSizes(
  createTheme({
    palette: {
      mode: "dark",
      text: {
        primary: '#fff',
        secondary: grey[600],
      },
      primary: {
        main: grey[400]
      },
      secondary: {
        main: blueGrey[800]
      },
      info: {
        main: indigo[900]
      },
      warning: {
        main: orange[900]
      },
      error: {
        main: red[900]
      },
      success: {
        main: green[900]
      }
    }
  })
);

const CustomTheme: FC = ({ children }) => (
  <ThemeProvider theme={theme}>
    <CssBaseline />
    {children}
  </ThemeProvider>
);

export default CustomTheme;
