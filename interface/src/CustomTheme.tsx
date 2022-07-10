import { FC, useMemo } from 'react';
import useMediaQuery from '@mui/material/useMediaQuery';

import { CssBaseline } from '@mui/material';
import { createTheme, responsiveFontSizes, ThemeProvider } from '@mui/material/styles';
import { indigo, blueGrey, orange, red, green} from '@mui/material/colors';

import { RequiredChildrenProps } from './utils';

const CustomTheme: FC<RequiredChildrenProps> = ({ children }) => {
  const prefersDarkMode = useMediaQuery('(prefers-color-scheme: dark)');

  const theme = useMemo(
      () =>
      responsiveFontSizes(
        createTheme({
          palette: {
            mode: prefersDarkMode ? 'dark' : 'light',
            ...(prefersDarkMode
              ? {
                // palette values for dark mode
              }
              : {
                  // palette values for light mode
                  background: {
                    default: "#fafafa",
                  },
                }),
            primary: indigo,
            secondary: blueGrey,
            info: {
              main: indigo[500]
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
          }
        }),
      ),
    [prefersDarkMode],
    );

  return (
    <ThemeProvider theme={theme}>
      <CssBaseline />
      {children}
    </ThemeProvider>
  );
};

export default CustomTheme;
